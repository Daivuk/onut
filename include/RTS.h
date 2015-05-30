#pragma once
#include <Windows.h>
#include <string>
#include <cinttypes>
#include <vector>
#include <thread>
#include <atomic>
#include <unordered_map>
#include "object.h"
#include "Synchronous.h"

namespace onut
{
    static const int packetSize = 1400;

#pragma pack (push)
#pragma pack (1)
    struct sPacketHeader
    {
        char signature[4];
        uint8_t turnId : 6;
        uint8_t ack : 1;
        uint8_t connection : 1;
        uint64_t playerId;
    };
#pragma pack (pop)

    struct sPacket
    {
        sPacket() {}

        sPacket(char *in, int in_size, const sockaddr_in& in_from)
        {
            size = std::min<>(in_size, packetSize);
            from = in_from;
            memcpy(pBuf, in, packetSize);
        }

        sPacket(const sPacket &other)
        {
            size = other.size;
            from = other.from;
            memcpy(pBuf, other.pBuf, size);
        }

        union
        {
            sPacketHeader header;
            uint8_t pBuf[packetSize];
        };
        int size;
        sockaddr_in from;
    };

    class RTSSocket : public Object
    {
    public:
        RTSSocket();
        RTSSocket(SOCKET parentSocket);
        virtual ~RTSSocket();

        bool                isValid() const { return m_sock != INVALID_SOCKET; }
        SOCKET              getSock() const { return m_sock; }
        const std::string&  getIPPort() const { return m_ipport; };
        void                setIPPort(const std::string& ipPort);
        const sockaddr_in  &getAddr() const { return m_addr; }
        void                setAddr(const sockaddr_in& addr);

    private:
        SOCKET m_sock = INVALID_SOCKET;
        std::string m_ipport;
        sockaddr_in m_addr;
        bool m_ownSocket = false;
    };

    /*!
        Blocking call that returns a Socket with ipport.
        Returns nullptr if failed
    */
    RTSSocket *natPunchThrough(const std::string& url);

    std::vector<std::string> getLocalIPS();

    class RTSPeer : public Object
    {
    public:
        RTSPeer(RTSSocket *pParentSocket, const std::string &ipPort, uint64_t playerId);
        virtual ~RTSPeer();

        const std::string& getIPPort() const { return m_ipPort; }
        RTSSocket *getSocket() const { return m_pSocket; }
        uint64_t getPlayerId() const { return m_playerId; }
        uint8_t getCurrentTurn() const { return m_currentTurn; }
        bool isConnected() const { return m_isConnected; }

    private:
        friend class RTS;

        void queueTurn(const sPacket& packet);
        bool isReadyForTurn(uint8_t turn);
        void extractTurn(uint8_t turn, sPacket *pOut);
        void sendPacket(const sPacket& packet);
        void resendPackets();
        void ackReceived(uint8_t turn);
        void connectionAckReceived(const sPacket& packet);
        void updateConnection(uint64_t parentPlayerId);
        void keepAlive();
        void setIsConnected(const sockaddr_in& addr);

        RTSSocket *m_pSocket;
        std::string m_ipPort;
        uint64_t m_playerId;
        uint8_t m_currentTurn = 0; // 0 - 127
        bool m_isConnected = false;
        std::vector<std::string> m_ips;
        std::string m_port;
        int m_connectionTries = 0;
        int m_connectionAttemptId = 0;

        std::vector<sPacket> queuedTurns;
        std::vector<sPacket> sending;
    };

    class RTS : public Object
    {
    public:
        RTS();
        virtual ~RTS();

        void addMe(RTSSocket *pSocket, uint64_t playerId);
        void addPeer(RTSPeer *pPeer);
        void removePeer(RTSPeer *pPeer);
        const std::vector<RTSPeer *> &getPeers() const { return m_peers; }
        int update();
        void start();
        uint32_t getTurn() const { return m_realTurn; }
        RTSSocket *getSocket() const { return m_pMySocket; }

        void sendCommand(uint8_t cmdId, void *pData);
        void registerCommand(uint8_t cmdId, int cmdSize, const std::function<void(void*,RTSPeer*)> &callback);

    private:
        struct sCmd
        {
            int size;
            std::function<void(void*, RTSPeer*)> callback;
        };

        void stopRecvThread();
        void startRecvThread();
        void onPacket(const sPacket& packet);
        RTSPeer *getPeerFromPacket(const sPacket& packet);
        bool processTurn();
        bool arePlayersReadyForTurn(uint8_t turn);
        void processCommands(uint8_t *pCommands, int size, RTSPeer *pPeer);
        sCmd *getCommand(uint8_t cmdId);
        void resendPackets();
        void updateConnections();
        void keepAlive();
        bool arePeersConnected() const;
        void extractTurn(uint8_t turn, sPacket *pOut);

        WSADATA m_wsa;
        RTSSocket *m_pMySocket = nullptr;
        std::vector<RTSPeer *> m_peers;
        std::thread *m_pRecvThread = nullptr;
        std::atomic<bool> m_isRecvThreadValid = false;
        Synchronous<> m_sync;
        uint8_t m_currentTurn = 0;
        uint32_t m_realTurn = 0;
        bool m_isStarted = false;
        std::chrono::steady_clock::time_point lastTurnTime;
        std::chrono::steady_clock::time_point lastResend;
        std::chrono::steady_clock::time_point lastConnectionAttempt;
        std::chrono::steady_clock::time_point lastKeepAlive;
        std::unordered_map<uint8_t, sCmd> commands;
        sPacket m_commandBuffer;
        uint64_t m_myPlayerId;
        std::vector<sPacket> queuedTurns; // Those are mine
        int m_frame = 0; // Turn frame. 15 Frame per turn (FPT)
    };
};
