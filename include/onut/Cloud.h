#pragma once
// Onut
#include <onut/Updater.h>

// STL
#include <functional>
#include <string>
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
            std::string title;
            std::string description;
            bool achieved = false;
            int count = 0;
            int max = 0;
        };
        using Achievements = std::vector<Achievement>;
        using AchievementCallback = std::function<void(Achievements)>;

        static OCloudRef create(const std::string& appId, const std::string& appSecret);

        Cloud(const std::string& appId, const std::string& appSecret);
        virtual ~Cloud() {}

        virtual void login(const std::string& email, const std::string& password, const LoginCallback& callback) = 0;
        virtual void createAccount(const std::string& username, const std::string& email, const std::string& password, const LoginCallback& callback) = 0;
        virtual void logout(const LoginCallback& callback) = 0;
        virtual void getAchievements(const AchievementCallback& callback) = 0;

        const std::string& getUsername() const;

    protected:
        void setUsername(const std::string& username);

        std::string m_appId;
        std::string m_appSecret;
        std::string m_username;
    };
};

extern OCloudRef oCloud;
