#ifndef SOCKET_TCP_WIN32_H_INCLUDED
#define SOCKET_TCP_WIN32_H_INCLUDED

// onut
#include <onut/SocketTCP.h>

// Platform
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(SocketTCP_Win32)

namespace onut
{
    class SocketTCP_Win32 final : public SocketTCP
    {
    public:
        OSocketTCPRef accept() override;
        bool recv(std::vector<uint8_t> &out) override;
        void send(const void *pData, int size) override;
        void close() override;

        SocketTCP_Win32();
        ~SocketTCP_Win32();

    private:
        friend class SocketTCP;

        SOCKET m_socket = INVALID_SOCKET;
        char m_recvbuf[512];
    };
}

#endif
