#pragma once
// STL
#include <string>

namespace onut
{
    enum class LogSeverity
    {
        Info,
        Warning,
        Error
    };

    void log(LogSeverity logSeverity, const std::string& message);
};

void OLog(const std::string& message);
void OLogW(const std::string& message);
void OLogE(const std::string& message);
