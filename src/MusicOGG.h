#ifndef MUSICRPI_H_INCLUDED
#define MUSICRPI_H_INCLUDED

// Onut
#include <onut/Music.h>
#include <onut/Resource.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(MusicRPI)

namespace onut
{
    class MusicRPI final : public Music
    {
    public:
        MusicRPI();
        ~MusicRPI();
        
        void play(bool loop = false) override;
        void stop() override;
        void pause() override;
        void resume() override;
        void setVolume(float volume) override;

        bool isPlaying() const override { return m_isPlaying; }
        bool isDone() override;

    private:
        friend class Music;

        float m_volume = 1.f;
        bool m_isPlaying = false;
        bool m_loop = false;
    };
}

#endif
