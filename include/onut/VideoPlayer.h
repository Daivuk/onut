#pragma once
// Third party
#if defined(WIN32)
#include <Windows.h>
#endif

// STL
#include <string>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Texture);
OForwardDeclare(VideoPlayer);

namespace onut
{
    class VideoPlayer
    {
    public:
#if defined(WIN32)
        static OVideoPlayerRef createWithWindowHandle(HWND handle);
#endif
        static OVideoPlayerRef createWithRenderTarget(const OTextureRef& pRenderTarget);

        virtual ~VideoPlayer();

        virtual bool isPlaying() const = 0;
        virtual void pause() = 0;
        virtual void play() = 0;
        virtual void setLoop(bool bLoop) = 0;
        virtual void setPlayRate(double playRate) = 0;
        virtual void setSource(const std::string& url) = 0;
        virtual void setVolume(float volume) = 0;
        virtual void togglePlayPause() = 0;
        virtual void update() = 0;
    };
}
