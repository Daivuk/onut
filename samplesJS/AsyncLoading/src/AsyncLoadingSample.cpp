// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/Async.h>
#include <onut/Dispatcher.h>
#include <onut/Font.h>
#include <onut/Log.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
#include <onut/Timing.h>

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
        std::this_thread::sleep_for(std::chrono::seconds(5));

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
    oSpriteBatch->begin();

    if (!loaded)
    {
        oSpriteBatch->drawSprite(OGetTexture("img1.png"), OScreenCenterf, Color::White, loadingRotation);
        pFont->draw("Loading", Vector2(OScreenCenterXf, OScreenCenterYf + 96.f), OCenter);
    }
    else
    {
        oSpriteBatch->drawSprite(OGetTexture("img2.png"), Vector2(OScreenCenterXf - 128.f, OScreenCenterYf));
        oSpriteBatch->drawSprite(OGetTexture("img3.png"), OScreenCenterf);
        oSpriteBatch->drawSprite(OGetTexture("img4.png"), Vector2(OScreenCenterXf + 128.f, OScreenCenterYf));
        pFont->draw("Loaded!", Vector2(OScreenCenterXf, OScreenCenterYf + 96.f), OCenter);
    }

    // End and flush the batch
    oSpriteBatch->end();
}
