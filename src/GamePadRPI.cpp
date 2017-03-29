// Internal
#include "GamePadLinux.h"

namespace onut
{
    OGamePadRef GamePad::create(int index)
    {
        return std::shared_ptr<GamePadLinux>(new GamePadLinux(index));
    }

    GamePadLinux::GamePadLinux(int index)
        : GamePad(index)
    {
    }

    GamePadLinux::~GamePadLinux()
    {
    }

    void GamePadLinux::update()
    {
    }

    bool GamePadLinux::isConnected() const
    {
        return false;
    }

    bool GamePadLinux::isPressed(Button button) const
    {
        return false;
    }

    bool GamePadLinux::isJustPressed(Button button) const
    {
        return false;
    }

    bool GamePadLinux::isJustReleased(Button button) const
    {
        return false;
    }
}
