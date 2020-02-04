// Oak Nut include
#include <onut/Maths.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Shader.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
#include <onut/Timing.h>

float vertexAnim = 0.0f;
float pixelAnim = 0.0f;

OTextureRef pMTR[4];

void initSettings()
{
    oSettings->setGameName("Shader Sample");
}

void init()
{
    pMTR[0] = OTexture::createRenderTarget({ 128, 128 }, false, OFormatRGBA8);
    pMTR[1] = OTexture::createRenderTarget({ 128, 128 }, false, OFormatRGBA8);
    pMTR[2] = OTexture::createRenderTarget({ 128, 128 }, false, OFormatRGBA8);
    pMTR[3] = OTexture::createRenderTarget({ 128, 128 }, false, OFormatRGBA8);
}

void update()
{
    vertexAnim += OGetDeltaTime();
    pixelAnim += OGetDeltaTime() * .5f;
}

void render()
{
    auto pTexture0 = OGetTexture("texture0.png");
    auto pTexture1 = OGetTexture("texture1.png");

    auto pVertexShader = OGetShader("sample.vs");
    auto pPixelShader = OGetShader("sample.ps");

    auto pMrtPS = OGetShader("mrt.ps");

    // Clear
    oRenderer->clear(OColorHex(1d232d));

    oSpriteBatch->begin();

    // Set custom shaders
    oRenderer->renderStates.vertexShader = pVertexShader;
    oRenderer->renderStates.pixelShader = pPixelShader;
    oRenderer->renderStates.textures[0] = pTexture0;
    oRenderer->renderStates.textures[1] = pTexture1;

    // Set uniforms
    pVertexShader->setFloat("anim", vertexAnim);
    pPixelShader->setFloat("anim", pixelAnim);

    // Draw sprite
    oSpriteBatch->drawSprite(pTexture0, {OScreenWf / 2 - 150, OScreenHf / 2});

    oSpriteBatch->end();

    // MRT sample
    for (int i = 0; i < 4; ++i) oRenderer->renderStates.renderTargets[i].push(pMTR[i]);
    oSpriteBatch->begin();
    oRenderer->renderStates.pixelShader = pMrtPS;
    oSpriteBatch->drawRect(pTexture0, { 0, 0, 128, 128 });
    oSpriteBatch->end();
    for (int i = 0; i < 4; ++i) oRenderer->renderStates.renderTargets[i].pop();

    // Show are MRT result
    oSpriteBatch->begin();
    oSpriteBatch->drawSprite(pMTR[0], { OScreenWf / 2 + 150, OScreenHf / 2-64 });
    oSpriteBatch->drawSprite(pMTR[1], { OScreenWf / 2 + 150+128, OScreenHf / 2-64 });
    oSpriteBatch->drawSprite(pMTR[2], { OScreenWf / 2 + 150, OScreenHf / 2+64 });
    oSpriteBatch->drawSprite(pMTR[3], { OScreenWf / 2 + 150+128, OScreenHf / 2+64 });
    oSpriteBatch->end();
}

void postRender()
{
}

void renderUI()
{
}
