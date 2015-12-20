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
    OSettings->setGameName("Sprites Sample");
    ORun(init, update, render);
}

void init()
{
}

void update()
{
    if (OGamePadJustPressed(OABtn))
    {
        OPlaySound("sound1.wav");
    }
    if (OGamePadJustPressed(OBBtn))
    {
        OPlaySound("sound2.wav");
    }
    if (OGamePadJustPressed(OXBtn))
    {
        OPlaySound("sound3.wav");
    }
    if (OGamePadJustPressed(OYBtn))
    {
        OPlaySound("sound4.wav");
    }
}

void render()
{
    // Clear
    ORenderer->clear(OColorHex(1d232d));

    // Begin a batch
    OSpriteBatch->begin(onut::SpriteBatch::eBlendMode::Add);

    Vector2 Lpos(OScreenCenterXf - 150.f, OScreenCenterYf);
    Vector2 Rpos(OScreenCenterXf + 150.f, OScreenCenterYf);

    // Player 1
    OSB->drawSprite(nullptr, Lpos + OLThumb(0) * 200.f, Color(1, 0, 0, 1), 0.f, 32.f);
    OSB->drawSprite(nullptr, Lpos + ORThumb(0) * 200.f, Color(0, 0, 1, 1), 0.f, 32.f);

    // Player 2
    OSB->drawSprite(nullptr, Rpos + OLThumb(1) * 200.f, Color(1, 0, 0, 1), 0.f, 32.f);
    OSB->drawSprite(nullptr, Rpos + ORThumb(1) * 200.f, Color(0, 0, 1, 1), 0.f, 32.f);

    // End and flush the batch
    OSpriteBatch->end();
}
