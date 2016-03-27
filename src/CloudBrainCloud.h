#pragma once
// Onut
#include <onut/Cloud.h>

// Third party
#include <braincloud/IServerCallback.h>

// STL
#include <string>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(CloudBrainCloud)
namespace BrainCloud
{
    class BrainCloudClient;
};
namespace Json
{
    class Value;
};

namespace onut
{
    class CloudBrainCloud final : public Cloud, public BrainCloud::IServerCallback
    {
    public:
        CloudBrainCloud(const std::string& appId, const std::string& appSecret);
        ~CloudBrainCloud();

        void login(const std::string& email, const std::string& password, const LoginCallback& callback) override;
        void createAccount(const std::string& username, const std::string& email, const std::string& password, const LoginCallback& callback) override;
        void logout(const LoginCallback& callback) override;
        void getAchievements(const AchievementCallback& callback) override;

    protected:
        void serverCallback(BrainCloud::ServiceName serviceName, BrainCloud::ServiceOperation serviceOperation, const std::string& jsonData) override;
        void serverError(BrainCloud::ServiceName serviceName, BrainCloud::ServiceOperation serviceOperation, int statusCode, int reasonCode, const std::string& jsonError) override;
        void serverWarning(BrainCloud::ServiceName serviceName, BrainCloud::ServiceOperation serviceOperation, int statusCode, int reasonCode, int numRetries, const std::string& statusMessage) override;

    private:
        enum class State
        {
            Offline,
            LogingIn,
            CreatingAcount,
            Online,
            LogingOut
        };

        void update() override;
        void initializeBrainCloud();
        void onGoOnline(const Json::Value& json);

        BrainCloud::BrainCloudClient* m_pBrainCloudClient = nullptr;
        State m_state = State::Offline;
        std::string m_profileId;
    };
};
