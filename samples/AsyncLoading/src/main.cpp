// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include "onut.h"

void init();
void update();
void render();

float loadingRotation = 0.f;
bool loaded = false;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    OSettings->setGameName("Async Loading Sample");
    ORun(init, update, render);
}

void init()
{
    // Start loading
    OAsync([]
    {
        // OGetTexture would also work, but it would sync to main thread for
        // each of them. Making the async loading a bit useless.
        OLoadTexture("img2.png");
        OLoadTexture("img3.png");
        OLoadTexture("img4.png");
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
    auto pFont = OGetBMFont("font.fnt");

    // Clear
    ORenderer->clear(OColorHex(1d232d));

    // Begin a batch
    OSpriteBatch->begin();

    if (!loaded)
    {
        OSB->drawSprite(OGetTexture("img1.png"), OScreenCenterf, Color::White, loadingRotation);
        pFont->draw<OCenter>("Loading", Vector2(OScreenCenterXf, OScreenCenterYf + 96.f));
    }
    else
    {
        OSB->drawSprite(OGetTexture("img2.png"), Vector2(OScreenCenterXf - 128.f, OScreenCenterYf));
        OSB->drawSprite(OGetTexture("img3.png"), OScreenCenterf);
        OSB->drawSprite(OGetTexture("img4.png"), Vector2(OScreenCenterXf + 128.f, OScreenCenterYf));
        pFont->draw<OCenter>("Loaded!", Vector2(OScreenCenterXf, OScreenCenterYf + 96.f));
    }

    // End and flush the batch
    OSpriteBatch->end();
}
