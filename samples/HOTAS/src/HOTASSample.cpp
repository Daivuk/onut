// Oak Nut include
#include <onut/Font.h>
#include <onut/Input.h>
#include <onut/Joystick.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Sound.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>

void initSettings()
{
    oSettings->setGameName("HOTAS Sample");
    oSettings->setResolution({ 1280, 720});
    oSettings->setIsResizableWindow(true);
}

void init()
{
}

void shutdown()
{
}

void update()
{
    auto count = oInput->getJoystickCount();
    for (int i = 0; i < count; ++i)
    {
        auto pJoystick = OGetJoystick(i);
        if (!pJoystick) continue;
        
        auto buttonCount = pJoystick->getButtonCount();
        for (int j = 0; j < buttonCount; ++j)
        {
            if (pJoystick->isJustPressed(j))
            {
                OPlaySound("click.wav");
            }
        }
    }
}

void render()
{
    auto pFont = OGetFont("font.fnt");
    auto pSB = oSpriteBatch.get();

    // Clear
    oRenderer->clear(OColorHex(2f3133));

    // Begin a batch
    pSB->begin();

    auto count = oInput->getJoystickCount();
    auto frameSize = OScreenWf / (float)count;
    for (int i = 0; i < count; ++i)
    {
        auto pJoystick = OGetJoystick(i);
        if (!pJoystick) continue;
        
        float frameX = (float)i * frameSize + 10;
        pSB->drawOutterOutlineRect(Rect(frameX, 10, frameSize - 20, OScreenHf - 20), 1);

        // Info
        pSB->drawText(pFont, "Name: ^990" + pJoystick->getName(), {frameX + 10, 20});
        pSB->drawText(pFont, "Type: ^990" + pJoystick->getTypeName(), {frameX + 10, 40});

        // Axis
        auto axisCount = pJoystick->getAxisCount();
        for (int j = 0; j < axisCount; ++j)
        {
            auto value = pJoystick->getAxis(j);

            float y = 60 + (float)j * 40;

            pSB->drawText(pFont, 
                          "^990" + OGetJoystickPrettyAxisName(j, i), 
                          {frameX + 10, y});
            pSB->drawRect(nullptr, Rect(frameX + 10, y + 15, frameSize - 40, 1), Color(0.25f));
            for (int k = 0; k <= 16; ++k)
            {
                pSB->drawRect(nullptr, Rect(frameX + 10 + (frameSize - 40) * (float)k / 16, y + 15, 1, 10), Color(0.25f));
            }
            pSB->drawCross(Vector2(frameX + 10 + (frameSize - 40) * (value * 0.5f + 0.5f), y + 20), 5, Color(1, 1, 0));
        }

#define BTN_PAD 4.0f
#define BTN_W 96.0f
#define BTN_H 32.0f

        // Buttons
        Vector2 pos(10, 60 + (float)axisCount * 40);
        auto buttonCount = pJoystick->getButtonCount();
        for (int j = 0; j < buttonCount; ++j)
        {
            pSB->drawOutterOutlineRect(Rect(frameX + pos.x + BTN_PAD, pos.y + BTN_PAD, BTN_W - BTN_PAD * 2, BTN_H - BTN_PAD * 2), 
                                       1, Color(0.25f));
            auto pressed = pJoystick->isPressed(j);
            if (pressed)
            {
                pSB->drawRect(nullptr,
                              Rect(frameX + pos.x + BTN_PAD + 2, pos.y + BTN_PAD + 2, BTN_W - BTN_PAD * 2 - 4, BTN_H - BTN_PAD * 2 - 4), 
                              Color(0.5f));
            }
            pSB->drawText(pFont, 
                          OGetJoystickPrettyButtonName(j, i), 
                          {frameX + pos.x + (BTN_W / 2), pos.y + (BTN_H / 2)}, 
                          OCenter, pressed ? Color(1, 1, 0) : Color(0.5f));

            pos.x += BTN_W;
            if (pos.x >= frameSize - 20 - BTN_W)
            {
                pos.y += BTN_H;
                pos.x = 10;
            }
        }
    }

    // End and flush the batch
    pSB->end();
}

void postRender()
{
}

void renderUI()
{
}
