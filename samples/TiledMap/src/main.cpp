// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include "onut.h"

void init();
void update();
void render();

onut::TiledMap* pTiledMap = nullptr;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    OSettings->setGameName("Tiled Map Sample");
    ORun(init, update, render);
}

void init()
{
    pTiledMap = new onut::TiledMap("../../assets/maps/sample.tmx");
}

void update()
{
}

void render()
{
    pTiledMap->render(RECT{0, 0, OScreenW, OScreenH});
}
