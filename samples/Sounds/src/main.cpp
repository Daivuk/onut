// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include "onut.h"

void init();
void update();
void render();

OSoundInstance* pLoopingSound = nullptr;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    OSettings->setGameName("Sounds Sample");
    ORun(init, update, render);
}

void init()
{
    pLoopingSound = OCreateSoundInstance("looping.wav");
    pLoopingSound->setLoop(true);
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
        OPlaySound("sound1.wav", volume, balance, .5f);
    }
    if (OJustPressed(OINPUT_3))
    {
        OPlaySound("sound1.wav", volume, balance, 2.f);
    }
    if (OJustPressed(OINPUT_4))
    {
        OPlayRandomSound({"sound1.wav", "sound2.wav", "sound3.wav", "sound4.wav", "sound5.wav"}, 
                         volume, balance);
    }
    if (OJustPressed(OINPUT_5))
    {
        OPlaySound("sound5.wav", volume, balance);
    }
    if (OJustPressed(OINPUT_6))
    {
        if (pLoopingSound->isPlaying())
        {
            pLoopingSound->pause();
        }
        else
        {
            pLoopingSound->play();
        }
    }
    if (OJustPressed(OINPUT_7))
    {
        OPlaySoundCue("soundCue.xml", volume, balance);
    }
}

void render()
{
    // Clear to black
    ORenderer->clear({0, 0, 0, 1});

    // Draw info
    auto pFont = OGetBMFont("font.fnt");

    OSB->begin();

    pFont->draw("Press ^9901^999 to play sound1 at 100% speed", {10, 10});
    pFont->draw("Press ^9902^999 to play sound1 at 50% speed", {10, 30});
    pFont->draw("Press ^9903^999 to play sound1 at 200% speed", {10, 50});
    pFont->draw("Press ^9904^999 to play a random sound", {10, 70});
    pFont->draw("Press ^9905^999 to play sound5", {10, 90});
    if (pLoopingSound->isPlaying())
    {
        pFont->draw("Press ^9906^999 to start/stop looping sound (^099Playing^999)", {10, 110});
    }
    else
    {
        pFont->draw("Press ^9906^999 to start/stop looping sound", {10, 110});
    }
    pFont->draw("Press ^9907^999 to play cue file", {10, 130});

    pFont->draw("Hold ^990Left Arrow^999 to on left channel", {10, OScreenHf - 50});
    pFont->draw("Hold ^990Right Arrow^999 to on right channel", {10, OScreenHf - 30});

    OSB->end();
}
