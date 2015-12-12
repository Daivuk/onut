// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include "onut.h"

void init();
void update();
void render();

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    OSettings->setGameName("Particles Sample");
    ORun(init, update, render);
}

void init()
{
}

void update()
{
    if (OJustPressed(OINPUT_SPACE))
    {
        OEmitPFX("test.pfx", Vector3{OScreenWf / 2, OScreenHf / 2, 0});
    }
}

void render()
{
    // Clear to black
    ORenderer->clear({0, 0, 0, 1});

    // Draw info
    auto pFont = OGetBMFont("font.fnt");
    pFont->draw("Press ^990Space Bar^999 to spawn particles", {10, 10});
}
