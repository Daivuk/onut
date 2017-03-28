// Onut
#include <onut/Dispatcher.h>
#include <onut/Log.h>
#include <onut/Multiplayer.h>
#include <onut/Settings.h>
#include <onut/SocketTCP.h>

#if defined(WIN32)
// Platform
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
static WSADATA wsaData;

// STL
#include <cassert>
#endif

// Third party
#include <json/json.h>

OMultiplayerRef oMultiplayer;

namespace onut
{
    Multiplayer::Player::Player()
    {
    }

    Multiplayer::Player::Player(const std::string& userId, const std::string& username, int rank)
        : m_userId(userId)
        , m_username(username)
        , m_rank(rank)
    {
    }

    const std::string& Multiplayer::Player::getUserId() const
    {
        return m_userId;
    }

    const std::string& Multiplayer::Player::getUsername() const
    {
        return m_username;
    }

    int Multiplayer::Player::getRank() const
    {
        return m_rank;
    }

    int Multiplayer::Player::getTeamIndex() const
    {
        return m_teamIndex;
    }

    const Multiplayer::Players& Multiplayer::Lobby::getPlayers() const
    {
        return m_players;
    }

    OMultiplayerRef Multiplayer::create()
    {
        auto pRet = std::shared_ptr<Multiplayer>(new Multiplayer());
        return pRet;
    }

    Multiplayer::Multiplayer()
    {
        m_isMatchMaking = false;
#if defined(WIN32)
        // Initialize Winsock
        int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
        assert(ret == 0);
#endif
    }

    Multiplayer::~Multiplayer()
    {
        stopMatchMaking();
#if defined(WIN32)
        WSACleanup();
#endif
    }

    void Multiplayer::findMatch(const Player& player, const std::string& gameMode, const LobbyCallback& lobbyCallback)
    {
        // Stop if already going on
        stopMatchMaking();

        if (!lobbyCallback)
        {
            OLog("Missing lobby callback");
            return;
        }

        // Setup our lobby
        m_myPlayer = player;
        m_lobby = Lobby();
        m_gameId = oSettings->getAppId();
        m_modeId = gameMode;

        m_lobbyCallback = lobbyCallback;
        m_isMatchMaking = true;
        m_pMatchMakingThread = std::make_shared<std::thread>(std::bind(&Multiplayer::matchMakingThread, this));
    }

    void Multiplayer::stopMatchMaking()
    {
        m_matchMakingMutex.lock();
        m_lobbyCallback = nullptr;
        m_isMatchMaking = false;
        m_pMatchMakingSocket = nullptr;
        m_matchMakingMutex.unlock();
        if (m_pMatchMakingThread)
        {
            m_pMatchMakingThread->join();
            m_pMatchMakingThread = nullptr;
        }
    }

    void Multiplayer::matchMakingThread()
    {
        // Connect
        m_matchMakingMutex.lock();
        m_pMatchMakingSocket = SocketTCP::connect(oSettings->getMatchMakingAddress(), oSettings->getMatchMakingPort());
        if (!m_pMatchMakingSocket)
        {
            m_isMatchMaking = false;
            if (m_lobbyCallback)
            {
            auto lobby = m_lobby;
            auto callback = m_lobbyCallback;
                oDispatcher->dispatch([callback, lobby]
                {
                    callback(false, lobby);
                });
            }
            m_matchMakingMutex.unlock();
            return;
        }

        // Callback first success with only our player in the lobby
        if (m_lobbyCallback)
        {
            auto lobby = m_lobby;
            auto callback = m_lobbyCallback;
            oDispatcher->dispatch([callback, lobby]
            {
                callback(true, lobby);
            });
        }

        // Send our player info to the server
        std::string packetData;
        {
            Json::Value jsPlayer(Json::objectValue);
            jsPlayer["id"] = m_myPlayer.m_userId;
            jsPlayer["username"] = m_myPlayer.m_username;
            jsPlayer["rank"] = m_myPlayer.m_rank;
            jsPlayer["gameId"] = m_gameId;
            jsPlayer["modeId"] = m_modeId;
            Json::FastWriter jsWriter;
            packetData = jsWriter.write(jsPlayer);
        }
        m_pMatchMakingSocket->send(packetData.data(), packetData.size() + 1);
        m_matchMakingMutex.unlock();

        // Start receiving lobby updates
        std::vector<uint8_t> data;
        std::string jsonStr;
        while (m_pMatchMakingSocket->recv(data))
        {
            m_matchMakingMutex.lock();
            if (!m_isMatchMaking)
            {
                m_matchMakingMutex.unlock();
                return;
            }

            //�Did we reach end of text?
            size_t nullPos = std::string::npos;
            for (size_t i = 0; i < data.size(); ++i)
            {
                if (data[i] == 0)
                {
                    nullPos = i;
                    break;
                }
            }
            if (nullPos != std::string::npos)
            {
                jsonStr = (char *)data.data();
                data.erase(data.begin(), data.begin() + nullPos + 1);

                Json::Reader reader;
                Json::Value root;
                if (!reader.parse(jsonStr, root))
                {
                    OLog("Invalid match making json");
                    break;
                }

                // Update lobby
                const auto& jsTeams = root["teams"];
                if (!jsTeams.isArray())
                {
                    OLog("Invalid match making json");
                    break;
                }

                Lobby lobby;
                for (size_t i = 0; i < jsTeams.size(); ++i)
                {
                    const auto& jsTeam = jsTeams[(int)i];
                    if (!jsTeam.isArray())
                    {
                        OLog("Invalid match making json");
                        m_pMatchMakingSocket = nullptr;
                        m_isMatchMaking = false;
                        m_lobbyCallback = nullptr;
                        return;
                    }
                    for (size_t j = 0; j < jsTeam.size(); ++j)
                    {
                        const auto& jsPlayer = jsTeam[(int)j];
                        if (!jsPlayer.isObject())
                        {
                            OLog("Invalid match making json");
                            m_pMatchMakingSocket = nullptr;
                            m_isMatchMaking = false;
                            m_lobbyCallback = nullptr;
                            return;
                        }
                        const auto& jsUserId = jsPlayer["id"];
                        const auto& jsUsername = jsPlayer["username"];
                        const auto& jsRank = jsPlayer["rank"];
                        if (!jsUserId.isString() ||
                            !jsUsername.isString() ||
                            !jsRank.isInt())
                        {
                            OLog("Invalid match making json");
                            m_pMatchMakingSocket = nullptr;
                            m_isMatchMaking = false;
                            m_lobbyCallback = nullptr;
                            return;
                        }
                        Player player;
                        player.m_userId = jsUserId.asString();
                        player.m_username = jsUserId.asString();
                        player.m_rank = jsRank.asInt();
                        player.m_teamIndex = (int)i;
                        lobby.m_players.push_back(player);
                    }
                }
                if (m_lobbyCallback)
                {
                    m_lobby = lobby;
                    auto callback = m_lobbyCallback;
                    oDispatcher->dispatch([callback, lobby]
                    {
                        callback(true, lobby);
                    });
                }
            }
            m_matchMakingMutex.unlock();
        }

        m_pMatchMakingSocket = nullptr;
        m_isMatchMaking = false;
        m_lobbyCallback = nullptr;
    }
}
