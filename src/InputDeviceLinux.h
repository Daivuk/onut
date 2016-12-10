#ifndef INPUTDEVICELINUX_H_INCLUDED
#define INPUTDEVICELINUX_H_INCLUDED
#if defined(__unix__)

// Onut
#include <onut/Timer.h>

// Internal
#include "InputDevice.h"

// Thirdparty
#include <linux/input.h>

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
        
        int m_fd = -1;
        char m_keyMap[KEY_MAX / 8 + 1];
        char m_previousKeyMap[KEY_MAX / 8 + 1];
        
        OTimer m_initTimer;
    };
}

#endif

#endif
