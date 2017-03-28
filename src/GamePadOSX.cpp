// Internal
#include "GamePadOSX.h"

namespace onut
{
    OGamePadRef GamePad::create(int index)
    {
        return std::shared_ptr<GamePadOSX>(new GamePadOSX(index));
    }

    GamePadOSX::GamePadOSX(int index)
        : GamePad(index)
    {
    }

    GamePadOSX::~GamePadOSX()
    {
    }

    void GamePadOSX::update()
    {
    }

    bool GamePadOSX::isConnected() const
    {
        return false;
    }

    bool GamePadOSX::isPressed(Button button) const
    {
        return false;
    }

    bool GamePadOSX::isJustPressed(Button button) const
    {
        return false;
    }

    bool GamePadOSX::isJustReleased(Button button) const
    {
        return false;
    }
}
