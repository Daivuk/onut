#ifndef AUDIOENGINELINUX_H_INCLUDED
#define AUDIOENGINELINUX_H_INCLUDED

// Onut
#include <onut/AudioEngine.h>

// STL
#include <atomic>
#include <thread>

namespace onut
{
    class AudioEngineLinux final : public AudioEngine
    {
    public:
        AudioEngineLinux();
        ~AudioEngineLinux();

    private:
        void update() override;
        int getSampleRate() const override;
        int getChannels() const override;
    };
};

#endif
