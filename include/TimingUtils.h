#pragma once
#include "TimeInfo.h"

namespace onut {
    /**
        Get the time info for the current frame
    */
    const TimeInfo<>& getTimeInfo();
}

/**
    Delta time between current update frame and the previous one
*/
extern float ODT;

/**
    Sleep the current thread for an amount of miliseconds
    @param ms Miliseconds count
*/
template<
    typename TtimeType,
    typename std::enable_if<std::is_integral<TtimeType>::value, TtimeType>::type* = nullptr>
inline void OSleep(const TtimeType& ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
