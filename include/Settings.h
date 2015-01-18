#pragma once
#include <string>
#include <Windows.h>

namespace onut
{
    class Settings
    {
    public:
        void                load(const std::string& filename = "settings.cfg");
        void                save(const std::string& filename = "settings.cfg");

        const POINT&        getResolution() const { return m_resolution; }
        void                setResolution(const POINT& resolution);

        const std::string   getGameName() const { return m_gameName; }
        void                setGameName(const std::string& gameName);

        bool                getIsResizableWindow() const { return m_isResizableWindow; }
        void                setIsResizableWindow(bool isResizableWindow);

    private:
        POINT               m_resolution = POINT{800, 600};
        std::string         m_gameName = "Game Name";
        bool                m_isResizableWindow = false;

        bool                m_isDirty = false;
    };
}
