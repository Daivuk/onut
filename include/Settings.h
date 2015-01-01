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

        const std::string   getDefaultFont() const { return m_defaultFont; }
        void                setDefaultFont(const std::string& defaultFont);

    private:
        POINT               m_resolution = POINT{800, 600};
        std::string         m_gameName = "Game Name";
        std::string         m_defaultFont = "Mainfnt.fnt";

        bool                m_isDirty = false;
    };
}
