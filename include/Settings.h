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
        void                setIsResizableWindow(bool isBorderLessFullscreen);

        bool                getBorderlessFullscreen() const { return m_isBorderLessFullscreen; }
        void                setBorderlessFullscreen(bool m_isBorderLessFullscreen);

        bool                getIsFixedStep() const { return m_isFixedStep; }
        void                setIsFixedStep(bool isFixedStep);

        bool                getIsEditorMode() const { return m_isEditorMode; }
        void                setIsEditorMode(bool isEditorMode);

        void                setUserSettingDefault(const std::string& key, const std::string& value);
        void                setUserSetting(const std::string& key, const std::string& value);
        const std::string&  getUserSetting(const std::string& key) const;

    private:
        POINT               m_resolution = POINT{800, 600};
        std::string         m_gameName = "Game Name";
        bool                m_isResizableWindow = false;
        bool                m_isFixedStep = true;
        bool                m_isBorderLessFullscreen = false;
        bool                m_isEditorMode = false;

        std::atomic<bool>   m_isDirty = false;
        std::atomic<bool>   m_requestShutdown = false;
        std::atomic<bool>   m_isSaving = false;
        std::unordered_map<std::string, std::string> m_userSettings;
        std::thread         m_savingThread;
    };
}
