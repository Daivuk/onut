#ifndef SOCKET_TCP_UNIX_H_INCLUDED
#define SOCKET_TCP_UNIX_H_INCLUDED

// Onut
#include <onut/SocketTCP.h>

// STL
#include <vector>

// System
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(SocketTCP_Unix)

namespace onut
{
    class SocketTCP_Unix final : public SocketTCP
    {
    public:
        SocketTCP_Unix();
        virtual ~SocketTCP_Unix();

        OSocketTCPRef accept() override;
        bool recv(std::vector<uint8_t> &out) override;
        void send(const void *pData, int size) override;
        void close() override;

    private:
        friend class SocketTCP;

        int m_socket = -1;
        char m_recvbuf[512];
    };
}

#endif
