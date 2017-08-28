// Oak Nut include
#include <onut/EntityFactory.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SoundComponent.h>

void initSettings()
{
    oSettings->setGameName("Entities Sample");
}

void init()
{
    auto pTiledMapEntity = OCreateTiledMapEntity("sample.tmx");

    auto pSpriteEntity = OCreateSpriteEntity("onutLogo.png", Vector2(50, 70));
    pTiledMapEntity->add(pSpriteEntity);

    auto pTextEntity = OCreateTextEntity("main.fnt", "Hello World", Vector2(100, 200));
    pTiledMapEntity->add(pTextEntity);

    auto pSoundEntity = OCreateSoundEntity("nature.wav");
    auto pSoundComponent = pSoundEntity->getComponent<OSoundComponent>();
    pSoundComponent->setLoop(true);
    pSoundComponent->play();
    pTiledMapEntity->add(pSoundEntity);
}

void update()
{
}

void render()
{
    // Clear
    oRenderer->clear(OColorHex(1d232d));
}

void postRender()
{
}

void renderUI()
{
}
