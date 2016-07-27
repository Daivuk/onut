#pragma once
// STL
#include <condition_variable>
#include <mutex>
#include <vector>

// Forward declaration
#include <onut/ForwardDeclaration.h>
OForwardDeclare(AudioEngine);
OForwardDeclare(SoundInstance);

namespace onut
{
    class AudioEngine
    {
    public:
        static OAudioEngineRef create();

        virtual ~AudioEngine();

        virtual void update() = 0;
        virtual int getSampleRate() const = 0;

        void addInstance(const OSoundInstanceRef& pInstance);
        void removeInstance(const OSoundInstanceRef& pInstance);

    protected:
        AudioEngine();

        using Instances = std::vector<OSoundInstanceWeak>;

        void progressInstances(int frameCount, int channelCount, float* pOut);

        std::mutex m_instancesMutex;
        Instances m_instances;
    };
};

extern OAudioEngineRef oAudioEngine;
