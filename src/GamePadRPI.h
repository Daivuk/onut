#ifndef GAMEPADRPI_H_INCLUDED
#define GAMEPADRPI_H_INCLUDED

// onut
#include <onut/GamePad.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(GamePadRPI);

namespace onut
{
    class GamePadRPI final : public GamePad
    {
    public:
        GamePadRPI(int index);
        ~GamePadRPI();

        void update() override;
        bool isConnected() const override;
        bool isPressed(Button button) const override;
        bool isJustPressed(Button button) const override;
        bool isJustReleased(Button button) const override;

    private:
    };
}

#endif
