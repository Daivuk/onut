// Onut
#include <onut/Sound.h>

// Private
#include "AudioEngine.h"

// STL
#include <algorithm>

OAudioEngineRef oAudioEngine;

namespace onut
{
    AudioEngine::~AudioEngine()
    {
    }

    AudioEngine::AudioEngine()
    {
    }

    void AudioEngine::addInstance(const OSoundInstanceRef& pInstance)
    {
        std::lock_guard<std::mutex> locker(m_instancesMutex);
        m_instances.push_back(pInstance);
        m_instancesWait.notify_one();
    }

    void AudioEngine::removeInstance(const OSoundInstanceRef& pInstance)
    {
        auto pInstanceRaw = pInstance.get();
        std::lock_guard<std::mutex> locker(m_instancesMutex);
        for (auto it = m_instances.begin(); it != m_instances.end(); ++it)
        {
            if (it->lock().get() == pInstanceRaw)
            {
                m_instances.erase(it);
                m_instancesWait.notify_one();
                break;
            }
        }
    }

    void AudioEngine::progressInstances(int frameCount, int channelCount, float* pOut)
    {
        std::unique_lock<std::mutex> locker(m_instancesMutex);
        if (m_instances.empty())
        {
            m_instancesWait.wait(locker);
        }
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

            auto pSoundPtr = pInstancePtr->m_pSound.get();
            int offset = pInstancePtr->m_offset;
            auto pSoundBuffer = pSoundPtr->m_pBuffer + offset * pSoundPtr->m_channelCount;
            float volume = pInstancePtr->m_volume;
            int bufferSampleCount = pSoundPtr->m_bufferSampleCount;
            bool loop = pInstancePtr->m_loop;
            float balance = pInstancePtr->m_balance;
            switch (channelCount)
            {
                case 1:
                {
                    switch (pSoundPtr->m_channelCount)
                    {
                        case 1:
                        {
                            int len = std::min(frameCount, bufferSampleCount - offset);
                            for (int i = 0; i < len; ++i)
                            {
                                pOut[i] += pSoundBuffer[i] * volume;
                            }
                            offset += len;
                            break;
                        }
                        case 2:
                        {
                            int len = std::min(frameCount, bufferSampleCount - offset);
                            for (int i = 0; i < len; ++i)
                            {
                                pOut[i] += (pSoundBuffer[i * 2 + 0] + pSoundBuffer[i * 2 + 1]) * 0.5f * volume;
                            }
                            offset += len;
                            break;
                        }
                        default:
                            assert(false);
                    }
                    break;
                }
                case 2:
                {
                    switch (pSoundPtr->m_channelCount)
                    {
                        case 1:
                        {
                            int len = std::min(frameCount, bufferSampleCount - offset);
                            for (int i = 0; i < len; ++i)
                            {
                                pOut[i * 2 + 0] += pSoundBuffer[i] * volume;
                                pOut[i * 2 + 1] += pSoundBuffer[i] * volume;
                            }
                            offset += len;
                            break;
                        }
                        case 2:
                        {
                            int len = std::min(frameCount, bufferSampleCount - offset) * 2;
                            for (int i = 0; i < len; ++i)
                            {
                                pOut[i] += pSoundBuffer[i] * volume;
                            }
                            offset += len / 2;
                            break;
                        }
                        default:
                            assert(false);
                    }
                    break;
                }
                default:
                    assert(false);
            }

            pInstancePtr->m_offset = offset;
            if (offset >= pSoundPtr->m_bufferSampleCount)
            {
                if (loop)
                {
                    pInstancePtr->m_offset = 0;
                }
                else
                {
                    it = m_instances.erase(it);
                    continue;
                }
            }

            ++it;
        }
    }
};
