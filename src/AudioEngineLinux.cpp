// Private
#include "AudioEngineLinux.h"

// Onut
#include <onut/Log.h>

// STL
#include <cassert>
#include <chrono>

namespace onut
{
    OAudioEngineRef AudioEngine::create()
    {
        return std::shared_ptr<AudioEngine>(new AudioEngineLinux());
    }

    AudioEngineLinux::AudioEngineLinux()
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
            m_thread = std::thread(std::bind(&AudioEngineLinux::threadMain, this));
        }
    }

    AudioEngineLinux::~AudioEngineLinux()
    {
        if (m_pHandle)
        {
            m_isRunning = false;
            if (m_thread.joinable()) m_thread.join();
            audioplay_delete(m_pHandle);
        }
    }

    void AudioEngineLinux::threadMain()
    {
        uint8_t* pBuffer;
        
        while (m_isRunning)
        {
            pBuffer = audioplay_get_buffer(m_pHandle);
            
            if (pBuffer)
            {
                progressInstances(m_frameCount, getSampleRate(), m_channelCount, (float*)pBuffer);
                audioplay_play_buffer(m_pHandle, pBuffer, m_bufferSize);
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
        }
    }

    void AudioEngineLinux::update()
    {
    }

    int AudioEngineLinux::getSampleRate() const
    {
        return m_sampleRate;
    }

    int AudioEngineLinux::getChannels() const
    {
        return m_channelCount;
    }
};
