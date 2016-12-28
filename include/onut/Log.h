#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

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

    void initLog();
    void drawLog();
};

void OLog(const std::string& message);
void OLogW(const std::string& message);
void OLogE(const std::string& message);

#endif
