// Onut
#include <onut/Log.h>
#include <onut/Font.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Strings.h>
#include <onut/SpriteBatch.h>

// Third party
#if defined(WIN32)
#include <Windows.h>
#endif

// STL
#if !defined(WIN32)
#include <iostream>
#endif
#include <mutex>
#include <vector>

namespace onut
{
    static bool s_showOnScreenLog = false;
    static std::mutex s_onScreenLogMutex;
    static std::vector<std::string> s_logHistory;

    void initLog()
    {
        s_showOnScreenLog = oSettings->getShowOnScreenLog();
    }

    void log(LogSeverity logSeverity, const std::string& message)
    {
        std::string output;
        switch (logSeverity)
        {
            case LogSeverity::Info:
                output = "onut: ";
                break;
            case LogSeverity::Warning:
                output = "onut(WARNING): ";
                break;
            case LogSeverity::Error:
                output = "onut(ERROR): ";
                break;
        }
        output += message + '\n';
#if defined(WIN32)
        OutputDebugStringA(output.c_str());
#else
        std::cout << output;
#endif
        if (s_showOnScreenLog)
        {
            auto split = onut::splitString(message, '\n');
            s_onScreenLogMutex.lock();
            for (const auto& line : split) s_logHistory.push_back(line);
            while (s_logHistory.size() > 30) s_logHistory.erase(s_logHistory.begin());
            s_onScreenLogMutex.unlock();
        }
    }

    void drawLog()
    {
        auto pFont = OGetFont("font.fnt");
        if (pFont)
        {
            Vector2 pos(0.0f, OScreenHf - 12.f);
            for (auto rit = s_logHistory.rbegin(); rit != s_logHistory.rend(); ++rit)
            {
                pFont->draw(*rit, pos);
                pos.y -= 12.f;
            }
        }
    }
};

void OLog(const std::string& message)
{
    onut::log(onut::LogSeverity::Info, message);
}

void OLogW(const std::string& message)
{
    onut::log(onut::LogSeverity::Warning, message);
}

void OLogE(const std::string& message)
{
    onut::log(onut::LogSeverity::Error, message);
}
