// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include "onut/Async.h"
#include "onut/Font.h"
#include "onut/Log.h"
#include "onut/Settings.h"
#include "onut/Texture.h"

#include "onut_old.h"

void init();
void update();
void render();

float loadingRotation = 0.f;
bool loaded = false;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Async Loading Sample");
    ORun(init, update, render);
}

void init()
{
    // Start loading
    OAsync([]
    {
        OGetTexture("img2.png");
        OGetTexture("img3.png");
        OGetTexture("img4.png");
        OLog("Loaded!");

        // Since this is going to be very fast, force a sleep here so we can see the loading screen
        OSleep(std::chrono::seconds(5));

        OSync([] // Sync back to main loop
        {
            loaded = true; // We could have used an std::atomic here. Its for showing you can trigger events safely from there
        });
    });
}

void update()
{
    loadingRotation += ODT * 360.f;
}

void render()
{
    auto pFont = OGetFont("font.fnt");

    // Clear
    oRenderer->clear(OColorHex(1d232d));

    // Begin a batch
    OSpriteBatch->begin();

    if (!loaded)
    {
        OSB->drawSprite(OGetTexture("img1.png"), OScreenCenterf, Color::White, loadingRotation);
        pFont->draw("Loading", Vector2(OScreenCenterXf, OScreenCenterYf + 96.f), OCenter);
    }
    else
    {
        OSB->drawSprite(OGetTexture("img2.png"), Vector2(OScreenCenterXf - 128.f, OScreenCenterYf));
        OSB->drawSprite(OGetTexture("img3.png"), OScreenCenterf);
        OSB->drawSprite(OGetTexture("img4.png"), Vector2(OScreenCenterXf + 128.f, OScreenCenterYf));
        pFont->draw("Loaded!", Vector2(OScreenCenterXf, OScreenCenterYf + 96.f), OCenter);
    }

    // End and flush the batch
    OSpriteBatch->end();
}
