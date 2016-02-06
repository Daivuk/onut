#pragma once
#if defined(_WIN32)
#include <Windows.h>
#else
#include <iostream>
#endif

#include "Log.h"
#include <string>

namespace onut
{
    void log(eLogSeverity logSeverity, const std::string& message)
    {
        std::string output;
        switch (logSeverity)
        {
            case eLogSeverity::Info:
                output = "onut: ";
                break;
            case eLogSeverity::Warning:
                output = "onut(WARNING): ";
                break;
            case eLogSeverity::Error:
                output = "onut(ERROR): ";
                break;
        }
#if defined(_WIN32)
        output += message + '\n';
        OutputDebugStringA(output.c_str());
#else
        std::cout << output << message << std::endl;
#endif
    }
};
