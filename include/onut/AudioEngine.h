#ifndef AUDIOENGINE_H_INCLUDED
#define AUDIOENGINE_H_INCLUDED

// Onut
#include <onut/Vector3.h>

// STL
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <vector>

// Forward declaration
#include <onut/ForwardDeclaration.h>
OForwardDeclare(AudioEngine);
OForwardDeclare(AudioStream);

namespace onut
{
    class AudioEngine
    {
    public:
        static OAudioEngineRef create();

        virtual ~AudioEngine();

        virtual void update() = 0;
        virtual int getSampleRate() const = 0;
        virtual int getChannels() const = 0;
        float getMasterVolume() const;
        float getSfxVolume() const;
        float getMusicVolume() const;
        void setMasterVolume(float volume);
        void setSfxVolume(float volume);
        void setMusicVolume(float volume);

        void addInstance(const OAudioStreamRef& pInstance);
        void removeInstance(const OAudioStreamRef& pInstance);
        void set3DListener(const Vector3& position, const Vector3& front, const Vector3& up);

    protected:
        AudioEngine();

        using Instances = std::vector<OAudioStreamWeak>;

        void progressInstances(int frameCount, int sampleRate, int channelCount, float* pOut);

        std::mutex m_instancesMutex;
        Instances m_instances;
        Vector3 m_3dPos = {0, 0, 0};
        Vector3 m_3dRight = {1, 0, 0};
        Vector3 m_3dFront = {0, 1, 0};
        Vector3 m_3dUp = {0, 0, 1};
        std::atomic<float> m_masterVolume;
        std::atomic<float> m_sfxVolume;
        std::atomic<float> m_musicVolume;
    };
};

extern OAudioEngineRef oAudioEngine;

#endif
