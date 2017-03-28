#ifndef VIDEOPLAYEROSX_H_INCLUDED
#define VIDEOPLAYEROSX_H_INCLUDED

// Onut
#include <onut/VideoPlayer.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Texture);
OForwardDeclare(VideoPlayerOSX);

namespace onut
{
    class VideoPlayerOSX final : public VideoPlayer, public std::enable_shared_from_this<VideoPlayerOSX>
    {
    public:
        ~VideoPlayerOSX();

        bool isPlaying() const override;
        void pause() override;
        void play() override;
        void setLoop(bool bLoop) override;
        void setPlayRate(double playRate) override;
        void setSource(const std::string& url) override;
        void setVolume(float volume) override;
        void togglePlayPause() override;
        void update() override;
        const OTextureRef& getRenderTarget() override;

    private:
        friend class VideoPlayer;
        
        OTextureRef m_pRenderTarget;
    };
}

#endif
