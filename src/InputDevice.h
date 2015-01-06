#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace onut
{
    class Window;

    class InputDevice
    {
    public:
        InputDevice(onut::Window* pWindow);
        virtual ~InputDevice();

        void update();

    private:
        void readKeyboard();
        void readMouse();

        IDirectInput8*			directInput = nullptr;
        IDirectInputDevice8*	keyboard = nullptr;
        IDirectInputDevice8*	mouse = nullptr;

        unsigned char			keyboardState[256];
        unsigned char			previousKeyboardState[256];

        DIMOUSESTATE			mouseState;
        DIMOUSESTATE			previousMouseState;
    };
}
