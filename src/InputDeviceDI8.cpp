// Onut
#include <onut/ContentManager.h>
#include <onut/Input.h>
#include <onut/Images.h>
#include <onut/Window.h>

// Private
#include "InputDeviceDI8.h"

#define FAIL_DI_WITH_ERROR(__errorMsg__) {MessageBoxA(NULL, __errorMsg__, "DirectInput8 Error", MB_OK); exit(0);}

namespace onut
{
    OInputDeviceRef InputDevice::create(OInput* pInput)
    {
        return std::shared_ptr<InputDeviceDI8>(new InputDeviceDI8(pInput));
    }

    InputDeviceDI8::InputDeviceDI8(OInput* pInput)
        : InputDevice(pInput)
    {
        unsetMouseIcon();
        HRESULT result;

        // Initialize DirectInput
        // Initialize the main direct input interface.
        result = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
        if (FAILED(result))
        {
            FAIL_DI_WITH_ERROR("DirectInput8Create");
        }

        // Initialize the direct input interface for the keyboard.
        result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
        if (FAILED(result))
        {
            FAIL_DI_WITH_ERROR("CreateDevice GUID_SysKeyboard");
        }

        // Set the data format.  In this case since it is a keyboard we can use the predefined data format.
        result = keyboard->SetDataFormat(&c_dfDIKeyboard);
        if (FAILED(result))
        {
            FAIL_DI_WITH_ERROR("keyboard SetDataFormat");
        }

        // Set the cooperative level of the keyboard to not share with other programs.
        result = keyboard->SetCooperativeLevel(oWindow->getHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
        if (FAILED(result))
        {
            FAIL_DI_WITH_ERROR("keyboard SetCooperativeLevel");
        }

        // Initialize the direct input interface for the mouse.
        result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
        if (FAILED(result))
        {
            FAIL_DI_WITH_ERROR("CreateDevice GUID_SysMouse");
        }

        // Set the data format for the mouse using the pre-defined mouse data format.
        result = mouse->SetDataFormat(&c_dfDIMouse);
        if (FAILED(result))
        {
            FAIL_DI_WITH_ERROR("mouse SetDataFormat");
        }

        // Set the cooperative level of the mouse to share with other programs.
        result = mouse->SetCooperativeLevel(oWindow->getHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
        if (FAILED(result))
        {
            FAIL_DI_WITH_ERROR("mouse SetCooperativeLevel");
        }

        memset(&mouseState, 0, sizeof(DIMOUSESTATE));
        memset(keyboardState, 0, 256);
        memset(previousKeyboardState, 0, 256);
    }

    InputDeviceDI8::~InputDeviceDI8()
    {
        if (mouse) mouse->Release();
        if (keyboard) keyboard->Release();
        if (directInput) directInput->Release();
    }

    void InputDeviceDI8::readKeyboard()
    {
        HRESULT result;

        // Read the keyboard device.
        result = keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
        if (FAILED(result))
        {
            // If the keyboard lost focus or was not acquired then try to get control back.
            if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
            {
                result = keyboard->Acquire();
                if (FAILED(result))
                {
                    return;
                }
            }
            else
            {
                return;
            }
        }

        for (int i = 0; i < 256; ++i)
        {
            if (!(previousKeyboardState[i] & 0x80) && (keyboardState[i] & 0x80))
            {
                m_pInput->setStateDown(static_cast<onut::Input::State>(i));
            }
            else if ((previousKeyboardState[i] & 0x80) && !(keyboardState[i] & 0x80))
            {
                m_pInput->setStateUp(static_cast<onut::Input::State>(i));
            }
        }
        memcpy(previousKeyboardState, keyboardState, 256);
    }

    void InputDeviceDI8::readMouse()
    {
        HRESULT result;

        // Read the mouse device.
        result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
        if (FAILED(result))
        {
            // If the mouse lost focus or was not acquired then try to get control back.
            if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
            {
                result = mouse->Acquire();
                if (FAILED(result))
                {
                    return;
                }
            }
            else
            {
                return;
            }
        }

        for (int i = 0; i < 4; ++i)
        {
            if (!(previousMouseState.rgbButtons[i] & 0x80) && (mouseState.rgbButtons[i] & 0x80))
            {
                m_pInput->setStateDown(static_cast<onut::Input::State>(static_cast<int>(OMouse1)+i));
            }
            else if ((previousMouseState.rgbButtons[i] & 0x80) && !(mouseState.rgbButtons[i] & 0x80))
            {
                m_pInput->setStateUp(static_cast<onut::Input::State>(static_cast<int>(OMouse1)+i));
            }
        }
        memcpy(&previousMouseState, &mouseState, sizeof(DIMOUSESTATE));
        m_pInput->setStateValue(OMouseX, (float)mouseState.lX);
        m_pInput->setStateValue(OMouseY, (float)mouseState.lY);
        m_pInput->setStateValue(OMouseZ, (float)mouseState.lZ);
    }

    void InputDeviceDI8::setMouseVisible(bool isCursorVisible)
    {
        ShowCursor(isCursorVisible ? TRUE : FALSE);
    }

    void InputDeviceDI8::setMouseIcon(const std::string& name, const Point& hotSpot)
    {
        auto it = m_cursors.find(name);
        if (it == m_cursors.end())
        {
            auto fullPath = oContentManager->findResourceFile(name);
            if (!fullPath.empty())
            {
                auto hCursor = onut::pngToCursor(fullPath, hotSpot);
                if (hCursor)
                {
                    m_cursors[name] = hCursor;
                    m_cursor = hCursor;
                    SetCursor(hCursor);
                }
            }
        }
        else
        {
            m_cursor = it->second;
            SetCursor(it->second);
        }
    }

    void InputDeviceDI8::unsetMouseIcon()
    {
        if (m_cursor)
        {
            m_cursor = NULL;
            SetCursor(LoadCursor(nullptr, IDC_ARROW));
        }
    }

    HCURSOR InputDeviceDI8::getCursor() const
    {
        return m_cursor;
    }

    void InputDeviceDI8::setCursor(HCURSOR cursor)
    {
        m_cursor = cursor;
        SetCursor(cursor);
    }
}
