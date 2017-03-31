#ifndef GAMEPADSDL2_H_INCLUDED
#define GAMEPADSDL2_H_INCLUDED

// onut
#include <onut/GamePad.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(GamePadSDL2);

namespace onut
{
    class GamePadSDL2 final : public GamePad
    {
    public:
        GamePadSDL2(int index);
        ~GamePadSDL2();

        void update() override;
        bool isConnected() const override;
        bool isPressed(Button button) const override;
        bool isJustPressed(Button button) const override;
        bool isJustReleased(Button button) const override;

    private:
    };
}

#endif
