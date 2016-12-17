#define _CRT_SECURE_NO_DEPRECATE

#include <algorithm>
#include <cinttypes>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <string.h>

#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#pragma comment (lib, "Ws2_32.lib")
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include "dirent.h"
#define IS_SOCKET_VALID(__socket__) (__socket__ != INVALID_SOCKET)
#elif defined(__unix__)
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
typedef int SOCKET;
#define IS_SOCKET_VALID(__socket__) (__socket__ >= 0)
#define INVALID_SOCKET (-1)
#define closesocket close
#define SD_SEND SHUT_WR
#endif

#if defined(WIN32)
void killProcessByName(const char *filename)
{
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof(pEntry);
    BOOL hRes = Process32First(hSnapShot, &pEntry);
    while (hRes)
    {
        if (strcmp(pEntry.szExeFile, filename) == 0)
        {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
                                          (DWORD)pEntry.th32ProcessID);
            if (hProcess != NULL)
            {
                TerminateProcess(hProcess, 9);
                CloseHandle(hProcess);
            }
        }
        hRes = Process32Next(hSnapShot, &pEntry);
    }
    CloseHandle(hSnapShot);
}
#endif

int server(const std::string& path, const std::string& port);
int client(const std::string& path);

std::string serverPath;

std::vector<std::string> splitString(const std::string& in_string, char in_delimiter, bool in_removeEmptyElements = true)
{
    std::vector<std::string> elems;
    unsigned int start = 0;
    unsigned int end = 0;
    for (; end < in_string.length(); ++end)
    {
        if (in_string[end] == in_delimiter)
        {
            if (end - start || !in_removeEmptyElements)
            {
                elems.push_back(in_string.substr(start, end - start));
            }
            start = end + 1;
        }
    }
    if (!in_removeEmptyElements && start == in_string.length())
    {
        elems.push_back("");
    }
    if (start < in_string.length())
    {
        if (end - start || !in_removeEmptyElements)
        {
            elems.push_back(in_string.substr(start, end - start));
        }
    }
    return elems;
}

std::string toUpper(const std::string& str)
{
    auto ret = str;
    std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
    return ret;
}

std::string getExtension(const std::string& filename)
{
    auto pos = filename.find_last_of('.');
    if (pos == std::string::npos) return "";
    return toUpper(filename.substr(pos + 1));
}

std::vector<std::string> findAllFiles(const std::string& lookIn, const std::string& extension, bool deepSearch)
{
    std::vector<std::string> ret;

    bool all = extension == "*";
    auto upExt = toUpper(extension);
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(lookIn.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (!strcmp(ent->d_name, "."))
            {
                continue;
            }
            else if (!strcmp(ent->d_name, ".."))
            {
                continue;
            }

            if (ent->d_type & DT_DIR)
            {
                if (deepSearch)
                {
                    auto ret2 = findAllFiles(lookIn + "/" + ent->d_name, extension, deepSearch);
                    ret.insert(ret.end(), ret2.begin(), ret2.end());
                }
            }
            else
            {
                if (all)
                {
                    ret.push_back(lookIn + "/" + ent->d_name);
                }
                else if (toUpper(getExtension(ent->d_name)) == upExt)
                {
                    ret.push_back(lookIn + "/" + ent->d_name);
                }
            }
        }
        closedir(dir);
    }

    return std::move(ret);
}

std::vector<char> getFileData(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    std::vector<char> data = {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    return std::move(data);
}

std::string makeRelativePath(const std::string& in_path, const std::string& in_relativeTo)
{
    auto path = in_path;
    std::replace(path.begin(), path.end(), '\\', '/');
    auto pathSplit = splitString(path, '/');

    auto relativeTo = in_relativeTo;
    std::replace(relativeTo.begin(), relativeTo.end(), '\\', '/');
    auto relativeSplit = splitString(relativeTo, '/');

    while (pathSplit.size() && relativeSplit.size() && pathSplit.front() == relativeSplit.front())
    {
        pathSplit.erase(pathSplit.begin());
        relativeSplit.erase(relativeSplit.begin());
    }

    std::stringstream ss;
    bool bFirst = true;
    for (auto& folder : relativeSplit)
    {
        if (!bFirst) ss << "/";
        bFirst = false;
        ss << "..";
    }
    for (auto& folder : pathSplit)
    {
        if (!bFirst) ss << "/";
        bFirst = false;
        ss << folder;
    }
    return std::move(ss.str());
}

std::string getFilename(const std::string& path)
{
    auto pos = path.find_last_of("\\/");
    if (pos == std::string::npos) return path;
    return path.substr(pos + 1);
}

#if defined(WIN32)
uint64_t to_uint64(FILETIME ft)
{
    return static_cast<uint64_t>(ft.dwHighDateTime) << 32 | ft.dwLowDateTime;
}

uint64_t getFileTime(const std::string& filename)
{
    WIN32_FILE_ATTRIBUTE_DATA info;
    GetFileAttributesExA(filename.c_str(), GetFileExInfoStandard, &info);
    return to_uint64(info.ftLastWriteTime);
}
#endif

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cout << "usage server: PILaunch server path port" << std::endl;
        std::cout << "usage client: PILaunch client path" << std::endl;
        return 1;
    }

    std::string type = argv[1];
    std::string path = argv[2];

    std::transform(type.begin(), type.end(), type.begin(), toupper);

    if (type == "SERVER")
    {
        if (argc < 4)
        {
            std::cout << "usage server: PILaunch server path port" << std::endl;
            return 1;
        }
        serverPath = path;
        return server(path, argv[3]);
    }
    else if (type == "CLIENT") return client(path);

    return 0;
}

#define DEFAULT_BUFLEN 512

#if defined(WIN32)
#endif

void handlePacket(const std::vector<char>& content)
{
    int strCnt = 0;
    for (auto c : content)
    {
        if (c == '\0')
        {
            ++strCnt;
        }
    }
    if (!strCnt) return;
    std::string command = content.data();
    if (command == "STOP")
    {
        std::cout << "STOP" << std::endl;
#if defined(WIN32)
        // Kill teh process
        killProcessByName("onut.exe");
#else
        // Kill teh process
        system("pkill JSStandAlone");
#endif
    }
    else if (command == "PLAY")
    {
        std::cout << "PLAY" << std::endl;
#if defined(WIN32)
        // Kill teh process
        killProcessByName("onut.exe");

        // Launch onut in that folder
        std::string cmdLine = "start onut " + serverPath;
        system(cmdLine.c_str());
#else
        // Kill teh process
        system("pkill JSStandAlone");
        
        // Launch onut in that folder
        std::string cmdLine = "~/github/onut/JSStandAlone/bin/linux/JSStandAlone " + serverPath + " &";
        system(cmdLine.c_str());
#endif
    }
    else if (command == "FILE")
    {
        if (strCnt < 3) return;
        std::string filename = content.data() + command.size() + 1;
        std::string sizeStr = content.data() + command.size() + 1 + filename.size() + 1;
        auto pData = content.data() + command.size() + 1 + filename.size() + 1 + sizeStr.size() + 1;
        int fileSize = 0;
        try
        {
            fileSize = std::stoi(sizeStr);
        }
        catch (...)
        {
            return;
        }
        // Save the file
        auto fullPath = serverPath;
        if (fullPath.back() != '\\' && fullPath.back() != '/')
        {
            fullPath += '/';
        }
        fullPath += filename;
        auto pFic = fopen(fullPath.c_str(), "wb");
        if (pFic)
        {
            fwrite(pData, 1, (size_t)fileSize, pFic);
            fclose(pFic);
            std::cout << "FILE: " << filename << std::endl;
        }
    }
    else if (command == "DEL")
    {
        if (strCnt < 2) return;
        std::string filename = content.data() + command.size() + 1;
        remove(filename.c_str());
        std::cout << "DELETE: " << filename << std::endl;
    }
    else if (command == "CLEAR")
    {
        auto filenames = findAllFiles(serverPath, "*", true);
        for (const auto& filename : filenames)
        {
            remove(filename.c_str());
        }
    }
}

int server(const std::string& path, const std::string& port)
{
#if defined(WIN32)
    WSADATA wsaData;
#endif
    int iResult;
    
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    std::vector<char> content;

#if defined(WIN32)
    struct addrinfo *result = NULL;
    struct addrinfo hints;
#else
    struct sockaddr_in serv_addr, cli_addr;
#endif

    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    
#if defined(WIN32)
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, port.c_str(), &hints, &result);
    if (iResult != 0)
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (!IS_SOCKET_VALID(ListenSocket))
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);
#else
    ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (!IS_SOCKET_VALID(ListenSocket)) 
    {
        printf("socket failed\n");
        return 1;
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    int portno = 3333;
    try
    {
        portno = std::stoi(port);
    }
    catch(...)
    {
        printf("wrong port number, defaulting to 3333\n");
    }
    serv_addr.sin_port = htons(portno);
    iResult = bind(ListenSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (iResult < 0)
    {
        closesocket(ListenSocket);
        printf("bind failed\n");
        return 1;
    }
#endif

#if defined(WIN32)
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
#else
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == -1)
    {
        closesocket(ListenSocket);
        printf("listen failed\n");
        return 1;
    }
#endif

    while (1)
    {
        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (!IS_SOCKET_VALID(ClientSocket))
        {
            closesocket(ListenSocket);
#if defined(WIN32)
            printf("accept failed with error: %d\n", WSAGetLastError());
            WSACleanup();
#else
            printf("accept failed\n");
#endif
            return 1;
        }

        // Receive until the peer shuts down the connection
        content.clear();
        do
        {
            iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0)
            {
                auto pos = content.size();
                content.insert(content.end(), iResult, '\0');
                memcpy(content.data() + pos, recvbuf, iResult);
            }
            else if (iResult < 0)
            {
                closesocket(ClientSocket);
#if defined(WIN32)
                printf("recv failed with error: %d\n", WSAGetLastError());
                WSACleanup();
#else
                printf("recv failed\n");
#endif
                return 1;
            }

        } while (iResult > 0);

        // shutdown the connection since we're done
        iResult = shutdown(ClientSocket, SD_SEND);
#if defined(WIN32)
        if (iResult == SOCKET_ERROR)
        {
            printf("shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }
#else
        if (iResult == -1)
        {
            printf("shutdown failed\n");
            closesocket(ClientSocket);
            return 1;
        }
#endif

        // cleanup
        closesocket(ClientSocket);

        if (!content.empty())
        {
            handlePacket(content);
        }
    }

    // No longer need server socket
    closesocket(ListenSocket);
#if defined(WIN32)
    WSACleanup();
#endif
    
    return 0;
}

#if defined(WIN32)
void sendData(const char* pData, int size, const std::string& ip, const std::string& port)
{
    struct addrinfo hints, *pAddr;
    SOCKET ConnectSocket = INVALID_SOCKET;
    int iResult;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    getaddrinfo(ip.c_str(), port.c_str(), &hints, &pAddr);

    // Attempt to connect to an address until one succeeds
    for (auto ptr = pAddr; ptr != NULL; ptr = ptr->ai_next)
    {
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET)
        {
            std::cout << "FATAL: Failed to create socket" << std::endl;
            freeaddrinfo(pAddr);
            return;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR)
        {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }

        break;
    }

    freeaddrinfo(pAddr);

    if (ConnectSocket == INVALID_SOCKET)
    {
        std::cout << "Failed to connect" << std::endl;
        return;
    }

    // Send the buffer
    iResult = send(ConnectSocket, pData, size, 0);
    if (iResult == SOCKET_ERROR)
    {
        std::cout << "Failed to send" << std::endl;
        closesocket(ConnectSocket);
        return;
    }

    printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        std::cout << "shutdown failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        return;
    }

    // cleanup
    closesocket(ConnectSocket);
}

void sendFile(const std::string& filename, const std::string& path, const std::string& ip, const std::string& port)
{
    auto content = getFileData(filename);
    if (!content.empty())
    {
        //auto relPath = makeRelativePath(filename, path);
        auto relPath = getFilename(filename);
        std::cout << "Transferring: " << relPath << std::endl;
        auto sizeStr = std::to_string(content.size());
        content.insert(content.begin(), relPath.size() + sizeStr.size() + 7, '\0');
        memcpy(content.data(), "FILE", 4);
        memcpy(content.data() + 5, relPath.data(), relPath.size());
        memcpy(content.data() + 6 + relPath.size(), sizeStr.data(), sizeStr.size());
        sendData(content.data(), content.size(), ip, port);
    }
    else
    {
        std::cout << "Skipping (Empty or not found): " << filename << std::endl;
    }
}

void sendDeleteFile(const std::string& filename, const std::string& path, const std::string& ip, const std::string& port)
{
    auto relPath = getFilename(filename);
    std::vector<char> content(4 + relPath.size() + 1, '\0');
    memcpy(content.data(), "DEL", 3);
    memcpy(content.data() + 4, relPath.data(), relPath.size());
    sendData(content.data(), content.size(), ip, port);
}

void sendCommand(const std::string& command, const std::string& ip, const std::string& port)
{
    sendData(command.data(), command.size() + 1, ip, port);
}
#endif

int client(const std::string& path)
{
#if defined(WIN32)
    WSADATA wsaData;
    int iResult;
    SOCKET ConnectSocket = INVALID_SOCKET;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        return 1;
    }

    std::string command;
    std::vector<std::string> split;
    struct FileTime
    {
        std::string filename;
        uint64_t date;
        bool dirty;
    };
    std::vector<FileTime> files;
    std::vector<std::string> filenames;
    std::string ip;
    std::string port;
    while (true)
    {
        std::cout << "> ";
        std::cin >> command;
        std::transform(command.begin(), command.end(), command.begin(), toupper);
        if (command == "EXIT") return 0;
        else if (command == "SETIP")
        {
            std::string ipport;
            std::cin >> ipport;
            split = splitString(ipport, ':');
            if (split.size() != 2)
            {
                std::cout << "SETIP format: xxx.xxx.xxx.xxx:port" << std::endl;
                continue;
            }
            ip = split[0];
            port = split[1];
        }
        else if (command == "PLAY")
        {
            // Find all files
            filenames = findAllFiles(path, "*", true);
            for (const auto& filename : filenames)
            {
                auto it = files.begin();
                for (; it != files.end(); ++it)
                {
                    if (it->filename == filename)
                    {
                        auto date = getFileTime(filename);
                        if (it->date < date)
                        {
                            it->date = date;
                            it->dirty = true;
                        }
                        break;
                    }
                }
                if (it == files.end())
                {
                    // New file
                    files.push_back({filename, getFileTime(filename), true});
                }
            }

            // Send deleted files
            std::vector<std::string> deletedFiles;
            auto it = files.begin();
            for (; it != files.end();)
            {
                bool exists = false;
                for (const auto& filename : filenames)
                {
                    if (filename == it->filename)
                    {
                        exists = true;
                        break;
                    }
                }
                if (!exists)
                {
                    deletedFiles.push_back(it->filename);
                    it = files.erase(it);
                    continue;
                }
                ++it;
            }
            for (auto& filename : deletedFiles)
            {
                sendDeleteFile(filename, path, ip, port);
            }

            // Send modified files
            for (auto& file : files)
            {
                if (file.dirty)
                {
                    // ... magic here
                    sendFile(file.filename, path, ip, port);
                    file.dirty = false;
                }
            }

            // Launch command
            sendCommand("PLAY", ip, port);
        }
        else if (command == "STOP")
        {
            // Force stop the game
            sendCommand("STOP", ip, port);
        }
        else if (command == "CLEAR")
        {
            files.clear();
            sendCommand("CLEAR", ip, port);
        }
    }

    WSACleanup();
    return 0;
#else
    std::cout << "Client not supported on Linux" << std::endl;
    return 1;
#endif
}
