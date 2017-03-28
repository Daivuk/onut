#ifndef MULTIPLAYER_H_INCLUDED
#define MULTIPLAYER_H_INCLUDED

// Onut

// STL
#include <atomic>
#include <functional>
#include <string>
#include <thread>
#include <vector>
#include <mutex>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Multiplayer)
OForwardDeclare(SocketTCP)

namespace onut
{
    class Multiplayer final
    {
    public:
        class Player
        {
        public:
            Player(const std::string& userId, const std::string& username, int rank);

            const std::string& getUserId() const;
            const std::string& getUsername() const;
            int getRank() const;
            int getTeamIndex() const;

        private:
            friend class Multiplayer;

            Player();

            std::string m_userId;
            std::string m_username;
            int m_rank;
            int m_teamIndex = -1;
        };

        using Players = std::vector<Player>;

        class Lobby
        {
        public:
            const Players& getPlayers() const;

        private:
            friend class Multiplayer;

            Players m_players;
        };

        using LobbyCallback = std::function<void(bool success, const Lobby& lobby)>;

        static OMultiplayerRef create();

        ~Multiplayer();

        void findMatch(const Player& player, const std::string& gameMode, const LobbyCallback& lobbyCallback);
        void stopMatchMaking();

    private:
        using ThreadRef = std::shared_ptr<std::thread>;

        Multiplayer();

        void matchMakingThread();

        LobbyCallback m_lobbyCallback;

        Lobby m_lobby;
        Player m_myPlayer;
        std::string m_gameId;
        std::string m_modeId;

        OSocketTCPRef m_pMatchMakingSocket;
        std::atomic<bool> m_isMatchMaking;
        ThreadRef m_pMatchMakingThread;
        std::mutex m_matchMakingMutex;
    };
};

extern OMultiplayerRef oMultiplayer;

#endif
