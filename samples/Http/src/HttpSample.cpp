// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/Font.h>
#include <onut/Http.h>
#include <onut/Input.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>

void init();
void update();
void render();

std::string originIP;
std::string userAgent;
OTextureRef pDownloadedTexture;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("HTTP Sample");
    ORun(init, update, render);
}

void init()
{
}

void update()
{
    if (OInputJustPressed(OKey1))
    {
        // Stalls the main loop
        originIP = OHttpGetString("http://httpbin.org/ip");
    }
    if (OInputJustPressed(OKey2))
    {
        // Doesn't stall the main loop
        OHttpGetStringAsync("http://httpbin.org/user-agent", [](std::string result)
        {
            userAgent = result;
        });
    }
    if (OInputJustPressed(OKey3))
    {
        OHTTPGetTextureAsync("https://pbs.twimg.com/profile_images/438501336108113920/KYIwbos1.png", [](OTextureRef pTexture)
        {
            pDownloadedTexture = pTexture;
        });
    }
}

void render()
{
    // Clear
    oRenderer->clear(OColorHex(1d232d));

    // Draw Info
    auto pFont = OGetFont("font.fnt");

    oSpriteBatch->begin();

    pFont->draw("Press ^9911^999 to GET Origin IP.", {10, 10});
    pFont->draw(originIP, {10, 30}, OTopLeft, Color(.7f, .7f, .7f, 1));

    pFont->draw("Press ^9912^999 to GET User Agent Asynchronously.", {10, 110});
    pFont->draw(userAgent, {10, 130}, OTopLeft, Color(.7f, .7f, .7f, 1));

    pFont->draw("Press ^9913^999 to download a picture.", {10, 210});
    oSpriteBatch->drawRect(pDownloadedTexture, Rect(10, 230, 256, 256));

    oSpriteBatch->end();
}
