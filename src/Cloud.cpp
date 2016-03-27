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
};
