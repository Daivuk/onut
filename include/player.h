#pragma once
#include "Texture.h"

#include <memory>
#include <string>
#if defined(WIN32)
#include <Windows.h>
#endif

namespace onut
{
    class Player
    {
    public:
        static Player* Create();

        virtual ~Player() {}

#if defined(WIN32)
        virtual void init(HWND handle) = 0;
#endif
        virtual void init(Texture* pRenderTarget) = 0;
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

using OPlayer = onut::Player;
