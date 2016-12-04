#ifndef AUDIOENGINEWASAPI_H_INCLUDED
#define AUDIOENGINEWASAPI_H_INCLUDED

#if defined(WIN32)

// Onut
#include <onut/AudioEngine.h>

// Platform
#include <Audioclient.h>
#include <Mmdeviceapi.h>

// STL
#include <atomic>
#include <thread>

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
        int getChannels() const override;
        void threadMain();

        UINT32 bufferFrameCount = 0;

        WAVEFORMATEX* m_pWaveFormat = nullptr;
        IMMDeviceEnumerator* m_pEnumerator = nullptr;
        IMMDevice* m_pDevice = nullptr;
        IAudioClient* m_pAudioClient = nullptr;
        HANDLE m_pEventHandler = nullptr;
        IAudioRenderClient* m_pRenderClient = nullptr;
        std::atomic<bool> m_isRunning = false;
        std::thread m_thread;
    };
};

#endif

#endif
