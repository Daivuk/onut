#pragma once
// Onut
#include <onut/Updater.h>

// STL
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Cloud)
OForwardDeclare(Texture)

namespace onut
{
    class Cloud : public UpdateTarget
    {
    public:
        struct LoginEvent
        {
            bool success;
            std::string errorMessage;
        };
        using LoginCallback = std::function<void(const LoginEvent&)>;

        struct Achievement
        {
            OTextureRef iconTexture;
            std::string id;
            std::string title;
            std::string description;
            bool achieved = false;
            int count = 0;
            int max = 0;
        };
        using Achievements = std::vector<Achievement>;
        using AchievementCallback = std::function<void(Achievement)>;
        using AchievementsCallback = std::function<void(Achievements)>;

        struct LeaderboardEntry
        {
            using Score = int64_t;
            using Rank = uint32_t;

            Score score;
            Rank rank;
            std::string name;
        };
        using Leaderboard = std::vector<LeaderboardEntry>;
        using LeaderboardCallback = std::function<void(Leaderboard)>;

        using Stats = std::unordered_map<std::string, int32_t>;

        static OCloudRef create(const std::string& appId, const std::string& appSecret);

        Cloud(const std::string& appId, const std::string& appSecret);
        virtual ~Cloud() {}

        virtual void login(const std::string& email, const std::string& password, const LoginCallback& callback) = 0;
        virtual void createAccount(const std::string& username, const std::string& email, const std::string& password, const LoginCallback& callback) = 0;
        virtual void logout(const LoginCallback& callback) = 0;
        virtual void getAchievements(const AchievementsCallback& callback) = 0;
        virtual void getLeaderboard(const std::string& leaderboardName, int count, const LeaderboardCallback& callback) = 0;
        virtual void sendEvent(const std::string& eventName, int multiplier = 0) = 0;
        virtual void postScore(const std::string& leaderboardName, LeaderboardEntry::Score score) = 0;
        Stats getStats() const;
        int32_t getStat(const std::string& statName) const;

        const std::string& getUsername() const;
        void setAchievementCallback(const AchievementCallback& callback);

    protected:
        void setUsername(const std::string& username);

        Stats m_stats;
        std::string m_appId;
        std::string m_appSecret;
        std::string m_username;
        AchievementCallback m_achievementCallback;
    };
};

extern OCloudRef oCloud;
