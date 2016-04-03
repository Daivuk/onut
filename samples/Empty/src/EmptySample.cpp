// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/ContentManager.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>

void init();
void update();
void render();

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Empty Sample");
    ORun(init, update, render);
}

void init()
{
}

void update()
{
}

void render()
{
    // Clear
    oRenderer->clear(OColorHex(1d232d));
}
