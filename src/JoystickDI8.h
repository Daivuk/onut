#ifndef JOYSTICKDI8_H_INCLUDED
#define JOYSTICKDI8_H_INCLUDED

// onut
#include <onut/Joystick.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(JoystickDI8);

namespace onut
{
    class JoystickDI8 final : public Joystick
    {
    public:
        JoystickDI8(int index);
        ~JoystickDI8();

        void update() override;

        bool isPressed(int button) const override;
        bool isJustPressed(int button) const override;
        bool isJustReleased(int button) const override;
        float getAxis(int id) const override;
        int getAxisCount() const override;
        int getButtonCount() const override;
        int getHatButtonBase() const override;
        int getAxisButtonBase() const override;
    };
}

#endif
