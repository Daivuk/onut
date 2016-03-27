// Onut
#include <onut/Settings.h>

// STL
#include <fstream>

OSettingsRef oSettings = std::make_shared<OSettings>();

namespace onut
{
    Settings::Settings()
    {
        // Load settings
        std::ifstream in("../../usersettings.cfg");
        if (!in.fail())
        {
            std::string line;
            std::getline(in, line);
            std::string key;
            while (!in.eof())
            {
                if (!key.empty())
                {
                    m_userSettings[key] = line;
                    key.clear();
                }
                else
                {
                    key = line;
                }
                getline(in, line);
            }
            in.close();
        }

        // Start saving thread
        m_savingThread = std::thread([this]
        {
            std::unique_lock<std::mutex> locker(m_mutex);
            while (m_isRunning)
            {
                m_conditionVariable.wait(locker);
                if (!m_isDirty) continue;
                m_isDirty = false;

                // Wait a little bit more, maybe the user is doing a bunch of transactions at once
                locker.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                locker.lock();

                // Copy the map so there is not conflics
                auto userSettings = m_userSettings;
                locker.unlock();
                std::ofstream fic("../../usersettings.cfg");
                if (!fic.fail())
                {
                    for (auto &kv : userSettings)
                    {
                        fic << kv.first << std::endl;
                        fic << kv.second << std::endl;
                    }
                    fic.close();
                }
                locker.lock();
            }
        });
    }

    Settings::~Settings()
    {
        m_isRunning = false;
        m_conditionVariable.notify_all();
        m_savingThread.join();
    }

    void Settings::setResolution(const Resolution& resolution)
    {
        m_resolution = resolution;
    }

    void Settings::setGameName(const std::string& gameName)
    {
        m_gameName = gameName;
    }

    void Settings::setIsResizableWindow(bool isResizableWindow)
    {
        m_isResizableWindow = isResizableWindow;
    }

    void Settings::setBorderlessFullscreen(bool isBorderLessFullscreen)
    {
        m_isBorderLessFullscreen = isBorderLessFullscreen;
    }

    void Settings::setIsFixedStep(bool isFixedStep)
    {
        m_isFixedStep = isFixedStep;
    }

    void Settings::setIsEditorMode(bool isEditorMode)
    {
        m_isFixedStep = isEditorMode ? true : m_isFixedStep;
        m_isEditorMode = isEditorMode;
    }

    void Settings::setAppId(const std::string& appId)
    {
        m_appId = appId;
    }

    void Settings::setAppSecret(const std::string& appSecret)
    {
        m_appSecret = appSecret;
    }

    void Settings::setUserSettingDefault(const std::string& key, const std::string& value)
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        auto it = m_userSettings.find(key);
        if (it == m_userSettings.end())
        {
            m_userSettings[key] = value;
            m_isDirty = true;
            m_conditionVariable.notify_one();
        }
    }

    void Settings::setUserSetting(const std::string& key, const std::string& value)
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        auto it = m_userSettings.find(key);
        if (it != m_userSettings.end())
        {
            if (it->second == value)
            {
                return; // No change
            }
        }
        m_userSettings[key] = value;
        m_isDirty = true;
        m_conditionVariable.notify_one();
    }

    const std::string& Settings::getUserSetting(const std::string& key)
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        auto it = m_userSettings.find(key);
        if (it == m_userSettings.end())
        {
            static std::string empty = "";
            return empty;
        }
        return it->second;
    }
}
