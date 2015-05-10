#pragma once
#include <string>
#include <Windows.h>
#include <unordered_map>
#include <string>
#include <atomic>
#include <thread>

namespace onut
{
    class Settings
    {
    public:
        Settings();
        virtual ~Settings();

        const POINT&        getResolution() const { return m_resolution; }
        void                setResolution(const POINT& resolution);

        const std::string   getGameName() const { return m_gameName; }
        void                setGameName(const std::string& gameName);

        bool                getIsResizableWindow() const { return m_isResizableWindow; }
        void                setIsResizableWindow(bool isResizableWindow);

        void                setUserSetting(const std::string& key, const std::string& value);
        const std::string&  getUserSetting(const std::string& key) const;

    private:
        POINT               m_resolution = POINT{800, 600};
        std::string         m_gameName = "Game Name";
        bool                m_isResizableWindow = false;

        std::atomic<bool>   m_isDirty = false;
        std::atomic<bool>   m_requestShutdown = false;
        std::atomic<bool>   m_isSaving = false;
        std::unordered_map<std::string, std::string> m_userSettings;
        std::thread         m_savingThread;
    };
}
