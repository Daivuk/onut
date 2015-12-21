// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include "onut.h"

void init();
void update();
void render();

OPlayer* pPlayer = nullptr;
OTexture* pVideoTexture = nullptr;
OAnim2 bounceAnim;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    OSettings->setGameName("Sprite Animation Sample");
    OSettings->setResolution({1280, 720});
    ORun(init, update, render);
}

void init()
{
    // Create a render target that the video will render to
    pVideoTexture = OTexture::createRenderTarget({320, 240});

    // Initialize our video using our rendering target
    pPlayer = OPlayer::Create();
    pPlayer->init(pVideoTexture);
    pPlayer->setSource("https://download.blender.org/durian/trailer/sintel_trailer-720p.mp4");
    pPlayer->play();

    // Start a loading animation to have something to draw on top of the video
    bounceAnim.startKeyframed(
        OScreenf,
        {
            OAnimAppleStyleBounce(OScreenf, Vector2(OScreenWf, OScreenHf - 50)),
            OAnimWait(OScreenf, 1.f)
        }, 
        OLoop);
}

void update()
{
    pPlayer->update();
}

void render()
{
    auto pOnutLogo = OGetTexture("onutLogo.png");
    auto pFont = OGetBMFont("font.fnt");

    // Clear
    ORenderer->clear(OColorHex(1d232d));

    OSpriteBatch->begin();
    OSB->drawRect(pVideoTexture, ORectFullScreen);
    OSB->drawSprite(pOnutLogo, bounceAnim, Color::White, Vector2::One);
    pFont->draw<ORight>("Pretend that's a loading animation ->\n^555(That's an Apple style bounce anim btw)", Vector2(OScreenWf - 128, OScreenHf - 32), Color(.75f));
    OSpriteBatch->end();
}
