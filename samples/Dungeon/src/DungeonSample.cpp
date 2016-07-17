// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/Anim.h>
#include <onut/ComponentFactory.h>
#include <onut/EntityFactory.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>

// Game includes
#include "Chest.h"
#include "Controllable.h"
#include "Damager.h"
#include "Door.h"
#include "DoorTraverser.h"
#include "Guard.h"
#include "Player.h"
#include "SmartRoomCamera.h"
#include "TreasureHunter.h"
#include "Vase.h"

void init();
void update();
void render();
void postRender();

OAnimFloat m_fadeAnim(1.0f);

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Dungeon Game Sample");
    ORun(init, update, render, postRender);
}

void registerComponents()
{
    // A chest containing treasures!
    ORegisterComponent(Chest);
    OBindIntProperty(Chest, Gold);

    // Component allowing to controller a character
    ORegisterComponent(Controllable);
    OBindFloatProperty(Controllable, Speed);
    OBindSoundProperty(Controllable, StepSound);

    // A component that inflicts damage when triggered
    ORegisterComponent(Damager);
    OBindIntProperty(Damager, Damage);

    // A door
    ORegisterComponent(Door);
    OBindBoolProperty(Door, Open);
    OBindSoundProperty(Door, OpenSound);
    OBindSoundProperty(Door, CloseSound);
    OBindEntityProperty(Door, Target);

    // Allows the owner entity to traverse doors
    ORegisterComponent(DoorTraverser);

    // Will position itself to follow the main player, but also respected room restrictions
    ORegisterComponent(SmartRoomCamera);

    // Guards will be alerted if they look in your direction,
    // and pursue/attack you.
    ORegisterComponent(Guard);
    OBindIntProperty(Guard, Life);

    // A treasure hunter entity can open cheats
    ORegisterComponent(TreasureHunter);

    // Vases can be broke and they usually contain gold or health
    ORegisterComponent(Vase);
    OBindIntProperty(Vase, Gold);
}

void init()
{
    registerComponents();

    // Create our tiled map entity.
    // The TiledMapComponent will look for the "entities" object layer and 
    // instantiate entities and their components based on properties
    // set in Tiled editor.
    auto pTiledMapEntity = OCreateTiledMapEntity("dungeon.tmx");
}

void update()
{
}

void render()
{
}

void postRender()
{
    float fade = m_fadeAnim.get();
    if (fade < 1.0f)
    {
        auto fadeColor = OLerp(Color::Black, Color::Transparent, fade);
        oSpriteBatch->begin();
        oSpriteBatch->drawRect(nullptr, OScreenRectf, fadeColor);
        oSpriteBatch->end();
    }
}
