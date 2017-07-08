// Oak Nut include
#include <onut/Input.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>

void initSettings()
{
    oSettings->setGameName("Cursor Sample");
    oSettings->setIsResizableWindow(true);
}

void init()
{
}

void update()
{
    const auto& mousePos = oInput->mousePosf;

    if (Rect(0, 0, 100, 100).Contains(mousePos))
    {
        oInput->setMouseIcon("green.png", Point(15, 10));
    }
    else if (Rect(100, 0, 100, 100).Contains(mousePos))
    {
        oInput->setMouseIcon("sword.png", Point(8, 8));
    }
    else if (Rect(200, 0, 100, 100).Contains(mousePos))
    {
        oInput->setMouseIcon("hand.png", Point(11, 11));
    }
    else if (Rect(300, 0, 100, 100).Contains(mousePos))
    {
        oInput->setMouseIcon("lemon.png", Point(8, 10));
    }
    else
    {
        oInput->unsetMouseIcon();
    }

    if (Rect(100, 200, 200, 100).Contains(mousePos))
    {
        oInput->setMouseVisible(false);
    }
    else
    {
        oInput->setMouseVisible(true);
    }
}

void render()
{
    // Clear
    oRenderer->clear(Color::Black);

    // Begin a batch
    oSpriteBatch->begin();

    // Draw cursor test zones
    oSpriteBatch->drawOutterOutlineRect(Rect(0, 0, 100, 100), 1, Color(1, 1, 0));
    oSpriteBatch->drawOutterOutlineRect(Rect(100, 0, 100, 100), 1, Color(1, 1, 0));
    oSpriteBatch->drawOutterOutlineRect(Rect(200, 0, 100, 100), 1, Color(1, 1, 0));
    oSpriteBatch->drawOutterOutlineRect(Rect(300, 0, 100, 100), 1, Color(1, 1, 0));
    oSpriteBatch->drawOutterOutlineRect(Rect(100, 200, 200, 100), 1, Color(1, 1, 0));

    // End and flush the batch
    oSpriteBatch->end();
}

void postRender()
{
}
