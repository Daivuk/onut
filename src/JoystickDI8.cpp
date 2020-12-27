// Internal
#include "JoystickDI8.h"
#include <onut/Log.h>

namespace onut
{
    OJoystickRef Joystick::create(int index)
    {
        return std::shared_ptr<JoystickDI8>(new JoystickDI8(index));
    }

    JoystickDI8::JoystickDI8(int index)
        : Joystick(index)
    {
    }

    JoystickDI8::~JoystickDI8()
    {
    }

    void JoystickDI8::update()
    {
    }

    bool JoystickDI8::isPressed(int button) const
    {
        return false;
    }

    bool JoystickDI8::isJustPressed(int button) const
    {
        return false;
    }

    bool JoystickDI8::isJustReleased(int button) const
    {
        return false;
    }

    float JoystickDI8::getAxis(int id) const
    {
        return 0.f;
    }

    int JoystickDI8::getAxisCount() const
    {
        return 0;
    }

    int JoystickDI8::getButtonCount() const
    {
        return 0;
    }

    int JoystickDI8::getHatButtonBase() const
    {
        return 0;
    }

    int JoystickDI8::getAxisButtonBase() const
    {
        return 0;
    }
}
