#pragma once
#include <future>

#include "Callback.h"
#include "TimeInfo.h"

namespace onut {
    /**
        Get the time info for the current frame
    */
    const TimeInfo& getTimeInfo();

    void syncCallbackToMainLoop(ICallback* pCallback);
    template<typename Tfn, typename ... Targs>
    void syncToMainLoop(Tfn callback, Targs... args) {
        auto pCallback = new Callback<Tfn, Targs...>(callback, args...);
        syncCallbackToMainLoop(pCallback);
    }
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

/**
    Run a task asynchronously from the current thread.
    @param args Function and arguments, or your usual lambda
    @return std::future<> of the type of your functions
*/
template<typename ... Targs>
inline auto OAsync(Targs... args) -> decltype(std::async(args...)) {
    return std::async(std::launch::async, args...);
}

/**
    Synchronize back to main thread. This can also be called from the main thread. It will just be delayed until the next frame.
    @param args Function and arguments, or your usual lambda
    @return nothing. But return type is defined as the type of your callback
*/
template<typename Tfn, typename ... Targs>
inline auto OSync(Tfn callback, Targs... args) -> decltype(callback(args...)) {
    onut::syncToMainLoop(callback, args...);
}
