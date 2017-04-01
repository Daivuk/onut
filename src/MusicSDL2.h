#ifndef MUSICSDL2_H_INCLUDED
#define MUSICSDL2_H_INCLUDED

// Onut
#include <onut/Music.h>
#include <onut/Resource.h>

// Third party
#include <SDL.h>
#include <SDL_mixer.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(MusicSDL2)

namespace onut
{
    class MusicSDL2 final : public Music
    {
    public:
        MusicSDL2();
        ~MusicSDL2();

        void play(bool loop = false) override;
        void stop() override;
        void pause() override;
        void resume() override;
        void setVolume(float volume) override;

        bool isPlaying() const override { return m_isPlaying; }
        bool isDone() override;

    private:
        friend class Music;

        Mix_Music *m_pMusic = nullptr;
        float m_volume = 1.f;
        bool m_isPlaying = false;
        bool m_loop = false;
    };
}

#endif
