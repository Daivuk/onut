#include "RTS.h"
#include <algorithm>
#include <thread>

namespace onut
{
    RTSSocket::RTSSocket()
    {
        if ((m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
        {
            return;
        }
    }

    RTSSocket::~RTSSocket()
    {
        if (m_sock != INVALID_SOCKET)
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
    }


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
                if ((recv_len = recvfrom(pSocket->getSock(), pBuf, 256, 0, NULL, NULL)) == SOCKET_ERROR)
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

    RTSPeer::RTSPeer(const std::string &ipPort, uint64_t playerId)
        : m_ipPort(ipPort)
        , m_playerId(playerId)
    {
        m_pSocket = new RTSSocket();
    }

    RTSPeer::~RTSPeer()
    {
    }

    RTS::RTS()
    {
        // Initialise winsock
        WSAStartup(MAKEWORD(2, 2), &m_wsa);
    }

    RTS::~RTS()
    {
        if (m_pMySocket) m_pMySocket->release();
        WSACleanup();
    }

    void RTS::addMe(RTSSocket *pSocket, uint64_t playerId)
    {
        pSocket->retain();
    }

    void RTS::addPeer(RTSPeer *in_pPeer)
    {
        in_pPeer->retain();
        for (auto pPeer : m_peers)
        {
            if (pPeer == in_pPeer)
            {
                in_pPeer->release();
                return;
            }
        }
        m_peers.push_back(in_pPeer);
    }
};
