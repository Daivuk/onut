// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include "onut/GamePad.h"
#include "onut/Settings.h"

#include "onut_old.h"

void init();
void update();
void render();

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Sprites Sample");
    ORun(init, update, render);
}

void init()
{
}

void update()
{
}

void drawController(int index, const Vector2& position)
{
    if (!OGetGamePad(index)->isConnected())
    {
        OSB->drawSprite(OGetTexture("gamepad.png"), position, Color(.5f));
    }
    else
    {
        OSB->drawSprite(OGetTexture(("gamepad" + std::to_string(index) + ".png").c_str()), position);
        OSB->drawSprite(OGetTexture("lthumb.png"), position + OGetGamePadLeftThumb(index) * 12.f);
        OSB->drawSprite(OGetTexture("rthumb.png"), position + OGetGamePadRightThumb(index) * 12.f);
        if (OGamePadPressed(OGamePadLeftThumb, index)) OSB->drawSprite(OGetTexture("lthumbPress.png"), position + OGetGamePadLeftThumb(index) * 12.f);
        if (OGamePadPressed(OGamePadRightThumb, index)) OSB->drawSprite(OGetTexture("rthumbPress.png"), position + OGetGamePadRightThumb(index) * 12.f);
        if (OGamePadPressed(OGamePadRightBumper, index)) OSB->drawSprite(OGetTexture("rb.png"), position);
        if (OGamePadPressed(OGamePadLeftBumper, index)) OSB->drawSprite(OGetTexture("lb.png"), position);
        if (OGamePadPressed(OGamePadBack, index)) OSB->drawSprite(OGetTexture("back.png"), position);
        if (OGamePadPressed(OGamePadStart, index)) OSB->drawSprite(OGetTexture("start.png"), position);
        if (OGamePadPressed(OGamePadA, index)) OSB->drawSprite(OGetTexture("a.png"), position);
        if (OGamePadPressed(OGamePadB, index)) OSB->drawSprite(OGetTexture("b.png"), position);
        if (OGamePadPressed(OGamePadX, index)) OSB->drawSprite(OGetTexture("x.png"), position);
        if (OGamePadPressed(OGamePadY, index)) OSB->drawSprite(OGetTexture("y.png"), position);
        if (OGamePadPressed(OGamePadDPadLeft, index)) OSB->drawSprite(OGetTexture("dleft.png"), position);
        if (OGamePadPressed(OGamePadDPadRight, index)) OSB->drawSprite(OGetTexture("dright.png"), position);
        if (OGamePadPressed(OGamePadDPadUp, index)) OSB->drawSprite(OGetTexture("dup.png"), position);
        if (OGamePadPressed(OGamePadDPadDown, index)) OSB->drawSprite(OGetTexture("ddown.png"), position);
        if (OGamePadPressed(OGamePadLeftTrigger, index)) OSB->drawSprite(OGetTexture("lt.png"), position);
        if (OGamePadPressed(OGamePadRightTrigger, index)) OSB->drawSprite(OGetTexture("rt.png"), position);
    }
}

void render()
{
    // Clear
    ORenderer->clear(Color::Black);

    // Begin a batch
    OSpriteBatch->begin();

    drawController(0, Vector2(OScreenWf * .3f, OScreenHf * .3f));
    drawController(1, Vector2(OScreenWf * .7f, OScreenHf * .3f));
    drawController(2, Vector2(OScreenWf * .3f, OScreenHf * .7f));
    drawController(3, Vector2(OScreenWf * .7f, OScreenHf * .7f));

    // End and flush the batch
    OSpriteBatch->end();
}
