#ifndef INPUTDEVICEDI8_H_INCLUDED
#define INPUTDEVICEDI8_H_INCLUDED

// Internal
#include "InputDevice.h"

// STL
#include <unordered_map>

// Third party
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Windows.h>

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

        void setCursorVisible(bool isCursorVisible) override;

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

        std::unordered_map<std::string, HCURSOR> cursors;
    };
}

extern OInputDeviceRef oInputDevice;
#endif
