// Onut
#include <onut/Files.h>
#include <onut/Input.h>
#include <onut/Log.h>

// Private
#include "InputDeviceOSX.h"

// STL
#include <cassert>
#include <fcntl.h>
#include <unistd.h>

namespace onut
{
    OInputDeviceRef InputDevice::create(OInput* pInput)
    {
        return std::shared_ptr<InputDeviceOSX>(new InputDeviceOSX(pInput));
    }

    InputDeviceOSX::InputDeviceOSX(OInput* pInput)
        : InputDevice(pInput)
    {
    }

    InputDeviceOSX::~InputDeviceOSX()
    {
    }

    void InputDeviceOSX::readKeyboard()
    {
    }

    void InputDeviceOSX::readMouse()
    {
    }
}
