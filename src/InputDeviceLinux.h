#ifndef INPUTDEVICELINUX_H_INCLUDED
#define INPUTDEVICELINUX_H_INCLUDED
#if defined(__unix__)

// Internal
#include "InputDevice.h"

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Input);
OForwardDeclare(InputDevice);

namespace onut
{
    class InputDeviceLinux final : public InputDevice
    {
    public:
        InputDeviceLinux(OInput* pInput);
        ~InputDeviceLinux();

    private:
        void readKeyboard() override;
        void readMouse() override;
    };
}

#endif

#endif
