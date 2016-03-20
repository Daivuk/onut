#pragma once
#include "onut/TimeInfo.h"
#include "onut/Updater.h"

namespace onut
{
    /**
    Timer class so you can start timed events. This class uses a Anim<> internally
    Template arguments:
    - Tprecision: Precision unit. default float
    */
    class Timer final : public UpdateTarget
    {
    public:
        /**
        Constructor. This will set the start value to 0
        */
        Timer() {}

        /**
        Start timed event
        */
        void start(float duration, const std::function<void()>& callback = nullptr)
        {
            m_progress = 0.f;
            m_duration = duration;
            m_callback = callback;
            oUpdater->registerTarget(this);
        }

        /**
        Stop. Value will stay where it is. So you can get the time it was stopped by calling: getProgress
        */
        void stop(bool callCallback = false)
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
        float getProgress() const
        {
            return m_progress;
        }

        /**
        Check if the timer is running
        */
        bool isRunning() const
        {
            return isUpdateTargetRegistered();
        }

    private:
        void update() override
        {
            m_progress += ODT;
            if (m_progress >= m_duration)
            {
                stop(true);
            }
        }

        float m_progress = 0.f;
        float m_duration = 1.f;
        std::function<void()> m_callback;
    };
}

/**
Sleep the current thread for an amount of miliseconds
@param ms Miliseconds count
*/
template<typename Tduration>
inline void OSleep(const Tduration& duration)
{
    std::this_thread::sleep_for(duration);
}

using OTimer = onut::Timer;
