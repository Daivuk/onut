#pragma once
// Onut
#include <onut/Timing.h>

OTimingRef oTiming;

namespace onut
{
    static const int MAX_UPDATE_PER_FRAME = 4;

    OTimingRef Timing::create(uint32_t updateFps)
    {
        return OMake<Timing>(updateFps);
    }

    Timing::Timing(uint32_t updateFps)
    {
        setUpdateFps(updateFps);
    }

    void Timing::setUpdateFps(uint32_t updateFps)
    {
        m_updateFps = updateFps;
        m_deltaTime = 1.0 / static_cast<double>(updateFps);
        m_timePerFrame = std::chrono::microseconds(1000000) / updateFps;
    }

    int Timing::update(bool fixedStep)
    {
        int framesToUpdate = 0;

        // Get cur time
        auto currentTime = std::chrono::high_resolution_clock::now();

        // Deltas
        auto updateElapsed = currentTime - m_lastUpdateTime;
        m_lastUpdateTime = currentTime;
        auto totalElapsed = currentTime - m_startTime;
        auto totalElapsedMS = std::chrono::duration_cast<std::chrono::milliseconds>(totalElapsed);
        m_totalElapsed = static_cast<double>(totalElapsedMS.count()) / 1000.0;

        // Progress current frame
        if (fixedStep)
        {
            m_currentFrameProgress += updateElapsed;
            while (m_currentFrameProgress > m_timePerFrame)
            {
                ++framesToUpdate;
                if (framesToUpdate > MAX_UPDATE_PER_FRAME)
                {
                    framesToUpdate = MAX_UPDATE_PER_FRAME;
                    m_currentFrameProgress = std::chrono::microseconds(0);
                    break;
                }
                m_currentFrameProgress -= m_timePerFrame;
            }

            m_deltaTime = 1.0 / static_cast<double>(m_updateFps);

            return framesToUpdate;
        }
        else
        {
            m_deltaTime = 
                static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(updateElapsed).count()) /
                1000000.0;
            return 1;
        }
    }

    void Timing::render()
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

    float Timing::getDeltaTime() const
    {
        return static_cast<float>(m_deltaTime);
    }

    float Timing::getRenderDeltaTime() const
    {
        auto deltaTime =
            static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(m_thisRenderTime - m_lastRenderTime).count()) /
            1000000.0;

        return static_cast<float>(deltaTime);
    }

    float Timing::getTotalElapsed() const
    {
        return static_cast<float>(m_totalElapsed);
    }

    int Timing::getFPS() const
    {
        return m_fps;
    }
}
