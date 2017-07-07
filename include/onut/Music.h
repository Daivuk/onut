#ifndef MUSIC_H_INCLUDED
#define MUSIC_H_INCLUDED

// Onut
#include <onut/Resource.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ContentManager)
OForwardDeclare(Music)

namespace onut
{
    class Music : public Resource
    {
    public:
        static OMusicRef createFromFile(const std::string& filename, const OContentManagerRef& pContentManager);

        virtual ~Music();

        virtual void play(bool loop = false) = 0;
        virtual void stop() = 0;
        virtual void pause() = 0;
        virtual void resume() = 0;
        virtual void setVolume(float volume) = 0;

        virtual bool isPlaying() const = 0;
        virtual bool isPaused() const = 0;
        virtual bool isDone() = 0;

    protected:
        Music();
    };
}

OMusicRef OGetMusic(const std::string& name);
void OPlayMusic(const std::string& name, bool loop = false);
void OStopMusic(const std::string& name);

#endif
