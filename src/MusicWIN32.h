#ifndef MUSICWIN32_H_INCLUDED
#define MUSICWIN32_H_INCLUDED

// Onut
#include <onut/Music.h>
#include <onut/Resource.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(MusicWIN32)
class Mp3;

namespace onut
{
    class MusicWin32 final : public Music
    {
    public:
        MusicWin32();

        void play(bool loop = false) override;
        void stop() override;
        void pause() override;
        void resume() override;
        void setVolume(float volume) override;

        bool isPlaying() const override { return m_isPlaying; }
        bool isDone() override;

    private:
        friend class Music;

        std::shared_ptr<Mp3> m_pMp3;
        float m_volume = 1.f;
        bool m_isPlaying = false;
        bool m_loop = false;
    };
}

#endif
