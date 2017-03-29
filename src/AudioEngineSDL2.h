#ifndef AUDIOENGINESDL2_H_INCLUDED
#define AUDIOENGINESDL2_H_INCLUDED

// Onut
#include <onut/AudioEngine.h>

// STL
#include <atomic>
#include <thread>

// Third parties
#include <SDL.h>

namespace onut
{
    class AudioEngineSDL2 final : public AudioEngine
    {
    public:
        AudioEngineSDL2();
        ~AudioEngineSDL2();

        void audioCallback(Uint8 *stream, int len);

    private:
        void update() override;
        int getSampleRate() const override;
        int getChannels() const override;

        SDL_AudioSpec m_audioSpec;
    };
};

#endif
