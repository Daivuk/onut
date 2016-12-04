#if defined(__unix__)
// Onut
#include <onut/Input.h>

// Private
#include "InputDeviceLinux.h"

namespace onut
{
    OInputDeviceRef InputDevice::create(OInput* pInput)
    {
        return std::shared_ptr<InputDeviceLinux>(new InputDeviceLinux(pInput));
    }

    InputDeviceLinux::InputDeviceLinux(OInput* pInput)
        : InputDevice(pInput)
    {
    }

    InputDeviceLinux::~InputDeviceLinux()
    {
    }

    void InputDeviceLinux::readKeyboard()
    {
    }

    void InputDeviceLinux::readMouse()
    {
    }
}

#endif
