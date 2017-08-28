// Oak Nut include
#include <onut/Font.h>
#include <onut/Http.h>
#include <onut/Input.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>

std::string originIP;
std::string userAgent;
OTextureRef pDownloadedTexture;
std::string postResult;

void initSettings()
{
    oSettings->setGameName("HTTP Sample");
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
        OHTTPGetTextureAsync("https://upload.wikimedia.org/wikipedia/commons/thumb/4/47/PNG_transparency_demonstration_1.png/560px-PNG_transparency_demonstration_1.png", [](OTextureRef pTexture)
        {
            pDownloadedTexture = pTexture;
        });
    }
    if (OInputJustPressed(OKey4))
    {
        const char* szData = "Hello World!";
        OHttp::Body body(szData, szData + strlen(szData) + 1);
        postResult = OHttpPost("http://httpbin.org/post", body);
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
    if (pDownloadedTexture)
        oSpriteBatch->drawRect(pDownloadedTexture, Rect(10, 230, pDownloadedTexture->getSizef()));

    pFont->draw("Press ^9914^999 to POST.", {400, 10});
    pFont->draw(postResult, {400, 30}, OTopLeft, Color(.7f, .7f, .7f, 1));

    oSpriteBatch->end();
}

void postRender()
{
}

void renderUI()
{
}
