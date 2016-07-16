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
#include "Controllable.h"
#include "Door.h"
#include "DoorTraverser.h"
#include "SmartRoomCamera.h"

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
    ORegisterComponent(Controllable);
    OBindFloatProperty(Controllable, Speed);

    ORegisterComponent(Door);
    OBindBoolProperty(Door, Open);
    OBindSoundProperty(Door, OpenSound);
    OBindSoundProperty(Door, CloseSound);
    OBindEntityProperty(Door, Target);

    ORegisterComponent(DoorTraverser);

    ORegisterComponent(SmartRoomCamera);
}

void init()
{
    registerComponents();

    // Create our tiled map entity.
    // The factory will look for the "entities" object layer and 
    // instantiate entities and their components based on properties
    // set in TiledMap editor.
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
