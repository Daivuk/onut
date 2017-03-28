// Private
#include "AudioEngineCoreAudio.h"

namespace onut
{
    OAudioEngineRef AudioEngine::create()
    {
        return std::shared_ptr<AudioEngine>(new AudioEngineCoreAudio());
    }

    AudioEngineCoreAudio::AudioEngineCoreAudio()
    {
    }

    AudioEngineCoreAudio::~AudioEngineCoreAudio()
    {
    }

    void AudioEngineCoreAudio::threadMain()
    {
    }

    void AudioEngineCoreAudio::update()
    {
    }

    int AudioEngineCoreAudio::getSampleRate() const
    {
        return 44100;
    }

    int AudioEngineCoreAudio::getChannels() const
    {
        return 2;
    }
};
