#pragma once
// Onut
#include <chrono>
#include <cinttypes>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Timing)

namespace onut
{
    class Timing final
    {
    public:
        static OTimingRef create(uint32_t updateFps = 120);

        Timing(uint32_t updateFps = 120);

        void setUpdateFps(uint32_t updateFps);

        int update(bool fixedStep = true);
        void render();

        /**
        @return the time elapsed between this frame and the last one, in seconds.
        Perfect for performing animations
        */
        float getDeltaTime() const;

        /**
        @return the time elapsed between this frame and the last one, in seconds.
        The render frames, not update.
        */
        float getRenderDeltaTime() const;

        /**
        @return the total time elapsed since the start of the game, in seconds.
        */
        float getTotalElapsed() const;

        /*!
        @return Frame rate. Updated each seconds
        */
        int getFPS() const;

    private:
        double m_deltaTime = 1.0 / 120.0;
        double m_totalElapsed = 0.0;
        decltype(std::chrono::high_resolution_clock::now()) m_startTime = std::chrono::high_resolution_clock::now();
        decltype(std::chrono::high_resolution_clock::now()) m_lastUpdateTime = std::chrono::high_resolution_clock::now();
        std::chrono::high_resolution_clock::duration m_currentFrameProgress = std::chrono::seconds(0);
        std::chrono::high_resolution_clock::duration m_timePerFrame = std::chrono::microseconds(1000000) / 120;
        int m_currentFPS = 0;
        int m_fps = 0;
        decltype(std::chrono::high_resolution_clock::now()) m_lastFPSSnapShotTime = std::chrono::high_resolution_clock::now();
        decltype(std::chrono::high_resolution_clock::now()) m_lastRenderTime = std::chrono::high_resolution_clock::now();
        decltype(std::chrono::high_resolution_clock::now()) m_thisRenderTime = std::chrono::high_resolution_clock::now();
        uint32_t m_updateFps = 120;
    };
}

extern OTimingRef oTiming;

#define OGetDeltaTime oTiming->getDeltaTime
#define ODT OGetDeltaTime()
