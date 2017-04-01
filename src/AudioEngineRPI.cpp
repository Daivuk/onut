// Private
#include "AudioEngineRPI.h"

// Onut
#include <onut/Log.h>

// STL
#include <cassert>
#include <chrono>
#include <limits>

namespace onut
{
    OAudioEngineRef AudioEngine::create()
    {
        return std::shared_ptr<AudioEngine>(new AudioEngineRPI());
    }

    AudioEngineRPI::AudioEngineRPI()
    {
        m_isRunning = false;
        auto ret = audioplay_create(&m_pHandle, m_sampleRate, m_channelCount, m_bitDepth, m_bufferCount, m_bufferSize);
        if (ret == -1)
        {
            OLog("Failed to create AudioEngine");
            m_pHandle = nullptr;
        }
        else
        {
            m_isRunning = true;
            m_thread = std::thread(std::bind(&AudioEngineRPI::threadMain, this));
        }
        m_pFloatBuffer = new float[m_frameCount * m_channelCount];
    }

    AudioEngineRPI::~AudioEngineRPI()
    {
        if (m_pHandle)
        {
            m_isRunning = false;
            if (m_thread.joinable()) m_thread.join();
            audioplay_delete(m_pHandle);
        }
        delete [] m_pFloatBuffer;
    }

    void AudioEngineRPI::threadMain()
    {
        uint8_t* pBuffer;
        
        while (m_isRunning)
        {
            pBuffer = audioplay_get_buffer(m_pHandle);
            
            if (pBuffer)
            {
                while (1)
                {
                    auto latency = audioplay_get_latency(m_pHandle);
                    if (latency < (getSampleRate() * (m_minLatency + 10)) / 1000) break;
                    std::this_thread::sleep_for(std::chrono::microseconds(10));
                }

                progressInstances(m_frameCount, getSampleRate(), m_channelCount, m_pFloatBuffer);
                int16_t* pOut = (int16_t*)pBuffer;
                auto len = m_frameCount * m_channelCount;
                float clamped;
                for (int i = 0; i < len; ++i)
                {
                    clamped = m_pFloatBuffer[i];
                    clamped *= 0.5f;
                    clamped = std::max(-1.0f, std::min(clamped, 1.0f));
                    pOut[i] = (int16_t)(clamped * (float)std::numeric_limits<int16_t>::max());
                }
                audioplay_play_buffer(m_pHandle, pBuffer, m_bufferSize);
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
        }
    }

    void AudioEngineRPI::update()
    {
    }

    int AudioEngineRPI::getSampleRate() const
    {
        return m_sampleRate;
    }

    int AudioEngineRPI::getChannels() const
    {
        return m_channelCount;
    }
};
