// Internal
#include "GamePadSDL2.h"

namespace onut
{
    OGamePadRef GamePad::create(int index)
    {
        return std::shared_ptr<GamePadSDL2>(new GamePadSDL2(index));
    }

    GamePadSDL2::GamePadSDL2(int index)
        : GamePad(index)
    {
    }

    GamePadSDL2::~GamePadSDL2()
    {
    }

    void GamePadSDL2::update()
    {
    }

    bool GamePadSDL2::isConnected() const
    {
        return false;
    }

    bool GamePadSDL2::isPressed(Button button) const
    {
        return false;
    }

    bool GamePadSDL2::isJustPressed(Button button) const
    {
        return false;
    }

    bool GamePadSDL2::isJustReleased(Button button) const
    {
        return false;
    }
}
