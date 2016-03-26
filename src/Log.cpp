#pragma once
// Onut
#include <onut/Log.h>

// Third party
#if defined(WIN32)
#include <Windows.h>
#endif

// STL
#if !defined(WIN32)
#include <iostream>
#endif

namespace onut
{
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
