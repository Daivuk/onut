// Oak Nut include
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
#include <onut/VideoPlayer.h>

OVideoPlayerRef pPlayer;
OTextureRef pVideoTexture;

void initSettings()
{
    oSettings->setGameName("Video Player Sample");
}

void init()
{
    // Initialize our video using a rendering target
    pVideoTexture = OTexture::createRenderTarget({1, 1});
    pPlayer = OVideoPlayer::createWithRenderTarget(pVideoTexture);
    pPlayer->setSource("https://download.blender.org/durian/trailer/sintel_trailer-720p.mp4");
    pPlayer->play();
}

void update()
{
    pPlayer->update();
}

void render()
{
    // Clear
    oRenderer->clear(OColorHex(0));

    oSpriteBatch->begin();
    oSpriteBatch->drawRect(pVideoTexture, ORectFit(ORectFullScreen, pVideoTexture->getSize()));
    oSpriteBatch->end();
}

void postRender()
{
}

void renderUI()
{
}
