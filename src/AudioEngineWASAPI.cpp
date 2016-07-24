#if defined(WIN32)

// Private
#include "AudioEngineWASAPI.h"

// STL
#include <cassert>
#include <chrono>

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

namespace onut
{
    OAudioEngineRef AudioEngine::create()
    {
        return std::shared_ptr<AudioEngine>(new AudioEngineWASAPI());
    }

    AudioEngineWASAPI::AudioEngineWASAPI()
    {
        HRESULT hr;

        hr = CoCreateInstance(
            CLSID_MMDeviceEnumerator, NULL,
            CLSCTX_ALL, IID_IMMDeviceEnumerator,
            (void**)&m_pEnumerator);
        assert(hr == S_OK);

        hr = m_pEnumerator->GetDefaultAudioEndpoint(
            eRender, eConsole, &m_pDevice);
        assert(hr == S_OK);

        hr = m_pDevice->Activate(
            IID_IAudioClient, CLSCTX_ALL,
            NULL, (void**)&m_pAudioClient);
        assert(hr == S_OK);

        hr = m_pAudioClient->GetMixFormat(&m_pWaveFormat);
        assert(hr == S_OK);

        hr = m_pAudioClient->Initialize(
            AUDCLNT_SHAREMODE_SHARED,
            0,
            hnsRequestedDuration,
            0,
            m_pWaveFormat,
            NULL);
        assert(hr == S_OK);

        hr = m_pAudioClient->GetBufferSize(&bufferFrameCount);
        assert(hr == S_OK);

        hr = m_pAudioClient->GetService(
            IID_IAudioRenderClient,
            (void**)&m_pRenderClient);
        assert(hr == S_OK);

        hnsActualDuration = static_cast<REFERENCE_TIME>(
            static_cast<double>(REFTIMES_PER_SEC) * 
            static_cast<double>(bufferFrameCount) / 
            static_cast<double>(m_pWaveFormat->nSamplesPerSec));

        hr = m_pAudioClient->Start();  // Start playing.
        assert(hr == S_OK);

        m_isRunning = true;
        m_thread = std::thread(std::bind(&AudioEngineWASAPI::threadMain, this));
    }

    AudioEngineWASAPI::~AudioEngineWASAPI()
    {
        m_isRunning = false;
        m_instancesWait.notify_all();
        if (m_thread.joinable()) m_thread.join();

        CoTaskMemFree(m_pWaveFormat);
        if (m_pEnumerator) m_pEnumerator->Release();
        if (m_pDevice) m_pDevice->Release();
        if (m_pAudioClient) m_pAudioClient->Release();
        if (m_pRenderClient) m_pRenderClient->Release();
    }

    void AudioEngineWASAPI::threadMain()
    {
        float angle = 0;
        while (m_isRunning)
        {
            HRESULT hr;
            UINT32 numFramesAvailable;
            UINT32 numFramesPadding;
            BYTE *pData;

            // See how much buffer space is available.
            hr = m_pAudioClient->GetCurrentPadding(&numFramesPadding);
            assert(hr == S_OK);

            numFramesAvailable = bufferFrameCount - numFramesPadding;
            if (numFramesPadding < SAMPLES_PER_FRAME)
            {
                // Grab all the available space in the shared buffer.
                hr = m_pRenderClient->GetBuffer(SAMPLES_PER_FRAME, &pData);
                assert(hr == S_OK);

                progressInstances(SAMPLES_PER_FRAME, m_pWaveFormat->nChannels, (float*)pData);

                hr = m_pRenderClient->ReleaseBuffer(SAMPLES_PER_FRAME, 0);
                assert(hr == S_OK);
            }
        }
    }

    void AudioEngineWASAPI::update()
    {
    }

    int AudioEngineWASAPI::getSampleRate() const
    {
        return static_cast<int>(m_pWaveFormat->nSamplesPerSec);
    }
};

#endif
