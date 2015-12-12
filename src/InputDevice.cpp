#include <thread>
#include "onut.h"
#include "InputDevice.h"
#include "Window.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


#define FAIL_DI_WITH_ERROR(__errorMsg__) {MessageBoxA(NULL, __errorMsg__, "DirectInput8 Error", MB_OK); exit(0);}

namespace onut
{
    InputDevice::InputDevice(onut::Window* pWindow)
    {
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
        result = keyboard->SetCooperativeLevel(pWindow->getHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
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
        result = mouse->SetCooperativeLevel(pWindow->getHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
        if (FAILED(result))
        {
            FAIL_DI_WITH_ERROR("mouse SetCooperativeLevel");
        }

        memset(&mouseState, 0, sizeof(DIMOUSESTATE));
        memset(keyboardState, 0, 256);
        memset(previousKeyboardState, 0, 256);
    }

    InputDevice::~InputDevice()
    {
        if (mouse) mouse->Release();
        if (keyboard) keyboard->Release();
        if (directInput) directInput->Release();
    }

    void InputDevice::update()
    {
        readKeyboard();
        readMouse();
    }

    void InputDevice::readKeyboard()
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
                OInput->setStateDown(i);
            }
            else if ((previousKeyboardState[i] & 0x80) && !(keyboardState[i] & 0x80))
            {
                OInput->setStateUp(i);
            }
        }
        memcpy(previousKeyboardState, keyboardState, 256);
    }

    void InputDevice::readMouse()
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
                OInput->setStateDown(OINPUT_MOUSEB1 + i);
            }
            else if ((previousMouseState.rgbButtons[i] & 0x80) && !(mouseState.rgbButtons[i] & 0x80))
            {
                OInput->setStateUp(OINPUT_MOUSEB1 + i);
            }
        }
        memcpy(&previousMouseState, &mouseState, sizeof(DIMOUSESTATE));
        OInput->setStateValue(OINPUT_MOUSEX, (float)mouseState.lX);
        OInput->setStateValue(OINPUT_MOUSEY, (float)mouseState.lY);
        OInput->setStateValue(OINPUT_MOUSEZ, (float)mouseState.lZ);
    }
}
