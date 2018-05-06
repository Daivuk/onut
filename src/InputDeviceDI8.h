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

        void setMouseVisible(bool isCursorVisible) override;
        void setMouseIcon(const std::string& name, const Point& hotSpot) override;
        void unsetMouseIcon() override;
        void setMousePos(const Point& mousePos) override;

        HCURSOR getCursor() const;
        void setCursor(HCURSOR cursor);

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

        HCURSOR m_cursor;
        std::unordered_map<std::string, HCURSOR> m_cursors;
    };
}

extern OInputDeviceRef oInputDevice;
#endif
