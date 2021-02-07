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
        m_masterVolume = 1.0f;
        m_sfxVolume = 1.0f;
        m_musicVolume = 1.0f;
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

    float AudioEngine::getMasterVolume() const
    {
        return m_masterVolume;
    }

    float AudioEngine::getSfxVolume() const
    {
        return m_sfxVolume;
    }

    float AudioEngine::getMusicVolume() const
    {
        return m_musicVolume;
    }

    void AudioEngine::setMasterVolume(float volume)
    {
        m_masterVolume = volume;
    }

    void AudioEngine::setSfxVolume(float volume)
    {
        m_sfxVolume = volume;
    }

    void AudioEngine::setMusicVolume(float volume)
    {
        m_musicVolume = volume;
    }

    void AudioEngine::progressInstances(int frameCount, int sampleRate, int channelCount, float* pOut)
    {
        {
            std::unique_lock<std::mutex> locker(m_instancesMutex);
            auto lPos = m_3dPos;
            auto lFront = m_3dFront;
            auto lRight = m_3dRight;
            auto lUp = m_3dUp;
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

                const auto& positioning = pInstancePtr->m_positioning;
                float volume = 1.0f;
                float balance = 0.0f;
                float pitch = 1.0f;
                if (positioning.enabled)
                {
                    auto dir = positioning.pos - lPos;
                    auto disSqrt = dir.LengthSquared();
                    auto dis = 0.0f;
                    if (disSqrt > 0.0f)
                    {
                        dis = dir.Length();

                        if (dis > positioning.radius)
                        {
                            ++it;
                            continue; // We don't play out of range sounds
                        }

                        dir /= dis;
                    }
                    auto dotR = lRight.Dot(dir);
                    auto dotF = lFront.Dot(dir);

                    volume = std::max<float>(0.0f, 1 - (dis / positioning.radius));
                    volume *= std::min<float>(1.0f, (dotF * 0.2f + 0.8f)); // Slightly volume down on the back
                    balance = dotR;
                    pitch = 1.0f; // TODO: Doppler
                }

                if (!pInstancePtr->progress(frameCount, sampleRate, channelCount, pOut, volume, balance, pitch))
                {
                    pInstancePtr->onStopped();
                    it = m_instances.erase(it);
                    continue;
                }

                ++it;
            }
        }
        float masterVolume = m_masterVolume;
        for (int i = 0, len = frameCount * channelCount; i < len; ++i)
        {
            pOut[i] *= masterVolume;
        }
    }

    void AudioEngine::set3DListener(const Vector3& position, const Vector3& front, const Vector3& up)
    {
        std::lock_guard<std::mutex> locker(m_instancesMutex);
        m_3dPos = position;
        m_3dFront = front;
        m_3dFront.Normalize();
        m_3dRight = front.Cross(up);
        m_3dRight.Normalize();
        m_3dUp = m_3dRight.Cross(m_3dFront);
    }
};
