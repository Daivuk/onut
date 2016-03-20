#pragma once
#include "onut/Timing.h"
#include "onut/Timer.h"

namespace onut
{
    void Timer::start(float duration, const std::function<void()>& callback)
    {
        m_progress = 0.f;
        m_duration = duration;
        m_callback = callback;
        oUpdater->registerTarget(this);
    }

    /**
    Stop. Value will stay where it is. So you can get the time it was stopped by calling: getProgress
    */
    void Timer::stop(bool callCallback)
    {
        oUpdater->unregisterTarget(this);
        if (callCallback)
        {
            m_callback();
        }
        m_callback = nullptr;
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
