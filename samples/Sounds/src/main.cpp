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
    OSettings->setGameName("Sounds Sample");
    ORun(init, update, render);
}

void init()
{
}

void update()
{
    float volume = 1.f;
    float balance = 0.f;

    if (OPressed(OINPUT_LEFT)) balance -= 1.f;
    if (OPressed(OINPUT_RIGHT)) balance += 1.f;

    if (OJustPressed(OINPUT_1))
    {
        OPlaySound("sound1.wav", volume, balance);
    }
    if (OJustPressed(OINPUT_2))
    {
        OPlaySound("sound2.wav", volume, balance);
    }
    if (OJustPressed(OINPUT_3))
    {
        OPlaySound("sound3.wav", volume, balance);
    }
    if (OJustPressed(OINPUT_4))
    {
        OPlaySound("sound4.wav", volume, balance);
    }
    if (OJustPressed(OINPUT_5))
    {
        OPlaySound("sound5.wav", volume, balance);
    }
}

void render()
{
    // Clear to black
    ORenderer->clear({0, 0, 0, 1});

    // Draw info
    auto pFont = OGetBMFont("font.fnt");

    pFont->draw("Press ^9901-5^999 to play sounds", {10, 10});
    pFont->draw("Hold ^990Left Arrow^999 to on left channel", {10, 30});
    pFont->draw("Hold ^990Right Arrow^999 to on right channel", {10, 50});
}
