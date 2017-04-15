#ifndef VIDEOPLAYEREMPTY_H_INCLUDED
#define VIDEOPLAYEREMPTY_H_INCLUDED

// Onut
#include <onut/VideoPlayer.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Texture);
OForwardDeclare(VideoPlayerEmpty);

namespace onut
{
    class VideoPlayerEmpty final : public VideoPlayer
    {
    public:
        ~VideoPlayerEmpty();

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
