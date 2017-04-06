// Onut
#include <onut/GamePad.h>
#include <onut/Input.h>

namespace onut
{
    GamePad::GamePad(int index)
        : m_index(index)
    {
    }

    GamePad::~GamePad()
    {
    }
}

OGamePadRef OGetGamePad(int index)
{
    return oInput->getGamePad(index);
}

bool OGamePadPressed(onut::GamePad::Button button, int gamePadIndex)
{
    return OGetGamePad(gamePadIndex)->isPressed(button);
}

bool OGamePadJustPressed(onut::GamePad::Button button, int gamePadIndex)
{
    return OGetGamePad(gamePadIndex)->isJustPressed(button);
}

bool OGamePadJustReleased(onut::GamePad::Button button, int gamePadIndex)
{
    return OGetGamePad(gamePadIndex)->isJustReleased(button);
}

const Vector2& OGetGamePadLeftThumb(int gamePadIndex)
{
    return OGetGamePad(gamePadIndex)->getLeftThumb();
}

const Vector2& OGetGamePadRightThumb(int gamePadIndex)
{
    return OGetGamePad(gamePadIndex)->getRightThumb();
}
