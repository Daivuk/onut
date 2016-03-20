#pragma once
#include "onut/TimeInfo.h"

OTimeInfoRef oTimeInfo;

namespace onut
{
    static const int MAX_UPDATE_PER_FRAME = 4;

    OTimeInfoRef TimeInfo::create(uint32_t updateFps)
    {
        return OMake<TimeInfo>(updateFps);
    }

    TimeInfo::TimeInfo(uint32_t updateFps)
    {
        setUpdateFps(updateFps);
    }

    void TimeInfo::setUpdateFps(uint32_t updateFps)
    {
        m_updateFps = updateFps;
        m_deltaTime = 1.0 / static_cast<double>(updateFps);
        m_timePerFrame = std::chrono::microseconds(1000000) / updateFps;
    }

    int TimeInfo::update(bool fixedStep)
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

    void TimeInfo::render()
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

    float TimeInfo::getDeltaTime() const
    {
        return static_cast<float>(m_deltaTime);
    }

    float TimeInfo::getRenderDeltaTime() const
    {
        auto deltaTime =
            static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(m_thisRenderTime - m_lastRenderTime).count()) /
            1000000.0;

        return static_cast<float>(deltaTime);
    }

    float TimeInfo::getTotalElapsed() const
    {
        return static_cast<float>(m_totalElapsed);
    }

    int TimeInfo::getFPS() const
    {
        return m_fps;
    }
}
