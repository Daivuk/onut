// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/Anim.h>
#include <onut/ComponentFactory.h>
#include <onut/Entity.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>

// Game includes
#include "Bat.h"
#include "Chest.h"
#include "Controllable.h"
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

void init();
void update();
void render();
void postRender();

OAnimFloat g_fadeAnim(1.0f);

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Dungeon Game Sample");
    ORun(init, update, render, postRender);
}

void registerComponents()
{
    // Bat enemy
    ORegisterComponent(Bat);

    // A chest containing treasures!
    ORegisterComponent(Chest);
    OBindIntProperty(Chest, Gold);

    // Component allowing to control a character
    ORegisterComponent(Controllable);
    OBindFloatProperty(Controllable, Speed);
    OBindSoundProperty(Controllable, StepSound);

    // If attached to an entity, it will flash white
    // upon receiving damage.
    ORegisterComponent(DamageFlasher);

    // A component that inflicts damage when touched
    ORegisterComponent(Damager);
    OBindIntProperty(Damager, Damage);

    // A door
    ORegisterComponent(Door);
    OBindBoolProperty(Door, Open);
    OBindSoundProperty(Door, OpenSound);
    OBindSoundProperty(Door, CloseSound);
    OBindEntityProperty(Door, Target);

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
        auto pPlayerLife = pPlayer->getComponent<Life>();
        if (pPlayerLife)
        {
            auto pHeartTexture = OGetTexture("heart.PNG");
            oSpriteBatch->begin(Matrix::CreateScale(4.0f));
            oSpriteBatch->changeFiltering(OFilterNearest);
            for (int i = 0; i < pPlayerLife->getAmount(); ++i)
            {
                oSpriteBatch->drawSprite(pHeartTexture, Vector2((float)i * 8.0f + 12.0f, 12.0f));
            }
            oSpriteBatch->end();
        }
    }
}
