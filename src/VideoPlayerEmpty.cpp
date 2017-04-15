// Onut
#include <onut/Texture.h>

// Internal
#include "VideoPlayerEmpty.h"

namespace onut
{
    OVideoPlayerRef VideoPlayer::createWithRenderTarget(const OTextureRef& pRenderTarget)
    {
        auto pRet = std::make_shared<VideoPlayerEmpty>();
        pRet->m_pRenderTarget = pRenderTarget;
        return pRet;
    }

    VideoPlayerEmpty::~VideoPlayerEmpty()
    {
    }

    void VideoPlayerEmpty::pause()
    {
    }

    void VideoPlayerEmpty::play()
    {
    }

    void VideoPlayerEmpty::setLoop(bool bLoop)
    {
    }

    void VideoPlayerEmpty::togglePlayPause()
    {
    }

    void VideoPlayerEmpty::setPlayRate(double playRate)
    {
    }

    void VideoPlayerEmpty::setSource(const std::string& url)
    {
    }

    void VideoPlayerEmpty::setVolume(float volume)
    {
    }

    bool VideoPlayerEmpty::isPlaying() const
    {
        return false;
    }

    void VideoPlayerEmpty::update()
    {
    }

    const OTextureRef& VideoPlayerEmpty::getRenderTarget()
    {
        return m_pRenderTarget;
    }
}
