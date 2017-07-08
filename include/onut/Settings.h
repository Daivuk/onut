#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

// Onut
#include <onut/Point.h>

// STL
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Settings);

namespace onut
{
    class Settings
    {
    public:
        using Resolution = Point;
        using SettingKey = std::string;
        using SettingValue = std::string;

        Settings();
        virtual ~Settings();

        const Resolution& getResolution() const { return m_resolution; }
        void setResolution(const Resolution& resolution);

        const Resolution& getRetroResolution() const { return m_retroResolution; }
        void setRetroResolution(const Resolution& retroResolution);

        bool getIsRetroMode() const { return m_isRetroMode; }
        void setIsRetroMode(bool isRetroMode);

        const std::string& getGameName() const { return m_gameName; }
        void setGameName(const std::string& gameName);

        bool getIsResizableWindow() const { return m_isResizableWindow; }
        void setIsResizableWindow(bool isResizableWindow);

        bool getBorderlessFullscreen() const { return m_isBorderLessFullscreen; }
        void setBorderlessFullscreen(bool m_isBorderLessFullscreen);

        bool getIsFixedStep() const { return m_isFixedStep; }
        void setIsFixedStep(bool isFixedStep);

        bool getIsEditorMode() const { return m_isEditorMode; }
        void setIsEditorMode(bool isEditorMode);

        std::string getAppId() const { return m_appId; }
        void setAppId(const std::string& appId);

        std::string getAppSecret() const { return m_appSecret; }
        void setAppSecret(const std::string& appSecret);

        void setUserSettingDefault(const SettingKey& key, const SettingValue& value);
        void setUserSetting(const SettingKey& key, const SettingValue& value);
        const SettingValue& getUserSetting(const SettingKey& key);

        bool getShowOnScreenLog() const { return m_showOnScreenLog; }
        void setShowOnScreenLog(bool showOnScreenLog);

        std::string getMatchMakingAddress() const { return m_matchMakingAddress; }
        void setMatchMakingAddress(const std::string& matchMakingAddress);

        int getMatchMakingPort() const { return m_matchMakingPort; }
        void setMatchMakingPort(int matchMakingPort);

        bool getShowFPS() const { return m_showFPS; }
        void setShowFPS(bool showFPS);

    private:
        using UserSettings = std::unordered_map<SettingKey, SettingValue>;

        Resolution m_resolution = Resolution{800, 600};
        std::string m_gameName = "Game Name";
        bool m_isResizableWindow = false;
        bool m_isFixedStep = true;
        bool m_isBorderLessFullscreen = false;
        bool m_isEditorMode = false;
        Resolution m_retroResolution = Resolution{256, 224};
        bool m_isRetroMode = false;
        UserSettings m_userSettings;
        std::string m_appId = "";
        std::string m_appSecret = "";
        bool m_showOnScreenLog = false;
        std::string m_matchMakingAddress = "192.168.1.112";
        int m_matchMakingPort = 4444;
        bool m_showFPS = true;

        std::atomic<bool> m_isDirty;
        std::atomic<bool> m_isRunning;
        std::thread m_savingThread;
        std::mutex m_mutex;
        std::condition_variable m_conditionVariable;
    };
}

extern OSettingsRef oSettings;

#endif
