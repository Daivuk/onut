#include <Windows.h>
#include "onut.h"

void init();
void update();
void render();

int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    // Default settings
    OSettings->setResolution({1280, 720});
    OSettings->setGameName("Oak Nut Particle System Editor");

    // Run
    onut::run(init, update, render);

    return 0;
}

void init()
{
}

void update()
{
}

void render()
{
}
