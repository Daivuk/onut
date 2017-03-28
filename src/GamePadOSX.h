#ifndef GAMEPADOSX_H_INCLUDED
#define GAMEPADOSX_H_INCLUDED

// onut
#include <onut/GamePad.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(GamePadOSX);

namespace onut
{
    class GamePadOSX final : public GamePad
    {
    public:
        GamePadOSX(int index);
        ~GamePadOSX();

        void update() override;
        bool isConnected() const override;
        bool isPressed(Button button) const override;
        bool isJustPressed(Button button) const override;
        bool isJustReleased(Button button) const override;

    private:
    };
}

#endif
