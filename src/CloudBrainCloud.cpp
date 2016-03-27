// Those thirdparty have to be included first
#include <braincloud/BrainCloudClient.h>
#include <braincloud/ServiceName.h>
#include <braincloud/ServiceOperation.h>

// Onut
#include <onut/Async.h>
#include <onut/Dispatcher.h>
#include <onut/Http.h>
#include <onut/Texture.h>

// Private
#include "CloudBrainCloud.h"

// Thirdparty
#include <json/json.h>

// STL
#include <atomic>

namespace onut
{
    class BCCallback : public BrainCloud::IServerCallback
    {
    public:
        struct Event
        {
            bool success;
            int statusCode;
            int reasonCode;
            Json::Value json;
            std::string rawJson;
        };

        using EventCallback = std::function<void(const Event&)>;

        BCCallback(const EventCallback& callback)
            : m_callback(callback)
        {
        }

        void serverCallback(BrainCloud::ServiceName serviceName, BrainCloud::ServiceOperation serviceOperation, const std::string& jsonData) override
        {
            Event evt;
            evt.rawJson = jsonData;
            evt.statusCode = 200;
            evt.reasonCode = 0;
            Json::Reader reader;
            evt.success = reader.parse(jsonData.c_str(), evt.json);
            oDispatcher->dispatch([this, evt]
            {
                m_callback(evt);
                delete this;
            });
        }

        void serverError(BrainCloud::ServiceName serviceName, BrainCloud::ServiceOperation serviceOperation, int statusCode, int reasonCode, const std::string& jsonError) override
        {
            Event evt;
            evt.rawJson = jsonError;
            evt.success = false;
            evt.statusCode = statusCode;
            evt.reasonCode = reasonCode;
            Json::Reader reader;
            reader.parse(jsonError.c_str(), evt.json);
            oDispatcher->dispatch([this, evt]
            {
                m_callback(evt);
                delete this;
            });
        }

        void serverWarning(BrainCloud::ServiceName serviceName, BrainCloud::ServiceOperation serviceOperation, int statusCode, int reasonCode, int numRetries, const std::string& statusMessage) override
        {
            // We don't care
            (void)serviceName;
            (void)serviceOperation;
            (void)statusCode;
            (void)reasonCode;
            (void)numRetries;
            (void)statusMessage;
        }

    private:
        EventCallback m_callback;
    };

    OCloudRef Cloud::create(const std::string& appId, const std::string& appSecret)
    {
        return OMake<CloudBrainCloud>(appId, appSecret);
    }

    CloudBrainCloud::CloudBrainCloud(const std::string& appId, const std::string& appSecret)
        : Cloud(appId, appSecret)
    {
    }

    CloudBrainCloud::~CloudBrainCloud()
    {
    }

    void CloudBrainCloud::update()
    {
        if (m_pBrainCloudClient)
        {
            m_pBrainCloudClient->runCallbacks();
        }
    }

    void CloudBrainCloud::initializeBrainCloud()
    {
        if (m_pBrainCloudClient) return;

        m_pBrainCloudClient = BrainCloud::BrainCloudClient::getInstance();
        m_pBrainCloudClient->initialize("https://sharedprod.braincloudservers.com/dispatcherv2", m_appSecret.c_str(), m_appId.c_str(), "1.0.0");

        oUpdater->registerTarget(this);
    }

    void CloudBrainCloud::onGoOnline(const Json::Value& json)
    {
        m_state = State::Online;
        if (!json["data"].isNull() && json["data"]["playerName"].isString())
        {
            setUsername(json["data"]["playerName"].asString());
        }
        else
        {
            setUsername("PLAYER_NAME_NOT_FOUND");
        }
    }

    void CloudBrainCloud::doRewards(const Json::Value& json)
    {
        // Update stats
        if (!json["data"].isNull() && !json["data"]["statistics"].isNull())
        {
            updateStats(json["data"]["statistics"]);
        }

        if (!json["data"].isNull() && !json["data"]["rewards"].isNull())
        {
            auto& jsonReward = json["data"]["rewards"]; 

            if (m_achievementCallback)
            {
                auto& jsonAchievements = jsonReward["playerAchievements"];
                if (jsonAchievements.isArray())
                {
                    std::vector<std::string> achievementNames;
                    for (auto& jsonAchievement : jsonAchievements)
                    {
                        if (jsonAchievement.isString())
                        {
                            achievementNames.push_back(jsonAchievement.asString());
                        }
                    }
                    if (!achievementNames.empty())
                    {
                        getAchievements([achievementNames, this](Achievements achievements)
                        {
                            if (m_achievementCallback)
                            {
                                for (auto& achievementName : achievementNames)
                                {
                                    for (auto& achievement : achievements)
                                    {
                                        if (achievement.id == achievementName)
                                        {
                                            // Notify the app
                                            m_achievementCallback(achievement);
                                        }
                                    }
                                }
                            }
                        });
                    }
                }
            }
        }
    }

    void CloudBrainCloud::updateStats(const Json::Value& json)
    {
        auto statNames = json.getMemberNames();
        for (auto& statName : statNames)
        {
            m_stats[statName] = static_cast<int32_t>(json[statName].asUInt());
        }
    }

    void CloudBrainCloud::login(const std::string& email, const std::string& password, const LoginCallback& callback)
    {
        initializeBrainCloud();
        m_pBrainCloudClient = BrainCloud::BrainCloudClient::getInstance();
        if (m_state != State::Offline)
        {
            if (callback) callback({false, "Already logged in"});
            return;
        }

        m_state = State::LogingIn;
        m_pBrainCloudClient->getAuthenticationService()->authenticateEmailPassword(email.c_str(), password.c_str(), false, new BCCallback([this, callback](const BCCallback::Event& event)
        {
            if (event.success)
            {
                if (!event.json["data"].isNull() && !event.json["data"]["statistics"].isNull())
                {
                    updateStats(event.json["data"]["statistics"]);
                }
                onGoOnline(event.json);
                if (callback) callback({true, ""});
            }
            else
            {
                m_state = State::Offline;
                if (callback) callback({false, "Failed to login"});
            }
        }));
    }

    void CloudBrainCloud::createAccount(const std::string& username, const std::string& email, const std::string& password, const LoginCallback& callback)
    {
        initializeBrainCloud();
        m_pBrainCloudClient = BrainCloud::BrainCloudClient::getInstance();
        if (m_state != State::Offline)
        {
            if (callback) callback({false, "Already logged in"});
            return;
        }

        m_state = State::CreatingAcount;
        m_pBrainCloudClient->getAuthenticationService()->authenticateEmailPassword(email.c_str(), password.c_str(), true, new BCCallback([this, username, callback](const BCCallback::Event& event)
        {
            if (event.success)
            {
                m_pBrainCloudClient->getPlayerStateService()->updatePlayerName(username.c_str(), new BCCallback([this, username, callback](const BCCallback::Event& event)
                {
                    if (event.success)
                    {
                        onGoOnline(event.json);
                        if (callback) callback({true, ""});
                    }
                    else
                    {
                        m_state = State::Offline;
                        if (callback) callback({false, "Failed to create account or login"});
                    }
                }));
            }
            else
            {
                m_state = State::Offline;
                if (callback) callback({false, "Failed to create account or login"});
            }
        }));
    }

    void CloudBrainCloud::logout(const LoginCallback& callback)
    {
        if (m_state != State::Online) return;
        oUpdater->unregisterTarget(this);
        m_state = State::LogingOut;
        m_pBrainCloudClient->getPlayerStateService()->logout(new BCCallback([this, callback](const BCCallback::Event& event)
        {
            m_pBrainCloudClient->shutdown();
            m_pBrainCloudClient = nullptr;
            m_state = State::Offline;
            m_stats.clear();
            if (callback) callback({true, ""});
        }));
    }

    void CloudBrainCloud::getAchievements(const AchievementsCallback& callback)
    {
        if (m_state != State::Online) return;
        m_pBrainCloudClient->getGamificationService()->readAchievements(true, new BCCallback([this, callback](const BCCallback::Event& event)
        {
            if (event.success)
            {
                if (!event.json["data"].isNull() && event.json["data"]["achievements"].isArray())
                {
                    OAsync([event, callback]
                    {
                        Achievements achievements;
                        auto& jsonAchivements = event.json["data"]["achievements"];
                        for (auto& jsonAchievement : jsonAchivements)
                        {
                            if (!jsonAchievement.isNull() &&
                                jsonAchievement["title"].isString() &&
                                jsonAchievement["description"].isString() &&
                                jsonAchievement["imageUrl"].isString() &&
                                jsonAchievement["status"].isString() &&
                                jsonAchievement["achievementId"].isString())
                            {
                                Achievement achievement;
                                achievement.id = jsonAchievement["achievementId"].asString();
                                achievement.title = jsonAchievement["title"].asString();
                                achievement.description = jsonAchievement["description"].asString();
                                achievement.achieved = jsonAchievement["status"].asString() == "AWARDED";
                                achievement.iconTexture = OHTTPGetTexture(jsonAchievement["imageUrl"].asString());
                                achievements.push_back(achievement);
                            }
                        }
                        OSync([achievements, callback]
                        {
                            if (callback) callback(achievements);
                        });
                    });
                }
                else
                {
                    if (callback) callback(Achievements()); // Just send empty back
                }
            }
            else
            {
                if (callback) callback(Achievements()); // Just send empty back
            }
        }));
    }

    void CloudBrainCloud::getLeaderboard(const std::string& leaderboardName, int count, const LeaderboardCallback& callback)
    {
        if (m_state != State::Online) return;
        m_pBrainCloudClient->getSocialLeaderboardService()->getGlobalLeaderboardPage(leaderboardName.c_str(), BrainCloud::SortOrder::HIGH_TO_LOW, 0, count - 1, false, new BCCallback([this, callback](const BCCallback::Event& event)
        {
            Leaderboard leaderboard;
            if (event.success)
            {
                if (!event.json["data"].isNull() && event.json["data"]["leaderboard"].isArray())
                {
                    auto& jsonLeaderboard = event.json["data"]["leaderboard"];
                    for (auto& jsonEntry : jsonLeaderboard)
                    {
                        if (jsonEntry["name"].isString() &&
                            jsonEntry["rank"].isUInt() &&
                            jsonEntry["score"].isInt())
                        {
                            LeaderboardEntry entry;
                            entry.name = jsonEntry["name"].asString();
                            entry.score = static_cast<LeaderboardEntry::Score>(jsonEntry["score"].asInt());
                            entry.rank = static_cast<LeaderboardEntry::Rank>(jsonEntry["rank"].asInt());
                            leaderboard.push_back(entry);
                        }
                    }
                }
            }
            callback(leaderboard);
        }));
    }

    void CloudBrainCloud::postScore(const std::string& leaderboardName, LeaderboardEntry::Score score)
    {
        if (m_state != State::Online) return;
        m_pBrainCloudClient->getSocialLeaderboardService()->postScoreToLeaderboard(leaderboardName.c_str(), static_cast<int64_t>(score), "", new BCCallback([this](const BCCallback::Event& event)
        {
            if (event.success)
            {
                doRewards(event.json);
            }
        }));
    }

    void CloudBrainCloud::sendEvent(const std::string& eventName, int multiplier)
    {
        if (m_state == State::Online)
        {
            m_pBrainCloudClient->getPlayerStatisticsEventService()->triggerPlayerStatisticsEvent(eventName.c_str(), multiplier, new BCCallback([this](const BCCallback::Event& event)
            {
                if (event.success)
                {
                    doRewards(event.json);
                }
            }));
        }
    }

    void CloudBrainCloud::serverCallback(BrainCloud::ServiceName serviceName, BrainCloud::ServiceOperation serviceOperation, const std::string& jsonData)
    {
        if (m_state != State::Online)
        {
            return; // Mkay?
        }
    }

    void CloudBrainCloud::serverError(BrainCloud::ServiceName serviceName, BrainCloud::ServiceOperation serviceOperation, int statusCode, int reasonCode, const std::string& jsonError)
    {
        if (m_state != State::Online)
        {
            return; // Mkay?
        }
    }

    void CloudBrainCloud::serverWarning(BrainCloud::ServiceName serviceName, BrainCloud::ServiceOperation serviceOperation, int statusCode, int reasonCode, int numRetries, const std::string& statusMessage)
    {
        // We don't care
        (void)serviceName;
        (void)serviceOperation;
        (void)statusCode;
        (void)reasonCode;
        (void)numRetries;
        (void)statusMessage;
    }
};
