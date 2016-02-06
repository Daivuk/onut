#pragma once
#include "Anim.h"
#include "TimeInfo.h"

namespace onut
{
    /**
    Get the time info for the current frame
    @return Reference on the TimeInfo for the game
    */
    const TimeInfo<>& getTimeInfo();

    /**
    Timer class so you can start timed events. This class uses a Anim<> internally
    Template arguments:
    - Tprecision: Precision unit. default float
    */
    template<typename Tprecision = float>
    class Timer
    {
    public:
        /**
        Constructor. This will set the start value to 0
        */
        Timer() :
            m_anim(static_cast<Tprecision>(0.0))
        {}

        /**
        Start timed event
        */
        void start(Tprecision duration, const std::function<void()>& callback = nullptr)
        {
            m_anim.startKeyframed(static_cast<Tprecision>(0.0), {{duration, duration, TweenType::LINEAR, callback}});
        }

        /**
        Stop. Value will stay where it is. So you can get the time it was stopped by calling: getProgress
        */
        void stop()
        {
            m_anim.stop(false);
        }

        /**
        Get the current time in the timer's progress. In seconds.
        */
        Tprecision getProgress() const
        {
            return m_anim.get();
        }

        /**
        Check if the timer is running
        */
        bool isRunning() const
        {
            return m_anim.isPlaying();
        }

    private:
        Anim<Tprecision> m_anim;
    };
}

/**
Delta time between current update frame and the previous one
*/
extern float ODT;

/**
Sleep the current thread for an amount of miliseconds
@param ms Miliseconds count
*/
template<typename Tduration>
inline void OSleep(const Tduration& duration)
{
    std::this_thread::sleep_for(duration);
}

using OTimer = onut::Timer<float>;
