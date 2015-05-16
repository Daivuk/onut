#pragma once
#include <Windows.h>
#include <string>
#include <cinttypes>
#include <vector>
#include "object.h"

namespace onut
{
    class RTSSocket : public Object
    {
    public:
        RTSSocket();
        virtual ~RTSSocket();

        bool                isValid() const { return m_sock != INVALID_SOCKET; }
        SOCKET              getSock() const { return m_sock; }
        const std::string&  getIPPort() const { return m_ipport; };
        void                setIPPort(const std::string& ipPort);

    private:
        SOCKET m_sock = INVALID_SOCKET;
        std::string m_ipport;
    };

    /*!
        Blocking call that returns a Socket with ipport.
        Returns nullptr if failed
    */
    RTSSocket *natPunchThrough(const std::string& url);

    class RTSPeer : public Object
    {
    public:
        RTSPeer(const std::string &ipPort, uint64_t playerId);
        virtual ~RTSPeer();

        const std::string& getIPPort() const { return m_ipPort; }
        RTSSocket *getSocket() const { return m_pSocket; }
        uint64_t getPlayerId() const { return m_playerId; }

    private:
        RTSSocket *m_pSocket;
        std::string m_ipPort;
        uint64_t m_playerId;
    };

    class RTS : public Object
    {
    public:
        RTS();
        virtual ~RTS();

        void addMe(RTSSocket *pSocket, uint64_t playerId);
        void addPeer(RTSPeer *pPeer);

    private:
        WSADATA m_wsa;
        RTSSocket *m_pMySocket = nullptr;
        std::vector<RTSPeer *> m_peers;
    };
};
