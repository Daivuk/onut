#ifndef INPUTDEVICEOSX_H_INCLUDED
#define INPUTDEVICEOSX_H_INCLUDED

// Onut
#include <onut/Timer.h>

// Internal
#include "InputDevice.h"

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Input);
OForwardDeclare(InputDevice);

namespace onut
{
    class InputDeviceOSX final : public InputDevice
    {
    public:
        InputDeviceOSX(OInput* pInput);
        ~InputDeviceOSX();

    private:
        void readKeyboard() override;
        void readMouse() override;
    };
}

#endif
