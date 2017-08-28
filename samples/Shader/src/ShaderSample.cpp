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

void initSettings()
{
    oSettings->setGameName("Shader Sample");
}

void init()
{
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
    oSpriteBatch->drawSprite(pTexture0, OScreenCenterf);
    oSpriteBatch->end();
}

void postRender()
{
}

void renderUI()
{
}
