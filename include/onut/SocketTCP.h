#ifndef SOCKET_TCP_H_INCLUDED
#define SOCKET_TCP_H_INCLUDED

// STL
#include <cinttypes>
#include <memory>
#include <string>
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(SocketTCP)

namespace onut
{
    class SocketTCP
    {
    public:
        static OSocketTCPRef listen(int port);
        static OSocketTCPRef connect(const std::string& ip, int port);

        virtual ~SocketTCP();

        virtual OSocketTCPRef accept() = 0;
        virtual bool recv(std::vector<uint8_t> &out) = 0;
        virtual void send(const void *pData, int size) = 0;
        virtual void close() = 0;

    protected:
        SocketTCP();
    };
}

#endif
