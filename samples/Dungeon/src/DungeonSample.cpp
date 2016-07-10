// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/EntityFactory.h>
#include <onut/onut.h>
#include <onut/Settings.h>

void init();
void update();
void render();

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Dungeon Game Sample");
    ORun(init, update, render);
}

void init()
{
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
