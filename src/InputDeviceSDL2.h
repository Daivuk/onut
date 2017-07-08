#ifndef INPUTDEVICESDL2_H_INCLUDED
#define INPUTDEVICESDL2_H_INCLUDED

// Onut
#include <onut/Timer.h>

// Internal
#include "InputDevice.h"

// Third parties
#include <SDL2/SDL.h>

// STL
#include <vector>
#include <unordered_map>

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

        void setMouseVisible(bool isCursorVisible) override;
        void setMouseIcon(const std::string& name, const Point& hotSpot) override;
        void unsetMouseIcon() override;

    private:
        void readKeyboard() override;
        void readMouse() override;

        std::vector<Uint8> m_previousKeyboardState;
        std::vector<Uint8> m_keyboardState;
        Uint8 m_previousMouseState[3];
        Uint8 m_mouseState[3];

        bool bSwaped = true;
        
        std::unordered_map<std::string, SDL_Cursor*> m_cursors;
        SDL_Cursor* m_pArrowCursor = nullptr;
    };
}

#endif
