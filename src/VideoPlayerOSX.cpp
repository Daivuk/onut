// Onut
#include <onut/Texture.h>

// Internal
#include "VideoPlayerOSX.h"

namespace onut
{
    OVideoPlayerRef VideoPlayer::createWithRenderTarget(const OTextureRef& pRenderTarget)
    {
        auto pRet = std::make_shared<VideoPlayerOSX>();
        pRet->m_pRenderTarget = pRenderTarget;
        return pRet;
    }

    VideoPlayerOSX::~VideoPlayerOSX()
    {
    }

    void VideoPlayerOSX::pause()
    {
    }

    void VideoPlayerOSX::play()
    {
    }

    void VideoPlayerOSX::setLoop(bool bLoop)
    {
    }

    void VideoPlayerOSX::togglePlayPause()
    {
    }

    void VideoPlayerOSX::setPlayRate(double playRate)
    {
    }

    void VideoPlayerOSX::setSource(const std::string& url)
    {
    }

    void VideoPlayerOSX::setVolume(float volume)
    {
    }

    bool VideoPlayerOSX::isPlaying() const
    {
        return false;
    }

    void VideoPlayerOSX::update()
    {
    }

    const OTextureRef& VideoPlayerOSX::getRenderTarget()
    {
        return m_pRenderTarget;
    }
}
