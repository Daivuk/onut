// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/GamePad.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>

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
        oSpriteBatch->drawSprite(OGetTexture("gamepad.png"), position, Color(.5f));
    }
    else
    {
        oSpriteBatch->drawSprite(OGetTexture(("gamepad" + std::to_string(index) + ".png").c_str()), position);
        oSpriteBatch->drawSprite(OGetTexture("lthumb.png"), position + OGetGamePadLeftThumb(index) * 12.f);
        oSpriteBatch->drawSprite(OGetTexture("rthumb.png"), position + OGetGamePadRightThumb(index) * 12.f);
        if (OGamePadPressed(OGamePadLeftThumb, index)) oSpriteBatch->drawSprite(OGetTexture("lthumbPress.png"), position + OGetGamePadLeftThumb(index) * 12.f);
        if (OGamePadPressed(OGamePadRightThumb, index)) oSpriteBatch->drawSprite(OGetTexture("rthumbPress.png"), position + OGetGamePadRightThumb(index) * 12.f);
        if (OGamePadPressed(OGamePadRightBumper, index)) oSpriteBatch->drawSprite(OGetTexture("rb.png"), position);
        if (OGamePadPressed(OGamePadLeftBumper, index)) oSpriteBatch->drawSprite(OGetTexture("lb.png"), position);
        if (OGamePadPressed(OGamePadBack, index)) oSpriteBatch->drawSprite(OGetTexture("back.png"), position);
        if (OGamePadPressed(OGamePadStart, index)) oSpriteBatch->drawSprite(OGetTexture("start.png"), position);
        if (OGamePadPressed(OGamePadA, index)) oSpriteBatch->drawSprite(OGetTexture("a.png"), position);
        if (OGamePadPressed(OGamePadB, index)) oSpriteBatch->drawSprite(OGetTexture("b.png"), position);
        if (OGamePadPressed(OGamePadX, index)) oSpriteBatch->drawSprite(OGetTexture("x.png"), position);
        if (OGamePadPressed(OGamePadY, index)) oSpriteBatch->drawSprite(OGetTexture("y.png"), position);
        if (OGamePadPressed(OGamePadDPadLeft, index)) oSpriteBatch->drawSprite(OGetTexture("dleft.png"), position);
        if (OGamePadPressed(OGamePadDPadRight, index)) oSpriteBatch->drawSprite(OGetTexture("dright.png"), position);
        if (OGamePadPressed(OGamePadDPadUp, index)) oSpriteBatch->drawSprite(OGetTexture("dup.png"), position);
        if (OGamePadPressed(OGamePadDPadDown, index)) oSpriteBatch->drawSprite(OGetTexture("ddown.png"), position);
        if (OGamePadPressed(OGamePadLeftTrigger, index)) oSpriteBatch->drawSprite(OGetTexture("lt.png"), position);
        if (OGamePadPressed(OGamePadRightTrigger, index)) oSpriteBatch->drawSprite(OGetTexture("rt.png"), position);
    }
}

void render()
{
    // Clear
    oRenderer->clear(Color::Black);

    // Begin a batch
    oSpriteBatch->begin();

    drawController(0, Vector2(OScreenWf * .3f, OScreenHf * .3f));
    drawController(1, Vector2(OScreenWf * .7f, OScreenHf * .3f));
    drawController(2, Vector2(OScreenWf * .3f, OScreenHf * .7f));
    drawController(3, Vector2(OScreenWf * .7f, OScreenHf * .7f));

    // End and flush the batch
    oSpriteBatch->end();
}
