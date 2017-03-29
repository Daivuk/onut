#ifndef INPUTDEVICESDL2_H_INCLUDED
#define INPUTDEVICESDL2_H_INCLUDED

// Onut
#include <onut/Timer.h>

// Internal
#include "InputDevice.h"

// Third parties
#include <SDL.h>

// STL
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Input);
OForwardDeclare(InputDevice);

namespace onut
{
    class InputDeviceSDL2 final : public InputDevice
    {
    public:
        InputDeviceSDL2(OInput* pInput);
        ~InputDeviceSDL2();

        void updateSDL2();

    private:
        void readKeyboard() override;
        void readMouse() override;

        std::vector<Uint8> m_previousKeyboardState;
        std::vector<Uint8> m_keyboardState;
        Uint8 m_previousMouseState[3];
        Uint8 m_mouseState[3];

        bool bSwaped = true;
    };
}

#endif
