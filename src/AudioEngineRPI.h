#ifndef AUDIOENGINERPI_H_INCLUDED
#define AUDIOENGINERPI_H_INCLUDED

// Onut
#include <onut/AudioEngine.h>

// Third party
#include "audioplay/audioplay.h"

// STL
#include <atomic>
#include <thread>

namespace onut
{
    class AudioEngineRPI final : public AudioEngine
    {
    public:
        AudioEngineRPI();
        ~AudioEngineRPI();

    private:
        void update() override;
        int getSampleRate() const override;
        int getChannels() const override;
        void threadMain();
        
        AUDIOPLAY_STATE_T* m_pHandle;
        int m_sampleRate = 44100;
        int m_channelCount = 2;
        int m_bufferCount = 3;
        int m_bitDepth = 16;
        int m_frameCount = 984;
        int m_minLatency = 0;
        int m_bufferSize = m_frameCount * m_channelCount * (m_bitDepth / 8);
        float* m_pFloatBuffer;
        
        std::atomic<bool> m_isRunning;
        std::thread m_thread;
    };
};

#endif
