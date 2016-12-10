// Onut
#include <onut/AudioEngine.h>
#include <onut/AudioStream.h>

// STL
#include <algorithm>
#include <memory.h>

OAudioEngineRef oAudioEngine;

namespace onut
{
    AudioEngine::~AudioEngine()
    {
    }

    AudioEngine::AudioEngine()
    {
    }

    void AudioEngine::addInstance(const OAudioStreamRef& pInstance)
    {
        std::lock_guard<std::mutex> locker(m_instancesMutex);
        m_instances.push_back(pInstance);
    }

    void AudioEngine::removeInstance(const OAudioStreamRef& pInstance)
    {
        auto pInstanceRaw = pInstance.get();
        std::lock_guard<std::mutex> locker(m_instancesMutex);
        for (auto it = m_instances.begin(); it != m_instances.end(); ++it)
        {
            if (it->lock().get() == pInstanceRaw)
            {
                m_instances.erase(it);
                break;
            }
        }
    }

    void AudioEngine::progressInstances(int frameCount, int sampleRate, int channelCount, float* pOut)
    {
        std::unique_lock<std::mutex> locker(m_instancesMutex);
        memset(pOut, 0, sizeof(float) * frameCount * channelCount);
        for (auto it = m_instances.begin(); it != m_instances.end();)
        {
            auto pInstance = it->lock();
            auto pInstancePtr = pInstance.get();
            if (!pInstancePtr)
            {
                it = m_instances.erase(it);
                continue;
            }

            if (!pInstancePtr->progress(frameCount, sampleRate, channelCount, pOut))
            {
                it = m_instances.erase(it);
                continue;
            }

            ++it;
        }
    }
};
