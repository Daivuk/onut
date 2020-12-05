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

            pSB->drawText(pFont, "^990" + OGetJoystickAxisName(j, i), {frameX + 10, y});
            pSB->drawRect(nullptr, Rect(frameX + 10, y + 15, frameSize - 40, 1), Color(0.25f));
            for (int k = 0; k <= 16; ++k)
            {
                pSB->drawRect(nullptr, Rect(frameX + 10 + (frameSize - 40) * (float)k / 16, y + 15, 1, 10), Color(0.25f));
            }
            pSB->drawCross(Vector2(frameX + 10 + (frameSize - 40) * (value * 0.5f + 0.5f), y + 20), 5, Color(1, 1, 0));
        }

        // Buttons
        Vector2 pos(10, 60 + (float)axisCount * 40);
        auto buttonCount = pJoystick->getButtonCount();
        for (int j = 0; j < buttonCount; ++j)
        {
            pSB->drawOutterOutlineRect(Rect(frameX + pos.x + 4, pos.y + 4, 96.0f - 8, 32.0f - 8), 1, Color(0.25f));
            auto pressed = pJoystick->isPressed(j);
            if (pressed)
            {
                pSB->drawRect(nullptr, Rect(frameX + pos.x + 6, pos.y + 6, 96.0f - 12, 32.0f - 12), Color(0.5f));
            }
            pSB->drawText(pFont, "^990" + OGetJoystickButtonName(j, i), {frameX + pos.x + 48, pos.y + 16}, 
                          OCenter, pressed ? Color(1, 1, 0) : Color(0.5f));

            pos.x += 96.0f;
            if (pos.x >= frameSize - 20 - 96.0f)
            {
                pos.y += 32.0f;
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
