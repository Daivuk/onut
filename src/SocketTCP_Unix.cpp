// onut
#include <onut/Log.h>

// Private
#include "SocketTCP_Unix.h"

// STL
#include <memory.h>

namespace onut
{
    OSocketTCPRef SocketTCP::listen(int port)
    {
        auto pRet = std::shared_ptr<SocketTCP_Unix>(new SocketTCP_Unix());
        auto pRaw = pRet.get();

        // Create socket
        pRaw->m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (pRaw->m_socket < 0)
        {
            OLog("listen socket creation failed");
            return nullptr;
        }

        // Bind socket
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port);
        if (::bind(pRaw->m_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        {
            OLog("listen bind failed");
            return nullptr;
        }

        // Start listening
        if (::listen(pRaw->m_socket, SOMAXCONN) == -1)
        {
            OLog("listen failed");
            return nullptr;
        }

        return pRet;
    }

    OSocketTCPRef SocketTCP::connect(const std::string& ip, int port)
    {
        return nullptr;
    }

    SocketTCP_Unix::SocketTCP_Unix()
    {
    }

    SocketTCP_Unix::~SocketTCP_Unix()
    {
        close();
    }

    OSocketTCPRef SocketTCP_Unix::accept()
    {
        // Accept a client socket
        auto clientSocket = ::accept(m_socket, NULL, NULL);
        if (clientSocket < 0)
        {
            OLog("accept failed");
            return nullptr;
        }

        auto pClientSocket = std::shared_ptr<SocketTCP_Unix>(new SocketTCP_Unix());
        pClientSocket->m_socket = clientSocket;
        return pClientSocket;
    }

    bool SocketTCP_Unix::recv(std::vector<uint8_t> &out)
    {
        int size = ::recv(m_socket, m_recvbuf, 512, 0);
        if (size > 0)
        {
            auto pos = out.size();
            out.resize(pos + size);
            memcpy(out.data() + pos, m_recvbuf, size);
            return true;
        }
        else if (size < 0)
        {
            ::close(m_socket);
            m_socket = -1;
            return false;
        }
        return true;
    }

    void SocketTCP_Unix::send(const void *pData, int size)
    {
        if (m_socket == -1)
        {
            return;
        }
        int ret = 0;
        while (ret >= 0 && ret < size)
        {
            ret = ::send(m_socket, (const char *)pData, size, 0);
            if (ret >= 0 && ret <= size)
            {
                size -= ret;
                pData = (const char *)pData + ret;
                continue;
            }
            return;
        }
    }

    void SocketTCP_Unix::close()
    {
        if (m_socket >= 0)
        {
            ::shutdown(m_socket, SHUT_RDWR);
            ::close(m_socket);
            m_socket = -1;
        }
    }
}
