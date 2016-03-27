#pragma once
// Onut
#include <onut/Updater.h>

// STL
#include <chrono>
#include <functional>

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
        Start timed event
        */
        void start(float duration, const std::function<void()>& callback = nullptr);
        void start(std::chrono::steady_clock::duration duration, const std::function<void()>& callback = nullptr);

        /**
        Stop. Value will stay where it is. So you can get the time it was stopped by calling: getProgress
        */
        void stop(bool callCallback = false);

        /**
        Get the current time in the timer's progress. In seconds.
        */
        float getProgress() const;

        /**
        Check if the timer is running
        */
        bool isRunning() const;

    private:
        void update() override;

        float m_progress = 0.f;
        float m_duration = 1.f;
        std::function<void()> m_callback;
    };
}

using OTimer = onut::Timer;
