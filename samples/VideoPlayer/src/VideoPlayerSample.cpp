// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
#include <onut/VideoPlayer.h>

void init();
void update();
void render();

OVideoPlayerRef pPlayer;
OTextureRef pVideoTexture;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Video Player Sample");
    ORun(init, update, render);
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
