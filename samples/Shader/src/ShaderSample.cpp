// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/Maths.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Shader.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
#include <onut/Timing.h>

void init();
void update();
void render();

float vertexAnim = 0.0f;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Shader Sample");
    ORun(init, update, render);
}

void init()
{
}

void update()
{
    vertexAnim += OGetDeltaTime();
}

void render()
{
    auto pNutTexture = OGetTexture("onutLogo.png");
    auto pVertexShader = OGetShader("sample.vs");
    //auto pPixelShader = OGetShader("sample.ps");

    // Clear
    oRenderer->clear(OColorHex(1d232d));

    oSpriteBatch->begin();

    // Set custom vertex shader
    oRenderer->renderStates.vertexShader = pVertexShader;

    // Set uniform
    pVertexShader->setFloat("anim", vertexAnim);

    // Draw sprite
    oSpriteBatch->drawSprite(pNutTexture, OScreenCenterf, Color(1, 0, 1, 1), 35.f);
    oSpriteBatch->end();
}
