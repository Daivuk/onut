#include "Settings.h"
#include "onut.h"
#include <fstream>

namespace onut
{
    Settings::Settings()
        : m_savingThread([this]
        {
            while (!m_requestShutdown)
            {
                if (!m_isSaving)
                {
                    if (m_isDirty)
                    {
                        // Wait a little bit more, maybe the user is doing a bunch of transactions at once
                        std::this_thread::sleep_for(std::chrono::milliseconds(200));
                        m_isSaving = true;
                        OSync([this]
                        {
                            m_isDirty = false;
                            OAsync([this](std::unordered_map<std::string, std::string> saveData)
                            {
                                std::ofstream fic("../../usersettings.cfg");
                                if (!fic.fail())
                                {
                                    for (auto &kv : saveData)
                                    {
                                        fic << kv.first << std::endl;
                                        fic << kv.second << std::endl;
                                    }
                                    fic.close();
                                }
                                m_isSaving = false;
                            }, m_userSettings);
                        });
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        })
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
    }

    Settings::~Settings()
    {
        m_savingThread.join();
    }

    void Settings::setResolution(const POINT& resolution)
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

    void Settings::setUserSettingDefault(const std::string& key, const std::string& value)
    {
        auto it = m_userSettings.find(key);
        if (it == m_userSettings.end())
        {
            m_userSettings[key] = value;
            m_isDirty = true;
        }
    }

    void Settings::setUserSetting(const std::string& key, const std::string& value)
    {
        m_userSettings[key] = value;
        m_isDirty = true;
    }

    const std::string& Settings::getUserSetting(const std::string& key) const
    {
        auto it = m_userSettings.find(key);
        if (it == m_userSettings.end())
        {
            static std::string empty = "";
            return empty;
        }
        return it->second;
    }
}
