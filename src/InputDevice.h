#pragma once
// Third party
#if defined(WIN32)
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#endif

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Input);
OForwardDeclare(InputDevice);
OForwardDeclare(Window);

namespace onut
{
    class Window;

    class InputDevice final
    {
    public:
        static OInputDeviceRef create(OInput* pInput, const OWindowRef& pWindow);

        InputDevice(OInput* pInput, const OWindowRef& pWindow);
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
