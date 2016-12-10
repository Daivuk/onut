// Private
#include "AudioEngineLinux.h"

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
    }

    AudioEngineLinux::~AudioEngineLinux()
    {
    }

    void AudioEngineLinux::update()
    {
    }

    int AudioEngineLinux::getSampleRate() const
    {
        return 44100;
    }

    int AudioEngineLinux::getChannels() const
    {
        return 2;
    }
};
