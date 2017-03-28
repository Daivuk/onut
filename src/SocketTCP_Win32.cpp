// onut
#include <onut/Log.h>

// Private
#include "SocketTCP_Win32.h"

// STL
#include <iostream>
#include <string>

namespace onut
{
    OSocketTCPRef SocketTCP::listen(int port)
    {
        auto pRet = std::shared_ptr<SocketTCP_Win32>(new SocketTCP_Win32());
        auto pRaw = pRet.get();

        struct addrinfo *result = NULL;
        struct addrinfo hints;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        // Resolve the server address and port
        int ret = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
        if (ret != 0)
        {
            OLog("getaddrinfo failed with error: " + std::to_string(ret));
            return nullptr;
        }

        // Create a SOCKET for connecting to server
        pRaw->m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (pRaw->m_socket == INVALID_SOCKET)
        {
            OLog("socket failed with error: " + std::to_string(WSAGetLastError()));
            freeaddrinfo(result);
            return nullptr;
        }

        // Setup the TCP listening socket
        ret = bind(pRaw->m_socket, result->ai_addr, (int)result->ai_addrlen);
        if (ret == SOCKET_ERROR)
        {
            OLog("bind failed with error: " + std::to_string(WSAGetLastError()));
            freeaddrinfo(result);
            return nullptr;
        }

        freeaddrinfo(result);

        ret = ::listen(pRaw->m_socket, SOMAXCONN);
        if (ret == SOCKET_ERROR)
        {
            OLog("listen failed with error: " + std::to_string(WSAGetLastError()));
            return nullptr;
        }

        return pRet;
    }

    OSocketTCPRef SocketTCP::connect(const std::string& ip, int port)
    {
        auto pRet = std::shared_ptr<SocketTCP_Win32>(new SocketTCP_Win32());
        auto pRaw = pRet.get();

        struct addrinfo hints, *pAddr;
        int iResult;

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &hints, &pAddr);

        // Attempt to connect to an address until one succeeds
        for (auto ptr = pAddr; ptr != NULL; ptr = ptr->ai_next)
        {
            // Create a SOCKET for connecting to server
            pRaw->m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
            if (pRaw->m_socket == INVALID_SOCKET)
            {
                OLog("Failed to create socket: " + ip + ":" + std::to_string(port));
                freeaddrinfo(pAddr);
                return nullptr;
            }

            // Connect to server.
            iResult = ::connect(pRaw->m_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (iResult == SOCKET_ERROR)
            {
                closesocket(pRaw->m_socket);
                pRaw->m_socket = INVALID_SOCKET;
                continue;
            }

            break;
        }

        freeaddrinfo(pAddr);

        if (pRaw->m_socket == INVALID_SOCKET)
        {
            OLog("Failed to connect: " + ip + ":" + std::to_string(port));
            return nullptr;
        }

        return pRet;
    }

    SocketTCP_Win32::SocketTCP_Win32()
    {
    }

    SocketTCP_Win32::~SocketTCP_Win32()
    {
        close();
    }

    OSocketTCPRef SocketTCP_Win32::accept()
    {
        // Accept a client socket
        auto clientSocket = ::accept(m_socket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET)
        {
            OLog("accept failed with error:" + std::to_string(WSAGetLastError()));
            return nullptr;
        }

        auto pClientSocket = std::shared_ptr<SocketTCP_Win32>(new SocketTCP_Win32());
        pClientSocket->m_socket = clientSocket;
        return pClientSocket;
    }

    bool SocketTCP_Win32::recv(std::vector<uint8_t> &out)
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
            closesocket(m_socket);
            m_socket = INVALID_SOCKET;
            return false;
        }
        return true;
    }

    void SocketTCP_Win32::send(const void *pData, int size)
    {
        if (m_socket == INVALID_SOCKET)
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

    void SocketTCP_Win32::close()
    {
        if (m_socket != INVALID_SOCKET)
        {
            ::shutdown(m_socket, SD_BOTH);
            closesocket(m_socket);
            m_socket = INVALID_SOCKET;
        }
    }
}
