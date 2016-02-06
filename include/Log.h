#pragma once
#include <string>

namespace onut
{
    enum class eLogSeverity
    {
        Info,
        Warning,
        Error
    };

    void log(eLogSeverity logSeverity, const std::string& message);
};

#define OLog(__msg__) onut::log(onut::eLogSeverity::Info, __msg__)
#define OLogW(__msg__) onut::log(onut::eLogSeverity::Warning, __msg__)
#define OLogE(__msg__) onut::log(onut::eLogSeverity::Error, __msg__)
