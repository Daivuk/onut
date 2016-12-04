#ifndef GAMEPADLINUX_H_INCLUDED
#define GAMEPADLINUX_H_INCLUDED

#if defined(__unix__)
// onut
#include <onut/GamePad.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(GamePadLinux);

namespace onut
{
    class GamePadLinux final : public GamePad
    {
    public:
        GamePadLinux(int index);
        ~GamePadLinux();

        void update() override;
        bool isConnected() const override;
        bool isPressed(Button button) const override;
        bool isJustPressed(Button button) const override;
        bool isJustReleased(Button button) const override;

    private:
    };
}

#endif

#endif
