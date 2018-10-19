// Oak Nut include
#include <onut/Anim.h>
#include <onut/ComponentFactory.h>
#include <onut/Entity.h>
#include <onut/Input.h>
#include <onut/Renderer.h>
#include <onut/SceneManager.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>

// Game includes
#include "Bat.h"
#include "Chest.h"
#include "Exit.h"
#include "Player.h"
#include "DamageFlasher.h"
#include "Damager.h"
#include "Door.h"
#include "DoorTraverser.h"
#include "Dungeon.h"
#include "Guard.h"
#include "Life.h"
#include "Player.h"
#include "PushBack.h"
#include "OnRoomResetter.h"
#include "SmartRoomCamera.h"
#include "TreasureHunter.h"
#include "Vase.h"

OAnimFloat g_fadeAnim(1.0f);

void initSettings()
{
    oSettings->setGameName("Dungeon Game Sample");
    oSettings->setIsRetroMode(true);
    oSettings->setIsResizableWindow(true);
    oSettings->setRetroResolution(Point(192, 144));
    oSettings->setResolution(oSettings->getRetroResolution() * 4);
    //oSettings->setBorderlessFullscreen(true);
}

void registerComponents()
{
    // Bat enemy
    ORegisterComponent(Bat);

    // A chest containing treasures!
    ORegisterComponent(Chest);
    OBindIntProperty(Chest, Gold);
    OBindIntProperty(Chest, Bomb);

    // Component allowing to control a character
    ORegisterComponent(Player);
    OBindFloatProperty(Player, Speed);
    OBindSoundProperty(Player, StepSound);
    OBindIntProperty(Player, BombCount);

    // If attached to an entity, it will flash white
    // upon receiving damage.
    ORegisterComponent(DamageFlasher);

    // A component that inflicts damage when touched
    ORegisterComponent(Damager);
    OBindIntProperty(Damager, Damage);

    // Player steps on this, end of level
    ORegisterComponent(Exit);

    // A door
    ORegisterComponent(Door);
    OBindBoolProperty(Door, Open);
    OBindSoundProperty(Door, OpenSound);
    OBindSoundProperty(Door, CloseSound);
    OBindEntityProperty(Door, Target);
    OBindBoolProperty(Door, NeedBomb);

    // Allows the entity to traverse doors
    ORegisterComponent(DoorTraverser);

    // Guard enemy
    ORegisterComponent(Guard);
    OBindStringProperty(Guard, Dir);

    // Life receives damage, and gets destroyed once reached zero
    ORegisterComponent(Life);
    OBindIntProperty(Life, Amount);
    OBindSoundProperty(Life, DamageSound);

    // Will push back the entity if damaged
    ORegisterComponent(PushBack);
    OBindFloatProperty(PushBack, Strength);

    // When the player enters a room, this component will reset
    // the entity's position and call a Reset message on itself.
    ORegisterComponent(OnRoomResetter);

    // Will position itself to follow the main player,
    // but also respecting room restrictions
    ORegisterComponent(SmartRoomCamera);

    // A treasure hunter entity can open chest upon touching them
    ORegisterComponent(TreasureHunter);

    // Vases can be broke and they usually contain gold or health potion
    ORegisterComponent(Vase);
    OBindIntProperty(Vase, Gold);
}

void init()
{
    // Register components
    registerComponents();

    // Create our dungeon
    g_pDungeon = OMake<Dungeon>("dungeon.tmx");
}

void update()
{
}

void render()
{
}

void postRender()
{
    // Global fade
    float fade = g_fadeAnim.get();
    if (fade < 1.0f)
    {
        auto fadeColor = OLerp(Color::Black, Color::Transparent, fade);
        oSpriteBatch->begin();
        oSpriteBatch->drawRect(nullptr, OScreenRectf, fadeColor);
        oSpriteBatch->end();
    }

    // Health
    auto pPlayer = g_pDungeon->getPlayer();
    if (pPlayer)
    {
        oSpriteBatch->begin(Matrix::CreateScale(4.0f));
        oRenderer->renderStates.sampleFiltering = OFilterNearest;

        auto pPlayerLife = pPlayer->getComponent<Life>();
        if (pPlayerLife)
        {
            auto pHeartTexture = OGetTexture("heart.PNG");
            for (int i = 0; i < pPlayerLife->getAmount(); ++i)
            {
                oSpriteBatch->drawSprite(pHeartTexture, Vector2(OScreenWf / 4.0f - (float)i * 8.0f - 12.0f, 12.0f));
            }
        }

        auto pPlayerComponent = pPlayer->getComponent<Player>();
        if (pPlayerComponent)
        {
            auto pCoinTexture = OGetTexture("coinIcon.PNG");
            for (int i = 0; i < pPlayerComponent->getCoinCount(); ++i)
            {
                oSpriteBatch->drawSprite(pCoinTexture, Vector2(OScreenWf / 4.0f - (float)i * 2.0f - 12.0f, 24.0f));
            }
        }
        if (pPlayerComponent)
        {
            auto pBombTexture = OGetTexture("bombIcon.PNG");
            for (int i = 0; i < pPlayerComponent->getBombCount(); ++i)
            {
                oSpriteBatch->drawSprite(pBombTexture, Vector2(OScreenWf / 4.0f - (float)i * 8.0f - 12.0f, 36.0f));
            }
        }

        oSpriteBatch->end();
    }

    // Draw some useful debug info from the scene manager
    oSceneManager->DrawDebugInfo();
}

void renderUI()
{
}
