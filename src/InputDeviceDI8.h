#pragma once
#if defined(WIN32)
// Internal
#include "InputDevice.h"

// Third party
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Input);
OForwardDeclare(InputDevice);
OForwardDeclare(Window);

namespace onut
{
    class InputDeviceDI8 final : public InputDevice
    {
    public:
        InputDeviceDI8(OInput* pInput);
        ~InputDeviceDI8();

    private:
        void readKeyboard() override;
        void readMouse() override;

        IDirectInput8* directInput = nullptr;
        IDirectInputDevice8* keyboard = nullptr;
        IDirectInputDevice8* mouse = nullptr;

        unsigned char keyboardState[256];
        unsigned char previousKeyboardState[256];

        DIMOUSESTATE mouseState;
        DIMOUSESTATE previousMouseState;
    };
}

extern OInputDeviceRef oInputDevice;
#endif
