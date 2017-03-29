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

        std::vector<int> m_devices;
        char m_keyMap[KEY_MAX / 8 + 1];
        bool m_states[KEY_MAX];
        bool m_previousStates[KEY_MAX];

        OTimer m_initTimer;
    };
}

#endif

#endif
