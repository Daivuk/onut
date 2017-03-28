#ifndef SOCKET_TCP_UNIX_H_INCLUDED
#define SOCKET_TCP_UNIX_H_INCLUDED

#include "SocketTCP.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class SocketTCP_Unix final : public SocketTCP
{
public:
    SocketTCP_Unix();
    virtual ~SocketTCP_Unix();

    SocketTCPRef accept() override;
    bool recv(vector<uint8_t> &out) override;
    void send(const void *pData, int size) override;
    void close() override;

private:
    friend class SocketTCP;

    int m_socket = -1;
    char m_recvbuf[512];
};

#endif
