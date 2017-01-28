#ifndef MUSIC_H_INCLUDED
#define MUSIC_H_INCLUDED

// Onut
#include <onut/Resource.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ContentManager)
OForwardDeclare(Music)
#if defined(WIN32)
class Mp3;
#endif

namespace onut
{
    class Music final : public Resource
    {
    public:
        static OMusicRef createFromFile(const std::string& filename, const OContentManagerRef& pContentManager);

        void play(bool loop = false);
        void stop();
        void pause();
        void resume();
        void setVolume(float volume);

        bool isPlaying() const { return m_isPlaying; }
        bool isDone();

    private:
        Music();

#if defined(WIN32)
        std::shared_ptr<Mp3> m_pMp3;
#endif
        float m_volume = 1.f;
        bool m_isPlaying = false;
        bool m_loop = false;
    };
}

OMusicRef OGetMusic(const std::string& name);
void OPlayMusic(const std::string& name, bool loop = false);
void OStopMusic(const std::string& name);

#endif
