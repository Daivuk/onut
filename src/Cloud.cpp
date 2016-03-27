// Onut
#include <onut/Cloud.h>

OCloudRef oCloud;

namespace onut
{
    Cloud::Cloud(const std::string& appId, const std::string& appSecret)
        : m_appId(appId)
        , m_appSecret(appSecret)
    {
    }

    const std::string& Cloud::getUsername() const
    {
        return m_username;
    }

    void Cloud::setUsername(const std::string& username)
    {
        m_username = username;
    }

    void Cloud::setAchievementCallback(const AchievementCallback& callback)
    {
        m_achievementCallback = callback;
    }

    Cloud::Stats Cloud::getStats() const
    {
        return m_stats;
    }

    int32_t Cloud::getStat(const std::string& statName) const
    {
        auto it = m_stats.find(statName);
        if (it == m_stats.end()) return 0;
        return it->second;
    }
};
