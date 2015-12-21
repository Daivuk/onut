// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include "onut.h"

void init();
void update();
void render();

OTexture* pBlured = nullptr;
OTexture* pSepia = nullptr;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    OSettings->setGameName("Sprites Sample");
    OSettings->setIsResizableWindow(true);
    ORun(init, update, render);
}

void init()
{
    pBlured = OTexture::createRenderTarget({256, 256});
    pSepia = OTexture::createRenderTarget({256, 256});
}

void update()
{
}

void render()
{
    auto pLandscape = OGetTexture("landscape.png");
    auto drawLandscapeToRenderTarget = [=](OTexture* pRenderTarget)
    {
        pRenderTarget->bindRenderTarget();
        OSB->begin();
        OSB->drawRect(pLandscape, {0, 0, 256, 256});
        OSB->end();
        pRenderTarget->unbindRenderTarget();
    };

    // Clear
    ORenderer->clear(OColorHex(1d232d));

    // Draw landscape to our render targets
    drawLandscapeToRenderTarget(pBlured);
    drawLandscapeToRenderTarget(pSepia);
    
    // Apply effects
    pBlured->blur();
    pSepia->sepia();

    // Draw out resulted textures
    auto pFont = OGetBMFont("font.fnt");
    OSpriteBatch->begin();

    OSB->drawRect(pLandscape, {0, 0, 256, 256});
    OSB->drawRect(pBlured, {256, 0, 256, 256});
    OSB->drawRect(pSepia, {512, 0, 256, 256});

    pFont->draw<OCenter>("Original", {128 + 0, 256 + 8});
    pFont->draw<OCenter>("Blur", {128 + 256, 256 + 8});
    pFont->draw<OCenter>("Sepia", {128 + 512, 256 + 8});

    OSpriteBatch->end();
}
