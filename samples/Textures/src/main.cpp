// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include "onut.h"

void init();
void update();
void render();

OTexture* pTextureFromFile = nullptr;
OTexture* pTextureFromFileData = nullptr;
OTexture* pTextureFromData = nullptr;
OTexture* pRenderTarget = nullptr;
OTexture* pDynamic = nullptr;

uint8_t dynamicData[128 * 128 * 4];
OAnimi dynamicAnim = 0;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    OSettings->setGameName("Sprites Sample");
    ORun(init, update, render);
}

void init()
{
    // From file
    pTextureFromFile = OTexture::createFromFile("../../assets/textures/onutLogo.png");

    // From file data
    auto fileData = onut::getFileData("../../assets/textures/onutLogo.png");
    pTextureFromFileData = OTexture::createFromFileData(fileData);

    // From raw data
    const uint8_t rawData[] = {
        255, 0, 255, 255,
        150, 150, 150, 255,
        150, 150, 150, 255,
        255, 0, 255, 255
    };
    pTextureFromData = OTexture::createFromData({2, 2}, rawData);

    // Create a render target
    pRenderTarget = OTexture::createRenderTarget({256, 256});

    // Dynamic
    pDynamic = OTexture::createDynamic({128, 128});
    for (auto j = 0; j < 128; ++j)
    {
        for (auto i = 0; i < 128; ++i)
        {
            auto k = (j * 128 + i) * 4;
            dynamicData[k + 0] = j * 2;
            dynamicData[k + 1] = j * 2;
            dynamicData[k + 2] = j * 2;
            dynamicData[k + 3] = 255;
        }
    }
    dynamicAnim.start(0, 255, 3.f, OLinear, OLoop);
}

void update()
{
}

void render()
{
    // Clear
    ORenderer->clear(OColorHex(1d232d));

    // Draw stuff to our render target
    pRenderTarget->bindRenderTarget();
    OSpriteBatch->begin();
    OSB->drawRect(pTextureFromFile, {0, 0, 128, 128});
    OSB->drawRect(pTextureFromFile, {128, 0, 128, 128});
    OSB->drawRect(pTextureFromFile, {0, 128, 128, 128});
    OSB->drawRect(pTextureFromFile, {128, 128, 128, 128});
    OSpriteBatch->end();
    pRenderTarget->unbindRenderTarget();

    // Update our dynamic texture
    auto animValue = dynamicAnim.get();
    for (auto j = 0; j < 128; ++j)
    {
        for (auto i = 0; i < 128; ++i)
        {
            auto k = (j * 128 + i) * 4;
            dynamicData[k + 0] = j * 2 - animValue;
            dynamicData[k + 1] = j * 2 + animValue;
            dynamicData[k + 2] = j * 2;
            dynamicData[k + 3] = 255;
        }
    }
    pDynamic->setData(dynamicData);

    // Draw out resulted textures
    auto pFont = OGetBMFont("font.fnt");
    OSpriteBatch->begin(Matrix::CreateTranslation(80, 212, 0));
    OSpriteBatch->changeFiltering(onut::SpriteBatch::eFiltering::Nearest);

    OSB->drawRect(pTextureFromFile, {0, 0, 128, 128});
    OSB->drawRect(pTextureFromFileData, {128, 0, 128, 128});
    OSB->drawRect(pTextureFromData, {256, 0, 128, 128});
    OSB->drawRect(pRenderTarget, {384, 0, 128, 128});
    OSB->drawRect(pDynamic, {512, 0, 128, 128});

    pFont->draw<OCenter>("From File", {64, 140});
    pFont->draw<OCenter>("From File Data", {64 + 128, 140});
    pFont->draw<OCenter>("From Data", {64 + 256, 140});
    pFont->draw<OCenter>("Render Target", {64 + 384, 140});
    pFont->draw<OCenter>("Dynamic", {64 + 512, 140});

    OSpriteBatch->end();
}
