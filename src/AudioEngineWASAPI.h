#pragma once
#if defined(WIN32)

// Platform
#include <Audioclient.h>
#include <Mmdeviceapi.h>

// Private
#include "AudioEngine.h"

// STL
#include <atomic>
#include <thread>

// REFERENCE_TIME time units per second and per millisecond
#define REFTIMES_PER_SEC 10000000
#define REFTIMES_PER_MILLISEC 10000
#define SAMPLES_PER_FRAME 512

namespace onut
{
    class AudioEngineWASAPI final : public AudioEngine
    {
    public:
        AudioEngineWASAPI();
        ~AudioEngineWASAPI();

    private:
        void update() override;
        int getSampleRate() const override;
        void threadMain();

        REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
        UINT32 bufferFrameCount = 0;
        REFERENCE_TIME hnsActualDuration = 0;

        WAVEFORMATEX* m_pWaveFormat = nullptr;
        IMMDeviceEnumerator* m_pEnumerator = nullptr;
        IMMDevice* m_pDevice = nullptr;
        IAudioClient* m_pAudioClient = nullptr;
        IAudioRenderClient* m_pRenderClient = nullptr;
        std::atomic<bool> m_isRunning = false;
        std::thread m_thread;
    };
};

#endif
