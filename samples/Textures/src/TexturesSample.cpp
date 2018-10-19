// Oak Nut include
#include <onut/Anim.h>
#include <onut/ContentManager.h>
#include <onut/Files.h>
#include <onut/Font.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>

OTextureRef pTextureFromFile;
OTextureRef pTextureFromFileData;
OTextureRef pTextureFromData;
OTextureRef pRenderTarget;
OTextureRef pDynamic;

uint8_t dynamicData[128 * 128 * 4];
OAnimInt dynamicAnim = 0;

void initSettings()
{
    oSettings->setGameName("Sprites Sample");
}

void init()
{
    auto filename = oContentManager->findResourceFile("onutLogo.png");

    // From file (Easiest way is to let the content manager find it using OGetTexture)
    pTextureFromFile = OTexture::createFromFile(filename);

    // From file data
    auto fileData = onut::getFileData(filename);
    pTextureFromFileData = OTexture::createFromFileData(fileData.data(), fileData.size());

    // From raw data
    const uint8_t rawData[] = {
        255, 0, 255, 255,
        150, 150, 150, 255,
        150, 150, 150, 255,
        255, 0, 255, 255
    };
    pTextureFromData = OTexture::createFromData(rawData, {2, 2});

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
    dynamicAnim.play(0, 255, 3.f, OTweenLinear, OLoop);
}

void update()
{
}

void render()
{
    // Clear
    oRenderer->clear(OColorHex(1d232d));

    // Draw stuff to our render target
    oRenderer->renderStates.renderTarget.push(pRenderTarget);
    oRenderer->renderStates.viewport.push({0, 0, pRenderTarget->getSize().x, pRenderTarget->getSize().y});
    oSpriteBatch->begin();
    oSpriteBatch->drawRect(pTextureFromFile, {0, 0, 128, 128});
    oSpriteBatch->drawRect(pTextureFromFile, {128, 0, 128, 128});
    oSpriteBatch->drawRect(pTextureFromFile, {0, 128, 128, 128});
    oSpriteBatch->drawRect(pTextureFromFile, {128, 128, 128, 128});
    oSpriteBatch->end();
    oRenderer->renderStates.viewport.pop();
    oRenderer->renderStates.renderTarget.pop();

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
    auto pFont = OGetFont("font.fnt");
    oSpriteBatch->begin(Matrix::CreateTranslation(80, 212, 0));
    oRenderer->renderStates.sampleFiltering = OFilterNearest;

    oSpriteBatch->drawRect(pTextureFromFile, {0, 0, 128, 128});
    oSpriteBatch->drawRect(pTextureFromFileData, {128, 0, 128, 128});
    oSpriteBatch->drawRect(pTextureFromData, {256, 0, 128, 128});
    oSpriteBatch->drawRect(pRenderTarget, {384, 0, 128, 128});
    oSpriteBatch->drawRect(pDynamic, {512, 0, 128, 128});
    
    pFont->draw("From File", {64, 140}, OCenter);
    pFont->draw("From File Data", {64 + 128, 140}, OCenter);
    pFont->draw("From Data", {64 + 256, 140}, OCenter);
    pFont->draw("Render Target", {64 + 384, 140}, OCenter);
    pFont->draw("Dynamic", {64 + 512, 140}, OCenter);
    
    oSpriteBatch->end();
}

void postRender()
{
}

void renderUI()
{
}
