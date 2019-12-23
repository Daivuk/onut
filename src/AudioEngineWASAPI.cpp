#if defined(WIN32)

// Private
#include "AudioEngineWASAPI.h"

// STL
#include <cassert>
#include <chrono>
#include <functional>

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

        hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&m_pEnumerator);
        assert(hr == S_OK);
        if (hr != S_OK) return;

        hr = m_pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &m_pDevice);
        assert(hr == S_OK);
        if (hr != S_OK) return;

        hr = m_pDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&m_pAudioClient);
        assert(hr == S_OK);
        if (hr != S_OK) return;

        hr = m_pAudioClient->GetMixFormat(&m_pWaveFormat);
        assert(hr == S_OK);
        if (hr != S_OK) return;

        hr = m_pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, 50000, 0, m_pWaveFormat, NULL);
        assert(hr == S_OK);
        if (hr != S_OK) return;

        m_pEventHandler = CreateEvent(nullptr, false, false, nullptr);
        assert(m_pEventHandler);
        if (m_pEventHandler == nullptr) return;

        hr = m_pAudioClient->SetEventHandle(m_pEventHandler);
        assert(hr == S_OK);
        if (hr != S_OK) return;

        hr = m_pAudioClient->GetBufferSize(&bufferFrameCount);
        assert(hr == S_OK);
        if (hr != S_OK) return;

        hr = m_pAudioClient->GetService(IID_IAudioRenderClient, (void**)&m_pRenderClient);
        assert(hr == S_OK);
        if (hr != S_OK) return;

        hr = m_pAudioClient->Start();  // Start playing.
        assert(hr == S_OK);
        if (hr != S_OK) return;

        m_isRunning = true;
        m_thread = std::thread(std::bind(&AudioEngineWASAPI::threadMain, this));
    }

    AudioEngineWASAPI::~AudioEngineWASAPI()
    {
        m_isRunning = false;
        if (m_thread.joinable()) m_thread.join();

        CoTaskMemFree(m_pWaveFormat);
        if (m_pEnumerator) m_pEnumerator->Release();
        if (m_pDevice) m_pDevice->Release();
        if (m_pAudioClient) m_pAudioClient->Release();
        if (m_pRenderClient) m_pRenderClient->Release();
    }

    void AudioEngineWASAPI::threadMain()
    {
        HRESULT hr;
        UINT32 numFramesAvailable;
        UINT32 numFramesPadding;
        BYTE *pData;
        DWORD retval;

        while (m_isRunning)
        {
            retval = WaitForSingleObject(m_pEventHandler, 2000);
            if (retval != WAIT_OBJECT_0)
            {
                // Event handle timed out after a 2-second wait.
                m_pAudioClient->Stop();
                break;
            }

            // See how much buffer space is available.
            hr = m_pAudioClient->GetCurrentPadding(&numFramesPadding);
            assert(hr == S_OK);
            if (hr != S_OK)
            {
                m_isRunning = false;
                return;
            }

            numFramesAvailable = bufferFrameCount - numFramesPadding;
            if (numFramesAvailable > 0)
            {
                // Grab all the available space in the shared buffer.
                hr = m_pRenderClient->GetBuffer(numFramesAvailable, &pData);
                assert(hr == S_OK);
                if (hr != S_OK)
                {
                    m_isRunning = false;
                    return;
                }

                progressInstances(numFramesAvailable, getSampleRate(), m_pWaveFormat->nChannels, (float*)pData);

                hr = m_pRenderClient->ReleaseBuffer(numFramesAvailable, 0);
                assert(hr == S_OK);
                if (hr != S_OK)
                {
                    m_isRunning = false;
                    return;
                }
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

    int AudioEngineWASAPI::getChannels() const
    {
        return static_cast<int>(m_pWaveFormat->nChannels);
    }
};

#endif
