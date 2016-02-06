// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include "onut.h"

void init();
void update();
void render();

OTexture* pBlured = nullptr;
OTexture* pSepia = nullptr;
OTexture* pCRT = nullptr;
OTexture* pCartoon = nullptr;
OTexture* pVignette = nullptr;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    OSettings->setGameName("Sprites Sample");
    ORun(init, update, render);
}

void init()
{
    pBlured = OTexture::createRenderTarget({256, 256});
    pSepia = OTexture::createRenderTarget({256, 256});
    pCRT = OTexture::createRenderTarget({256, 256});
    pCartoon = OTexture::createRenderTarget({256, 256});
    pVignette = OTexture::createRenderTarget({256, 256});
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
        OSB->drawRect(pLandscape, {0, 0, pRenderTarget->getSizef().x, pRenderTarget->getSizef().y});
        OSB->end();
        pRenderTarget->unbindRenderTarget();
    };

    // Clear
    ORenderer->clear(OColorHex(1d232d));

    // Draw landscape to our render targets
    drawLandscapeToRenderTarget(pBlured);
    drawLandscapeToRenderTarget(pSepia);
    drawLandscapeToRenderTarget(pCRT);
    drawLandscapeToRenderTarget(pCartoon);
    drawLandscapeToRenderTarget(pVignette);
    
    // Apply effects
    pBlured->blur();
    pSepia->sepia();
    pCRT->crt();
    pCartoon->cartoon();
    pVignette->vignette();

    // Draw out resulted textures
    auto pFont = OGetBMFont("font.fnt");
    OSpriteBatch->begin();

    OSB->drawRect(pLandscape, {0, 0, 256, 256});
    OSB->drawRect(pBlured, {256, 0, 256, 256});
    OSB->drawRect(pSepia, {512, 0, 256, 256});
    OSB->drawRect(pCRT, {0, 288, 256, 256});
    OSB->drawRect(pCartoon, {256, 288, 256, 256});
    OSB->drawRect(pVignette, {512, 288, 256, 256});

    pFont->draw<OCenter>("Original", {128 + 0, 256 + 8});
    pFont->draw<OCenter>("Blur", {128 + 256, 256 + 8});
    pFont->draw<OCenter>("Sepia", {128 + 512, 256 + 8});
    pFont->draw<OCenter>("CRT", {128 + 0, 288 + 256 + 8});
    pFont->draw<OCenter>("Cartoon", {128 + 256, 288 + 256 + 8});
    pFont->draw<OCenter>("Vignette", {128 + 512, 288 + 256 + 8});

    OSpriteBatch->end();
}
