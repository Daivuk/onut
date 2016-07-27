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
                break;
            }
        }
    }

    void AudioEngine::progressInstances(int frameCount, int channelCount, float* pOut)
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

            auto pSoundPtr = pInstancePtr->m_pSound.get();
            int offset = pInstancePtr->m_offset;
            auto pSoundBuffer = pSoundPtr->m_pBuffer + offset * pSoundPtr->m_channelCount;
            float volume = pInstancePtr->m_volume;
            int bufferSampleCount = pSoundPtr->m_bufferSampleCount;
            bool loop = pInstancePtr->m_loop;
            float balance = pInstancePtr->m_balance;
            float leftVolume = std::min(1.0f, -balance + 1.0f) * volume;
            float rightVolume = std::min(1.0f, balance + 1.0f) * volume;
            float pitch = pInstancePtr->m_pitch;
            int fi;
            int len = std::min(frameCount, (int)((float)(bufferSampleCount - offset) / pitch));
            switch (channelCount)
            {
                case 1:
                {
                    switch (pSoundPtr->m_channelCount)
                    {
                        case 1:
                        {
                            for (int i = 0; i < len; ++i)
                            {
                                fi = (int)((float)i * pitch);
                                pOut[i] += pSoundBuffer[fi] * volume;
                            }
                            break;
                        }
                        case 2:
                        {
                            for (int i = 0; i < len; ++i)
                            {
                                fi = (int)((float)i * pitch);
                                pOut[i] += (pSoundBuffer[fi * 2 + 0] + pSoundBuffer[fi * 2 + 1]) * 0.5f * volume;
                            }
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
                            for (int i = 0; i < len; ++i)
                            {
                                fi = (int)((float)i * pitch);
                                pOut[i * 2 + 0] += pSoundBuffer[fi] * leftVolume;
                                pOut[i * 2 + 1] += pSoundBuffer[fi] * rightVolume;
                            }
                            break;
                        }
                        case 2:
                        {
                            for (int i = 0; i < len * 2; i += 2)
                            {
                                fi = (int)((float)i * pitch);
                                pOut[i + 0] += pSoundBuffer[fi + 0] * leftVolume;
                                pOut[i + 1] += pSoundBuffer[fi + 0] * rightVolume;
                            }
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

            offset += (int)((float)len * pitch);
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
