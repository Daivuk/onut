#ifndef AUDIOENGINELINUX_H_INCLUDED
#define AUDIOENGINELINUX_H_INCLUDED

// Onut
#include <onut/AudioEngine.h>

// Third party
#include "audioplay/audioplay.h"

// STL
#include <atomic>
#include <thread>

namespace onut
{
    class AudioEngineLinux final : public AudioEngine
    {
    public:
        AudioEngineLinux();
        ~AudioEngineLinux();

    private:
        void update() override;
        int getSampleRate() const override;
        int getChannels() const override;
        void threadMain();
        
        AUDIOPLAY_STATE_T* m_pHandle;
        int m_sampleRate = 44100;
        int m_channelCount = 2;
        int m_bufferCount = 3;
        int m_bitDepth = 32;
        int m_frameCount = 500;
        int m_bufferSize = m_frameCount * m_channelCount * (m_bitDepth / 8);
        
        std::atomic<bool> m_isRunning;
        std::thread m_thread;
    };
};

#endif
