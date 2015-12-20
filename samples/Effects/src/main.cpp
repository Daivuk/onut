// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include "onut.h"

void init();
void update();
void render();

OTexture* pBlured = nullptr;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    OSettings->setGameName("Sprites Sample");
    ORun(init, update, render);
}

void init()
{
    pBlured = OTexture::createRenderTarget({256, 256});
}

void update()
{
}

void render()
{
    auto pLandscape = OGetTexture("landscape.png");

    // Clear
    ORenderer->clear(OColorHex(1d232d));

    // Draw something to our render target
    pBlured->bindRenderTarget();
    OSB->begin();
    OSB->drawRect(pLandscape, {0, 0, 256, 256});
    OSB->end();
    pBlured->unbindRenderTarget();
    
    // Blur
    pBlured->blur(32.f);

    // Draw out resulted textures
    auto pFont = OGetBMFont("font.fnt");
    OSpriteBatch->begin();

    OSB->drawRect(pLandscape, {0, 0, 256, 256});
    OSB->drawRect(pBlured, {256, 0, 256, 256});

    pFont->draw<OCenter>("Original", {128 + 0, 256 + 8});
    pFont->draw<OCenter>("Blur", {128 + 256, 256 + 8});

    OSpriteBatch->end();
}
