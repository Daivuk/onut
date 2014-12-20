#include "TimeInfo.h"

#define MAX_UPDATE_PER_FRAME 4

namespace onut {
    int TimeInfo::update() {
        int framesToUpdate = 0;

        // Get cur time
        auto currentTime = std::chrono::steady_clock::now();

        // Deltas
        auto updateElapsed = currentTime - m_lastUpdateTime;
        m_lastUpdateTime = currentTime;
        auto totalElapsed = currentTime - m_startTime;
        auto totalElapsedMS = std::chrono::duration_cast<std::chrono::milliseconds>(totalElapsed);
        m_totalElapsed = static_cast<double>(totalElapsedMS.count()) / 1000.0;

        // Progress current frame
        m_currentFrameProgress += updateElapsed;
        while (m_currentFrameProgress > m_timePerFrame) {
            ++framesToUpdate;
            if (framesToUpdate > MAX_UPDATE_PER_FRAME) framesToUpdate = MAX_UPDATE_PER_FRAME;
            m_currentFrameProgress -= m_timePerFrame;
        }

        return framesToUpdate;
    }

    float TimeInfo::getDeltaTime() const {
        return static_cast<float>(m_deltaTime);
    }

    float TimeInfo::getTotalElapsed() const {
        return static_cast<float>(m_totalElapsed);
    }
}
