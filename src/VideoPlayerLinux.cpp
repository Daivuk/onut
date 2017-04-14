// Onut
#include <onut/Texture.h>

// Internal
#include "VideoPlayerLinux.h"

namespace onut
{
    OVideoPlayerRef VideoPlayer::createWithRenderTarget(const OTextureRef& pRenderTarget)
    {
        auto pRet = std::make_shared<VideoPlayerLinux>();
        pRet->m_pRenderTarget = pRenderTarget;
        return pRet;
    }

    VideoPlayerLinux::~VideoPlayerLinux()
    {
    }

    void VideoPlayerLinux::pause()
    {
    }

    void VideoPlayerLinux::play()
    {
    }

    void VideoPlayerLinux::setLoop(bool bLoop)
    {
    }

    void VideoPlayerLinux::togglePlayPause()
    {
    }

    void VideoPlayerLinux::setPlayRate(double playRate)
    {
    }

    void VideoPlayerLinux::setSource(const std::string& url)
    {
    }

    void VideoPlayerLinux::setVolume(float volume)
    {
    }

    bool VideoPlayerLinux::isPlaying() const
    {
        return false;
    }

    void VideoPlayerLinux::update()
    {
    }

    const OTextureRef& VideoPlayerLinux::getRenderTarget()
    {
        return m_pRenderTarget;
    }
}
