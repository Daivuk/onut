#if defined(WIN32)

// Private
#include "AudioEngineWASAPI.h"

// Onut
#include <onut/Log.h>

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
        if (hr != S_OK) OLogE("Failed to IMMDeviceEnumerator");
        assert(hr == S_OK);
        if (hr != S_OK) return;

        hr = m_pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &m_pDevice);
        if (hr != S_OK) OLogE("Failed to GetDefaultAudioEndpoint");
        assert(hr == S_OK);
        if (hr != S_OK) return;

        hr = m_pDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&m_pAudioClient);
        if (hr != S_OK) OLogE("Failed to Activate audio device");
        assert(hr == S_OK);
        if (hr != S_OK) return;

        hr = m_pAudioClient->GetMixFormat(&m_pWaveFormat);
        if (hr != S_OK) OLogE("Failed to get mix format");
        assert(hr == S_OK);
        if (hr != S_OK) return;

        hr = m_pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, 50000, 0, m_pWaveFormat, NULL);
        if (hr != S_OK) OLogE("Failed to init audio client");
        assert(hr == S_OK);
        if (hr != S_OK) return;

        m_pEventHandler = CreateEvent(nullptr, false, false, nullptr);
        if (!m_pEventHandler) OLogE("Failed to create audio event handler");
        assert(m_pEventHandler);
        if (m_pEventHandler == nullptr) return;

        hr = m_pAudioClient->SetEventHandle(m_pEventHandler);
        if (hr != S_OK) OLogE("Failed to set audio event handle");
        assert(hr == S_OK);
        if (hr != S_OK) return;

        hr = m_pAudioClient->GetBufferSize(&bufferFrameCount);
        if (hr != S_OK) OLogE("Failed to get audio client buffer size");
        assert(hr == S_OK);
        if (hr != S_OK) return;

        hr = m_pAudioClient->GetService(IID_IAudioRenderClient, (void**)&m_pRenderClient);
        if (hr != S_OK) OLogE("Failed to get audio render client");
        assert(hr == S_OK);
        if (hr != S_OK) return;

        hr = m_pAudioClient->Start();  // Start playing.
        if (hr != S_OK) OLogE("Failed to start audio");
        assert(hr == S_OK);
        if (hr != S_OK) return;

        m_isRunning = true;
        m_thread = std::thread(std::bind(&AudioEngineWASAPI::threadMain, this));
    }

    AudioEngineWASAPI::~AudioEngineWASAPI()
    {
        {
            std::lock_guard<std::mutex> locker(m_instancesMutex);
            m_instances.clear();
        }
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
                OLogE("Failed to Get Current audio Padding");
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
                    OLogE("Failed to Get audio buffer");
                    m_isRunning = false;
                    return;
                }

                progressInstances(numFramesAvailable, getSampleRate(), m_pWaveFormat->nChannels, (float*)pData);

                hr = m_pRenderClient->ReleaseBuffer(numFramesAvailable, 0);
                assert(hr == S_OK);
                if (hr != S_OK)
                {
                    OLogE("Failed to release audio buffer");
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
