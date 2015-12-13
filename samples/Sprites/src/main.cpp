// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include "onut.h"

void init();
void update();
void render();

float g_spriteAngle = 0.f;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    OSettings->setGameName("Sprites Sample");
    ORun(init, update, render);
}

void init()
{
}

void update()
{
    g_spriteAngle += ODT * 45.f;
}

void render()
{
    auto pNutTexture = OGetTexture("onutLogo.png");
    auto pFrameTexture = OGetTexture("frameSmall.png");
    auto pChainTexture = OGetTexture("chain.png");

    // Clear
    ORenderer->clear(OColorHex(1d232d));

    // Begin a batch
    OSpriteBatch->begin();

    // Opaque
    OSpriteBatch->drawRect(nullptr, Rect(0, 0, 64, 64));
    OSpriteBatch->drawRect(nullptr, Rect(64, 0, 64, 64), Color(1, 0, 0, 1)); // Red
    OSpriteBatch->drawInclinedRect(nullptr, Rect(192, 0, 64, 64), -.5f); // Inclined

    // Textured
    OSpriteBatch->drawRect(pNutTexture, Rect(0, 64, 64, 64));
    OSpriteBatch->drawRect(pNutTexture, Rect(64, 64, 64, 64), Color(1, 0, 0, 1)); // Tinted

    // Per vertex coloring
    OSpriteBatch->drawRectWithColors(nullptr, Rect(0, 128, 64, 64), {Color(1, 0, 0, 1), Color(1, 1, 0, 1), Color(0, 1, 0, 1), Color(0, 0, 1, 1)});

    // Custom UVs
    OSpriteBatch->drawRectWithUVs(pNutTexture, Rect(64, 128, 64, 64), Vector4(0, 0, .5f, .5f));

    // Split the texture in 4 corner (Good for corsairs)
    OSpriteBatch->draw4Corner(pNutTexture, Rect(128, 64, 160, 160));

    // Scale 9, 9 Patch, 9 slice (whatever you guys call it)
    OSpriteBatch->drawRectScaled9(pFrameTexture, Rect(0, 256, 160, 160), Vector4(48, 48, 48, 48));
    OSpriteBatch->drawRectScaled9(pFrameTexture, Rect(160, 256, 256, 160), Vector4(48, 48, 48, 48));
    OSpriteBatch->drawRectScaled9RepeatCenters(pFrameTexture, Rect(416, 256, 256, 160), Vector4(48, 48, 48, 48));

    // Sprites
    OSpriteBatch->drawSprite(pNutTexture, Vector2(64, 480));
    OSpriteBatch->drawSprite(pNutTexture, Vector2(192, 480), Color::White, g_spriteAngle);
    OSpriteBatch->drawSprite(pNutTexture, Vector2(320, 480), Color::White, g_spriteAngle, .5f);

    // Lines
    OSpriteBatch->drawBeam(pChainTexture, Vector2(448, 480), Vector2(648, 440), 32.0f, Color::White);
    OSpriteBatch->drawBeam(pChainTexture, Vector2(448, 520), Vector2(648, 480), 32.0f, Color::White, g_spriteAngle * .1f);

    // End and flush the batch
    OSpriteBatch->end();
}
