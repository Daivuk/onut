// Internal
#include "GamePadRPI.h"

namespace onut
{
    OGamePadRef GamePad::create(int index)
    {
        return std::shared_ptr<GamePadRPI>(new GamePadRPI(index));
    }

    GamePadRPI::GamePadRPI(int index)
        : GamePad(index)
    {
    }

    GamePadRPI::~GamePadRPI()
    {
    }

    void GamePadRPI::update()
    {
    }

    bool GamePadRPI::isConnected() const
    {
        return false;
    }

    bool GamePadRPI::isPressed(Button button) const
    {
        return false;
    }

    bool GamePadRPI::isJustPressed(Button button) const
    {
        return false;
    }

    bool GamePadRPI::isJustReleased(Button button) const
    {
        return false;
    }
}
