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
        int update(bool fixedStep = true)
        {
            int framesToUpdate = 0;

            // Get cur time
            auto currentTime = std::chrono::high_resolution_clock::now();

            // Deltas
            auto updateElapsed = currentTime - m_lastUpdateTime;
            m_lastUpdateTime = currentTime;
            auto totalElapsed = currentTime - m_startTime;
            auto totalElapsedMS = std::chrono::duration_cast<std::chrono::milliseconds>(totalElapsed);
            m_totalElapsed = static_cast<Tprecision>(totalElapsedMS.count()) / 1000.0;

            // Progress current frame
            if (fixedStep)
            {
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

                m_deltaTime = static_cast<Tprecision>(1.0) / static_cast<Tprecision>(TframePerSecond);

                return framesToUpdate;
            }
            else
            {
                m_deltaTime = 
                    static_cast<Tprecision>(std::chrono::duration_cast<std::chrono::microseconds>(updateElapsed).count()) /
                    static_cast<Tprecision>(1000000.0);
                return 1;
            }
        }

        void render()
        {
            m_lastRenderTime = m_thisRenderTime;
            ++m_currentFPS;
            m_thisRenderTime = std::chrono::high_resolution_clock::now();
            if (m_thisRenderTime - m_lastFPSSnapShotTime >= std::chrono::seconds(1))
            {
                m_fps = m_currentFPS;
                m_currentFPS = 0;
                m_lastFPSSnapShotTime = m_thisRenderTime;
            }
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
        @return the time elapsed between this frame and the last one, in seconds.
        The render frames, not update.
        */
        template<typename TrequestedPrecision = float>
        TrequestedPrecision getRenderDeltaTime() const
        {
            auto deltaTime =
                static_cast<Tprecision>(std::chrono::duration_cast<std::chrono::microseconds>(m_thisRenderTime - m_lastRenderTime).count()) /
                static_cast<Tprecision>(1000000.0);

            return static_cast<TrequestedPrecision>(deltaTime);
        }

        /**
        @return the total time elapsed since the start of the game, in seconds.
        */
        template<typename TrequestedPrecision = float>
        TrequestedPrecision getTotalElapsed() const
        {
            return static_cast<TrequestedPrecision>(m_totalElapsed);
        }

        /*!
        @return Frame rate. Updated each seconds
        */
        int getFPS() const
        {
            return m_fps;
        }

    private:
        Tprecision                                          m_deltaTime = static_cast<Tprecision>(1.0) / static_cast<Tprecision>(TframePerSecond);
        Tprecision                                          m_totalElapsed = static_cast<Tprecision>(0.0);
        decltype(std::chrono::high_resolution_clock::now()) m_startTime = std::chrono::high_resolution_clock::now();
        decltype(std::chrono::high_resolution_clock::now()) m_lastUpdateTime = std::chrono::high_resolution_clock::now();
        std::chrono::high_resolution_clock::duration        m_currentFrameProgress = std::chrono::seconds(0);
        std::chrono::high_resolution_clock::duration        m_timePerFrame = std::chrono::microseconds(1000000) / TframePerSecond;
        int                                                 m_currentFPS = 0;
        int                                                 m_fps = 0;
        decltype(std::chrono::high_resolution_clock::now()) m_lastFPSSnapShotTime = std::chrono::high_resolution_clock::now();
        decltype(std::chrono::high_resolution_clock::now()) m_lastRenderTime = std::chrono::high_resolution_clock::now();
        decltype(std::chrono::high_resolution_clock::now()) m_thisRenderTime = std::chrono::high_resolution_clock::now();
    };
}
