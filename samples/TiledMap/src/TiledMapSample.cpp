// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/onut.h>
#include <onut/Settings.h>
#include <onut/TiledMap.h>

void init();
void update();
void render();

OTiledMapRef pTiledMap;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Tiled Map Sample");
    ORun(init, update, render);
}

void init()
{
    pTiledMap = OGetTiledMap("sample.tmx");
}

void update()
{
}

void render()
{
    pTiledMap->render();
}
