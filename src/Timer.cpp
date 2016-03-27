#pragma once
// Onut
#include <onut/Timing.h>
#include <onut/Timer.h>

namespace onut
{
    void Timer::start(float duration, const std::function<void()>& callback)
    {
        m_progress = 0.f;
        m_duration = duration;
        m_callback = callback;
        oUpdater->registerTarget(this);
    }

    void Timer::start(std::chrono::steady_clock::duration duration, const std::function<void()>& callback)
    {
        auto durationUs = std::chrono::duration_cast<std::chrono::microseconds>(duration);
        auto durationS = static_cast<float>(static_cast<double>(durationUs.count()) / 1000000.0);
        start(durationS, callback);
    }

    /**
    Stop. Value will stay where it is. So you can get the time it was stopped by calling: getProgress
    */
    void Timer::stop(bool callCallback)
    {
        oUpdater->unregisterTarget(this);
        if (callCallback)
        {
            auto callback = m_callback;
            m_callback = nullptr;
            callback();
        }
    }

    /**
    Get the current time in the timer's progress. In seconds.
    */
    float Timer::getProgress() const
    {
        return m_progress;
    }

    /**
    Check if the timer is running
    */
    bool Timer::isRunning() const
    {
        return isUpdateTargetRegistered();
    }

    void Timer::update()
    {
        m_progress += ODT;
        if (m_progress >= m_duration)
        {
            stop(true);
        }
    }
}
