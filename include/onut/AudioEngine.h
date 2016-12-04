#ifndef AUDIOENGINE_H_INCLUDED
#define AUDIOENGINE_H_INCLUDED

// STL
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

        void addInstance(const OAudioStreamRef& pInstance);
        void removeInstance(const OAudioStreamRef& pInstance);

    protected:
        AudioEngine();

        using Instances = std::vector<OAudioStreamWeak>;

        void progressInstances(int frameCount, int sampleRate, int channelCount, float* pOut);

        std::mutex m_instancesMutex;
        Instances m_instances;
    };
};

extern OAudioEngineRef oAudioEngine;

#endif
