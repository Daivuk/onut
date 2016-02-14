#pragma once
#if defined(WIN32)
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#endif

#include "onut/ForwardDeclaration.h"
OForwardDeclare(Input);
OForwardDeclare(InputDevice);

namespace onut
{
    class Window;

    class InputDevice final
    {
    public:
        static OInputDeviceRef create(OInput* pInput, onut::Window* pWindow);

        InputDevice(OInput* pInput, onut::Window* pWindow);
        ~InputDevice();

        void update();

    private:
        void readKeyboard();
        void readMouse();

#if defined(WIN32)
        IDirectInput8* directInput = nullptr;
        IDirectInputDevice8* keyboard = nullptr;
        IDirectInputDevice8* mouse = nullptr;

        unsigned char keyboardState[256];
        unsigned char previousKeyboardState[256];

        DIMOUSESTATE mouseState;
        DIMOUSESTATE previousMouseState;
#endif

        OInput* m_pInput;
    };
}

extern OInputDeviceRef oInputDevice;
