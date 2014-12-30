#pragma once
#include <chrono>
#include <Windows.h>

namespace onut
{
    template<uint32_t TframePerSecond = 120,
        uint32_t TmaxUpdatePerFrame = 4,
        typename Tprecision = double>
    class TimeInfo
    {
    public:
        int update()
        {
            int framesToUpdate = 0;

            // Get cur time
            auto currentTime = std::chrono::steady_clock::now();

            // Deltas
            auto updateElapsed = currentTime - m_lastUpdateTime;
            m_lastUpdateTime = currentTime;
            auto totalElapsed = currentTime - m_startTime;
            auto totalElapsedMS = std::chrono::duration_cast<std::chrono::milliseconds>(totalElapsed);
            m_totalElapsed = static_cast<Tprecision>(totalElapsedMS.count()) / 1000.0;

            // Progress current frame
            m_currentFrameProgress += updateElapsed;
            while (m_currentFrameProgress > m_timePerFrame)
            {
                ++framesToUpdate;
                if (framesToUpdate > TmaxUpdatePerFrame)
                {
                    framesToUpdate = TmaxUpdatePerFrame;
                    m_currentFrameProgress = std::chrono::microseconds(0);
                    break;
                }
                m_currentFrameProgress -= m_timePerFrame;
            }

            return framesToUpdate;
        }

        /**
        @return the time elapsed between this frame and the last one, in seconds.
        Perfect for performing animations
        */
        template<typename TrequestedPrecision = float>
        TrequestedPrecision getDeltaTime() const
        {
            return static_cast<TrequestedPrecision>(m_deltaTime);
        }

        /**
        @return the total time elapsed since the start of the game, in seconds.
        */
        template<typename TrequestedPrecision = float>
        TrequestedPrecision getTotalElapsed() const
        {
            return static_cast<TrequestedPrecision>(m_totalElapsed);
        }

    private:
        Tprecision                                  m_deltaTime = static_cast<Tprecision>(1.0) / static_cast<Tprecision>(TframePerSecond);
        Tprecision                                  m_totalElapsed = static_cast<Tprecision>(0.0);
        decltype(std::chrono::steady_clock::now())  m_startTime = std::chrono::steady_clock::now();
        decltype(std::chrono::steady_clock::now())  m_lastUpdateTime = std::chrono::steady_clock::now();
        std::chrono::steady_clock::duration         m_currentFrameProgress = std::chrono::seconds(0);
        std::chrono::steady_clock::duration         m_timePerFrame = std::chrono::microseconds(1000000) / TframePerSecond;
    };
}
