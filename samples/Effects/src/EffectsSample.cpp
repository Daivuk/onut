// Oak Nut include
#include <onut/Font.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>

void init();
void update();
void render();

OTextureRef pBlured = nullptr;
OTextureRef pSepia = nullptr;
OTextureRef pCRT = nullptr;
OTextureRef pCartoon = nullptr;
OTextureRef pVignette = nullptr;

void initSettings()
{
    oSettings->setGameName("Effects Sample");
}

void init()
{
    pBlured = OTexture::createRenderTarget({256, 256});
    pSepia = OTexture::createRenderTarget({256, 256});
    pCRT = OTexture::createRenderTarget({256, 256});
    pCartoon = OTexture::createRenderTarget({256, 256});
    pVignette = OTexture::createRenderTarget({256, 256});
}

void shutdown()
{
}

void update()
{
}

void drawLandscapeToRenderTarget(const OTextureRef& pRenderTarget)
{
    auto pLandscape = OGetTexture("landscape.png");

    oRenderer->renderStates.renderTargets[0].push(pRenderTarget);
    oRenderer->renderStates.viewport.push({0, 0, pRenderTarget->getSize().x, pRenderTarget->getSize().y});
    oSpriteBatch->begin();
    oSpriteBatch->drawRect(pLandscape, {0, 0, pRenderTarget->getSizef().x, pRenderTarget->getSizef().y});
    oSpriteBatch->end();
    oRenderer->renderStates.viewport.pop();
    oRenderer->renderStates.renderTargets[0].pop();
}

void render()
{
    auto pLandscape = OGetTexture("landscape.png");

    // Clear
    oRenderer->clear(OColorHex(1d232d));

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
    auto pFont = OGetFont("font.fnt");
    oSpriteBatch->begin();

    oSpriteBatch->drawRect(pLandscape, {0, 0, 256, 256});
    oSpriteBatch->drawRect(pBlured, {256, 0, 256, 256});
    oSpriteBatch->drawRect(pSepia, {512, 0, 256, 256});
    oSpriteBatch->drawRect(pCRT, {0, 288, 256, 256});
    oSpriteBatch->drawRect(pCartoon, {256, 288, 256, 256});
    oSpriteBatch->drawRect(pVignette, {512, 288, 256, 256});

    pFont->draw("Original", {128 + 0, 256 + 8}, OCenter);
    pFont->draw("Blur", {128 + 256, 256 + 8}, OCenter);
    pFont->draw("Sepia", {128 + 512, 256 + 8}, OCenter);
    pFont->draw("CRT", {128 + 0, 288 + 256 + 8}, OCenter);
    pFont->draw("Cartoon", {128 + 256, 288 + 256 + 8}, OCenter);
    pFont->draw("Vignette", {128 + 512, 288 + 256 + 8}, OCenter);

    oSpriteBatch->end();
}

void postRender()
{
}

void renderUI()
{
}
