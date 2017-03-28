#ifndef AUDIOENGINECOREAUDIO_H_INCLUDED
#define AUDIOENGINECOREAUDIO_H_INCLUDED

// Onut
#include <onut/AudioEngine.h>

// STL
#include <atomic>
#include <thread>

namespace onut
{
    class AudioEngineCoreAudio final : public AudioEngine
    {
    public:
        AudioEngineCoreAudio();
        ~AudioEngineCoreAudio();

    private:
        void update() override;
        int getSampleRate() const override;
        int getChannels() const override;
        void threadMain();
    };
};

#endif
