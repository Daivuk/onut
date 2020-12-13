#ifndef TIMING_H_INCLUDED
#define TIMING_H_INCLUDED

// STL
#include <chrono>
#include <cinttypes>
#include <string>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Timing)

using namespace std::chrono_literals;

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

    template<typename T>
    std::string secondsToString(const T& duration, bool show_minutes = false, bool show_hours = false)
    {
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

        std::string ret;

        if (seconds > 60 * 60 || show_hours)
        {
            auto hours = seconds / 60 / 60;
            ret += to_string(hours) + ":";
        }

        if (seconds > 60 || show_minutes)
        {
            auto minutes = seconds / 60;
            if (show_hours)
            {
                minutes %= 60;
                if (minutes < 10)
                {
                    ret += "0";
                }
                ret += to_string(minutes % 60) + ":";
            }
            else
            {
                ret += to_string(minutes) + ":";
            }
        }

        seconds %= 60;
        if (show_minutes)
        {
            seconds %= 60;
            if (seconds < 10)
            {
                ret += "0";
            }
            ret += to_string(seconds % 60);
        }
        else
        {
            ret += to_string(seconds);
        }

        return std::move(ret);
    }
}

extern OTimingRef oTiming;

#define OGetDeltaTime oTiming->getDeltaTime
#define ODT OGetDeltaTime()

#define ONow std::chrono::high_resolution_clock::now()
#define OTime decltype(std::chrono::high_resolution_clock::now())

#define ONanoseconds std::chrono::nanoseconds
#define OMicroseconds std::chrono::microseconds
#define OMilliseconds std::chrono::milliseconds
#define OSeconds std::chrono::seconds
#define OMinutes std::chrono::minutes
#define OHours std::chrono::hours

#define OToNanoseconds(__duration__) std::chrono::duration_cast<ONanoseconds>(__duration__)
#define OToMicroseconds(__duration__) std::chrono::duration_cast<OMicroseconds>(__duration__)
#define OToMilliseconds(__duration__) std::chrono::duration_cast<OMilliseconds>(__duration__)
#define OToSeconds(__duration__) std::chrono::duration_cast<OSeconds>(__duration__)
#define OToMinutes(__duration__) std::chrono::duration_cast<OMinutes>(__duration__)
#define OToHours(__duration__) std::chrono::duration_cast<OHours>(__duration__)

#endif
