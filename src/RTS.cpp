#include "RTS.h"
#include "Synchronous.h"
#include "Utils.h"
#include <algorithm>
#include <thread>

namespace onut
{
    static const char signature[4] = {'O', 'R', 'T', '1'};

    RTSSocket *natPunchThrough(const std::string& url)
    {
        // Decompose the address
        auto it = url.find_last_of(':');
        if (it == std::string::npos)
        {
            OutputDebugStringA("natPunchThrough not port in: ");
            OutputDebugStringA(url.c_str());
            OutputDebugStringA("\n");
            return nullptr;
        }
        auto portStr = url.substr(it + 1);
        u_short port = 0;
        try
        {
            port = static_cast<u_short>(std::stoul(portStr));
        }
        catch (std::exception e)
        {
            return nullptr;
        }
        auto addr = url.substr(0, it);

        // We try 10 times
        int tries = 0;
        while (tries++ < 10)
        {
            // Setup the address
            sockaddr_in toAddr = {0};
            toAddr.sin_family = AF_INET;
            toAddr.sin_port = htons(port);
            auto remoteHost = gethostbyname(addr.c_str());
            int i = 0;
            while (remoteHost->h_addr_list[i])
            {
                toAddr.sin_addr.S_un.S_addr = *(u_long *)remoteHost->h_addr_list[i++];

                // Create the socket
                auto pSocket = new RTSSocket();
                if (!pSocket->isValid())
                {
                    pSocket->release();
                    continue;
                }

                if (sendto(pSocket->getSock(), NULL, 0, 0, (struct sockaddr *)&toAddr, sizeof(toAddr)) == SOCKET_ERROR)
                {
                    OutputDebugStringA("Stun failed sendto: ");
                    OutputDebugStringA(addr.c_str());
                    OutputDebugStringA("\n");
                    pSocket->release();
                    continue;
                }

                int recv_len;
                int curBuf = 0;
                char pBuf[256];

                static int timeout = 1000;
                setsockopt(pSocket->getSock(), SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

                // Try to receive some data, this is a blocking call (But it's ok, we're in a thread yo)
                struct sockaddr_in si_other;
                int slen = sizeof(si_other);
                if ((recv_len = recvfrom(pSocket->getSock(), pBuf, 256, 0, (struct sockaddr *)&si_other, &slen)) == SOCKET_ERROR)
                {
                    // We might just have killed the thread
                    OutputDebugStringA("Stun failed recvfrom: ");
                    OutputDebugStringA(addr.c_str());
                    OutputDebugStringA("\n");
                    pSocket->release();
                    continue;
                }
                else
                {
                    if (recv_len)
                    {
                        pBuf[255] = '\0';
                        if (recv_len < 255) pBuf[recv_len] = '\0';
                    }

                    OutputDebugStringA("Stun response from ");
                    OutputDebugStringA(addr.c_str());
                    OutputDebugStringA(": ");
                    OutputDebugStringA(pBuf);
                    OutputDebugStringA("\n");

                    pSocket->setIPPort(pBuf);
                    return pSocket;
                }

                pSocket->release();
            } /* while (remoteHost->h_addr_list[i]) */

            std::this_thread::sleep_for(std::chrono::seconds(1));
        } /* while (tries++ < 10) */

        return nullptr;
    } /* natPunchThrough */

    std::vector<std::string> getLocalIPS()
    {
        std::vector<std::string> IPs;

        char ac[80];
        if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR)
        {
            return std::move(IPs);
        }

        struct hostent *phe = gethostbyname(ac);
        if (phe == 0)
        {
            return std::move(IPs);
        }

        for (int i = 0; phe->h_addr_list[i] != 0; ++i)
        {
            struct in_addr addr;
            memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
            IPs.push_back(inet_ntoa(addr));
        }

        return std::move(IPs);
    }

    RTSSocket::RTSSocket()
    {
        if ((m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
        {
            return;
        }
        m_ownSocket = true;
    }

    RTSSocket::RTSSocket(SOCKET parentSocket)
    {
        m_sock = parentSocket;
    }

    RTSSocket::~RTSSocket()
    {
        if (m_sock != INVALID_SOCKET && m_ownSocket)
        {
            closesocket(m_sock);
        }
    }

    void RTSSocket::setIPPort(const std::string& ipPort)
    {
        m_ipport = ipPort;
        std::replace(m_ipport.begin(), m_ipport.end(), '&', ':');
        if (m_ipport[m_ipport.size() - 1] == ':')
        {
            m_ipport.resize(m_ipport.size() - 1);
        }

        // Decompose the address
        auto it = m_ipport.find_last_of(':');
        if (it == std::string::npos)
        {
            OutputDebugStringA("RTSSocket no port in: ");
            OutputDebugStringA(ipPort.c_str());
            OutputDebugStringA("\n");
            if (m_sock != INVALID_SOCKET)
            {
                closesocket(m_sock);
                m_sock = INVALID_SOCKET;
            }
            return;
        }
        auto portStr = ipPort.substr(it + 1);
        int port = 0;
        try
        {
            port = std::stoi(portStr);
        }
        catch (std::exception e)
        {
            if (m_sock != INVALID_SOCKET)
            {
                closesocket(m_sock);
                m_sock = INVALID_SOCKET;
            }
            return;
        }
        auto addr = ipPort.substr(0, it);

        // Setup the address
        m_addr = {0};
        m_addr.sin_family = AF_INET;
        m_addr.sin_port = htons(port);
        m_addr.sin_addr.S_un.S_addr = inet_addr(addr.c_str());
    }

    void RTSSocket::setAddr(const sockaddr_in& addr)
    {
        m_addr = addr;
    }

    RTSPeer::RTSPeer(RTSSocket *pParentSocket, const std::string &ipPort, uint64_t playerId)
        : m_ipPort(ipPort)
        , m_playerId(playerId)
    {
        m_pSocket = new RTSSocket(pParentSocket->getSock());
        if (!m_pSocket->isValid())
        {
            m_pSocket->release();
            m_pSocket = nullptr;
            return;
        }
        m_pSocket->retain();

        auto split = splitString(m_ipPort, ':');
        if (split.size() < 2)
        {
            m_pSocket->release();
            m_pSocket = nullptr;
            return;
        }
        m_port = split.back();
        for (decltype(split.size()) i = 0; i < split.size() - 1; ++i)
        {
            m_ips.push_back(split[i]);
        }

        m_pSocket->setIPPort(m_ips[0] + ":" + m_port);
    }

    RTSPeer::~RTSPeer()
    {
        if (m_pSocket)
        {
            m_pSocket->release();
        }
    }

    void RTSPeer::updateConnection(uint64_t parentPlayerId)
    {
        m_connectionTries++;
        m_pSocket->setIPPort(m_ips[m_connectionAttemptId] + ":" + m_port);

        // Setup packet
        static sPacket packet;
        memset(packet.pBuf, 0, sizeof(packet.header));
        memcpy(packet.header.signature, signature, 4);
        packet.header.connection = 1;
        packet.header.turnId = m_connectionAttemptId;
        packet.header.playerId = parentPlayerId;
        packet.size = sizeof(packet.header);

        // Send
        auto toAddr = getSocket()->getAddr();
        sendto(getSocket()->getSock(), (char *)packet.pBuf, packet.size, 0, (struct sockaddr *)&toAddr, sizeof(toAddr));
        std::string addrStr = inet_ntoa(toAddr.sin_addr);
        OutputDebugStringA(("Trying to connect: " + addrStr + "\n").c_str());

        // Increment to next IP for next try
        m_connectionAttemptId = (m_connectionAttemptId + 1) % m_ips.size();
    }

    void RTSPeer::keepAlive()
    {
        // Setup packet
        static sPacket packet;
        memset(packet.pBuf, 0, sizeof(packet.header));
        memcpy(packet.header.signature, signature, 4);
        packet.header.connection = 1;
        packet.header.turnId = m_connectionAttemptId;
        packet.header.playerId = 0;
        packet.size = sizeof(packet.header);

        // Send
        auto toAddr = getSocket()->getAddr();
        sendto(getSocket()->getSock(), (char *)packet.pBuf, packet.size, 0, (struct sockaddr *)&toAddr, sizeof(toAddr));
    }

    void RTSPeer::queueTurn(const sPacket& packet)
    {
        // make sure it's not already in there
        for (auto &turn : queuedTurns)
        {
            if (turn.header.turnId == packet.header.turnId)
            {
                return;
            }
        }

        // Add it!
        queuedTurns.push_back(packet);
    }

    bool RTSPeer::isReadyForTurn(uint8_t turnId)
    {
        for (auto &turn : queuedTurns)
        {
            if (turn.header.turnId == turnId)
            {
                return true;
            }
        }
        return false;
    }

    void RTSPeer::extractTurn(uint8_t turnId, sPacket *pOut)
    {
        for (decltype(queuedTurns.size()) i = 0; i < queuedTurns.size(); ++i)
        {
            auto &turn = queuedTurns[i];
            if (turn.header.turnId == turnId)
            {
                memcpy(pOut, turn.pBuf, turn.size);
                pOut->size = turn.size;
                queuedTurns.erase(queuedTurns.begin() + i);
                return;
            }
        }
    }

    void RTS::extractTurn(uint8_t turnId, sPacket *pOut)
    {
        for (decltype(queuedTurns.size()) i = 0; i < queuedTurns.size(); ++i)
        {
            auto &turn = queuedTurns[i];
            if (turn.header.turnId == turnId)
            {
                memcpy(pOut, turn.pBuf, turn.size);
                pOut->size = turn.size;
                queuedTurns.erase(queuedTurns.begin() + i);
                return;
            }
        }
    }

    void RTSPeer::sendPacket(const sPacket& packet)
    {
        sending.push_back(packet);
        auto toAddr = getSocket()->getAddr();
        auto ret = sendto(getSocket()->getSock(), (char *)packet.pBuf, packet.size, 0, (struct sockaddr *)&toAddr, sizeof(toAddr));
        if (ret == SOCKET_ERROR)
        {
            auto err = WSAGetLastError();
            return;
        }
    }

    void RTSPeer::resendPackets()
    {
        auto toAddr = getSocket()->getAddr();
        for (auto &packet : sending)
        {
            auto ret = sendto(getSocket()->getSock(), (char *)packet.pBuf, packet.size, 0, (struct sockaddr *)&toAddr, sizeof(toAddr));
            if (ret == SOCKET_ERROR)
            {
                auto err = WSAGetLastError();
                continue;
            }
        }
    }

    void RTSPeer::ackReceived(uint8_t turnId)
    {
        for (decltype(sending.size()) i = 0; i < sending.size();)
        {
            auto &turn = sending[i];
            if (turn.header.turnId == turnId)
            {
                sending.erase(sending.begin() + i);
                continue;
            }
            ++i;
        }
    }

    void RTSPeer::connectionAckReceived(const sPacket& packet)
    {
        if (isConnected()) return;
        if (packet.header.turnId >= m_ips.size()) return; // That's wrong O_o
        m_isConnected = true;
        std::string ipport = m_ips[packet.header.turnId] + ":" + m_port;
        m_pSocket->setIPPort(ipport);
        m_pSocket->setAddr(packet.from);
        OutputDebugStringA(("Connected to: " + ipport + "\n").c_str());
    }

    RTS::RTS()
    {
        // Initialise winsock
        WSAStartup(MAKEWORD(2, 2), &m_wsa);
    }

    RTS::~RTS()
    {
        stopRecvThread();
        for (auto pPeer : m_peers)
        {
            pPeer->release();
        }
        if (m_pMySocket) m_pMySocket->release();
        WSACleanup();
    }

    void RTS::stopRecvThread()
    {
        if (m_pRecvThread)
        {
            m_isRecvThreadValid = false;
            m_pRecvThread->join();
            delete m_pRecvThread;
            m_pRecvThread = nullptr;
        }
    }

    bool validatePacket(const sPacket& packet)
    {
        if (packet.size < sizeof(sPacketHeader)) return false;
        if (memcmp(packet.pBuf, signature, 4)) return false;
        return true;
    }

    void RTS::startRecvThread()
    {
        stopRecvThread();
        m_isRecvThreadValid = true;
        m_pRecvThread = new std::thread([this]
        {
            int recv_len;
            int curBuf = 0;
            char pBuf[packetSize];
            struct sockaddr_in si_other;
            int slen = sizeof(si_other);

            while (m_isRecvThreadValid)
            {
                int timeout = 100; // Short burst of 100 ms
                setsockopt(m_pMySocket->getSock(), SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

                // Try to receive some data, this is a blocking call (But it's ok, we're in a thread yo)
                if ((recv_len = recvfrom(m_pMySocket->getSock(), pBuf, packetSize, 0, (struct sockaddr *)&si_other, &slen)) == SOCKET_ERROR)
                {
                    if (!recv_len)
                    {
                        // Socket was closed :|
                        return;
                    }
                    auto err = WSAGetLastError();
                    if (err == WSAETIMEDOUT) continue;
                    return; // Shiiiet!
                }

                if (recv_len)
                {
                    sPacket packet(pBuf, recv_len, si_other);
                    if (validatePacket(packet))
                    {
                        m_sync.sync([this](sPacket packet)
                        {
                            onPacket(packet);
                        }, packet);
                    }
                }
            }
        });
    }

    RTSPeer *RTS::getPeerFromPacket(const sPacket& packet)
    {
        for (auto pPeer : m_peers)
        {
            if (pPeer->getPlayerId() == packet.header.playerId)
            {
                return pPeer;
            }
        }
        return nullptr;
    }

    void RTS::onPacket(const sPacket& packet)
    {
        auto pPeer = getPeerFromPacket(packet);
        if (!pPeer) return;
        pPeer->setIsConnected(packet.from);

        if (packet.header.ack)
        {
            // It's a ack. We can safely delete that packet from the sending queue
            if (packet.header.connection)
            {
                pPeer->connectionAckReceived(packet);
            }
            else
            {
                pPeer->ackReceived(packet.header.turnId);
            }
            return;
        }

        // Send back a confirmation to the peer
        static sPacket replyPacket;
        memcpy(&replyPacket, &packet, sizeof(sPacketHeader));
        replyPacket.header.ack = 1;
        replyPacket.header.playerId = m_myPlayerId;
        auto toAddr = pPeer->getSocket()->getAddr();
        sendto(pPeer->getSocket()->getSock(), (char *)replyPacket.pBuf, sizeof(sPacketHeader), 0, (struct sockaddr *)&toAddr, sizeof(toAddr));

        if (packet.header.connection) return;

        // Add to queue
        pPeer->queueTurn(packet);
    }

    void RTSPeer::setIsConnected(const sockaddr_in& addr)
    {
        if (!m_isConnected)
        {
            m_pSocket->setAddr(addr);
            m_isConnected = true;
            std::string addrStr = inet_ntoa(addr.sin_addr);
            OutputDebugStringA(("Connected to: " + addrStr + "\n").c_str());
        }
    }

    void RTS::addMe(RTSSocket *pSocket, uint64_t playerId)
    {
        if (m_pMySocket)
        {
            stopRecvThread();
            m_pMySocket->release();
        }
        m_pMySocket = pSocket;
        if (m_pMySocket)
        {
            m_pMySocket->retain();
        }
        m_myPlayerId = playerId;
        if (m_pMySocket)
        {
            startRecvThread();
        }
    }

    void RTS::addPeer(RTSPeer *in_pPeer)
    {
        in_pPeer->retain();
        if (!in_pPeer->getSocket())
        {
            in_pPeer->release();
            return;
        }
        if (!in_pPeer->getSocket()->isValid())
        {
            in_pPeer->release();
            return;
        }
        for (auto pPeer : m_peers)
        {
            if (pPeer->getPlayerId() == in_pPeer->getPlayerId())
            {
                in_pPeer->release();
                return;
            }
        }
        m_peers.push_back(in_pPeer);

        // Sort by playerId
        std::sort(m_peers.begin(), m_peers.end(), [](RTSPeer *pA, RTSPeer *pB)
        {
            return pA->getPlayerId() < pB->getPlayerId();
        });
    }

    void RTS::removePeer(RTSPeer *in_pPeer)
    {
        for (decltype(m_peers.size()) i = 0; i < m_peers.size(); ++i)
        {
            auto pPeer = m_peers[i];
            if (pPeer == in_pPeer)
            {
                pPeer->release();
                m_peers.erase(m_peers.begin() + i);
                return;
            }
        }
    }

    void RTS::start()
    {
        m_isStarted = true;
        lastKeepAlive = lastConnectionAttempt = lastResend = lastTurnTime = std::chrono::steady_clock::now();
        m_currentTurn = 0;
        m_frame = 0;
        memset(&m_commandBuffer, 0, sizeof(m_commandBuffer));
        m_commandBuffer.size = sizeof(m_commandBuffer.header);

        // Send the turn for the next ones, #1 and #2
        memset(&m_commandBuffer, 0, sizeof(m_commandBuffer));
        memcpy(m_commandBuffer.header.signature, signature, 4);
        m_commandBuffer.header.ack = 0;
        m_commandBuffer.header.playerId = m_myPlayerId;
        m_commandBuffer.size = sizeof(sPacketHeader);
        m_commandBuffer.header.turnId = 1;
        for (auto pPeer : m_peers)
        {
            pPeer->sendPacket(m_commandBuffer);
        }
        m_commandBuffer.header.turnId = 2;
        for (auto pPeer : m_peers)
        {
            pPeer->sendPacket(m_commandBuffer);
        }
        m_commandBuffer.size = sizeof(m_commandBuffer.header);
    }

    static const int FPT = 6;
    static const auto RESEND_TIME = std::chrono::milliseconds(50);
    static const auto CONNECT_ATTEMPT_TIME = std::chrono::milliseconds(500);
    static const auto KEEP_ALIVE_TIME = std::chrono::milliseconds(2000);

    int RTS::update()
    {
        m_sync.processQueue();

        auto now = std::chrono::steady_clock::now();

        if (now - lastConnectionAttempt >= CONNECT_ATTEMPT_TIME)
        {
            // Make sure peers are connected
            updateConnections();
            lastConnectionAttempt = now;
        }

        // Keep alive for connected peers.
        // This is important because if the connection block for whatever
        // reason, like a player get stuck. We need to keep alive otherwise
        // UDP ports might close.
        if (now - lastKeepAlive >= CONNECT_ATTEMPT_TIME)
        {
            // Make sure peers are connected
            keepAlive();
            lastKeepAlive = now;
        }

        bool blocked = false;

        if (m_isStarted)
        {
            // Resend packets until they are acked
            if (now - lastResend >= RESEND_TIME)
            {
                resendPackets();
                lastResend = now;
            }

            // Do the turn logic
            if (arePeersConnected())
            {
                ++m_frame;
                if (m_frame > FPT)
                {
                    auto bProcessed = processTurn();
                    if (bProcessed)
                    {
                        m_currentTurn = (m_currentTurn + 1) % 64;
                        m_realTurn++;
                        m_frame -= FPT;
                    }
                    else
                    {
                        blocked = true;
                        --m_frame;
                    }
                }
            }
            else
            {
                lastTurnTime = now;
            }
        }

        return blocked ? 0 : 1;
    }

    void RTS::updateConnections()
    {
        for (auto pPeer : m_peers)
        {
            if (!pPeer->isConnected())
            {
                pPeer->updateConnection(m_myPlayerId);
            }
        }
    }

    void RTS::keepAlive()
    {
        for (auto pPeer : m_peers)
        {
            if (pPeer->isConnected())
            {
                pPeer->keepAlive();
            }
        }
    }

    bool RTS::arePeersConnected() const
    {
        for (auto pPeer : m_peers)
        {
            if (!pPeer->isConnected()) return false;
        }
        return true;
    }

    void RTS::resendPackets()
    {
        for (auto pPeer : m_peers)
        {
            pPeer->resendPackets();
        }
    }

    bool RTS::arePlayersReadyForTurn(uint8_t turn)
    {
        for (auto pPeer : m_peers)
        {
            if (!pPeer->isReadyForTurn(turn)) return false;
        }
        return true;
    }

    void RTS::registerCommand(uint8_t cmdId, int cmdSize, const std::function<void(void*, RTSPeer*)> &callback)
    {
        commands[cmdId] = {cmdSize, callback};
    }

    void RTS::sendCommand(uint8_t cmdId, void *pData)
    {
        auto pCmd = getCommand(cmdId);
        if (!pCmd) return;
        if (m_commandBuffer.size + pCmd->size + 1 > packetSize)
        {
            //TODO: Queue it for next turn I guess
            return;
        }
        *(m_commandBuffer.pBuf + m_commandBuffer.size) = cmdId;
        memcpy(m_commandBuffer.pBuf + m_commandBuffer.size + 1, pData, pCmd->size);
        m_commandBuffer.size += pCmd->size + 1;
    }

    RTS::sCmd *RTS::getCommand(uint8_t cmdId)
    {
        auto it = commands.find(cmdId);
        if (it == commands.end()) return nullptr;
        return &(it->second);
    }

    void RTS::processCommands(uint8_t *pCommands, int size, RTSPeer *pPeer)
    {
        while (size)
        {
            auto cmdId = *pCommands;
            auto *pCmd = getCommand(cmdId);
            if (!pCmd) return; // That's bad
            auto pCmdData = pCommands + 1;
            if (pCmd->callback)
            {
                pCmd->callback(pCmdData, pPeer);
            }
            pCommands += 1 + pCmd->size;
            size += 1 + pCmd->size;
        }
    }

    bool RTS::processTurn()
    {
        auto turnId = (m_currentTurn + 1) % 64;
        if (!arePlayersReadyForTurn(turnId)) return false;

        // Process commands for this turn on all peers
        static sPacket extractedPacket;
        for (auto pPeer : m_peers)
        {
            pPeer->extractTurn(turnId, &extractedPacket);
            auto headerSize = sizeof(sPacketHeader);
            processCommands(extractedPacket.pBuf + headerSize, extractedPacket.size - headerSize, pPeer);
        }
        extractTurn(turnId, &extractedPacket);
        auto headerSize = sizeof(sPacketHeader);
        processCommands(extractedPacket.pBuf + headerSize, extractedPacket.size - headerSize, nullptr);

        // Package commands for next turn (in 2 turns)
        memcpy(m_commandBuffer.header.signature, signature, 4);
        m_commandBuffer.header.ack = 0;
        m_commandBuffer.header.playerId = m_myPlayerId;
        m_commandBuffer.header.turnId = (turnId + 2) % 64;
        for (auto pPeer : m_peers)
        {
            pPeer->sendPacket(m_commandBuffer);
        }
        queuedTurns.push_back(m_commandBuffer);
        memset(&m_commandBuffer, 0, sizeof(m_commandBuffer));
        m_commandBuffer.size = sizeof(m_commandBuffer.header);

        return true;
    }
};
