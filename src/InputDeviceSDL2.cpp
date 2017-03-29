// Onut
#include <onut/Files.h>
#include <onut/Input.h>
#include <onut/Log.h>

// Private
#include "InputDeviceSDL2.h"

// STL
#include <cassert>
#include <fcntl.h>
#include <unistd.h>

namespace onut
{
    OInputDeviceRef InputDevice::create(OInput* pInput)
    {
        return std::shared_ptr<InputDeviceSDL2>(new InputDeviceSDL2(pInput));
    }

    InputDeviceSDL2::InputDeviceSDL2(OInput* pInput)
        : InputDevice(pInput)
    {
    }

    InputDeviceSDL2::~InputDeviceSDL2()
    {
    }

    void InputDeviceSDL2::readKeyboard()
    {
        int keyCount = (int)m_keyboardState.size();
        for (int i = 0; i < keyCount; ++i)
        {
            auto isDown = m_keyboardState[i];
            auto isPreviousDown = m_previousKeyboardState[i];
            if (!isPreviousDown && isDown)
            {
                m_pInput->setStateDown(static_cast<onut::Input::State>(i));
            }
            else if (isPreviousDown && !isDown)
            {
                m_pInput->setStateUp(static_cast<onut::Input::State>(i));
            }
        }

        memcpy(m_previousKeyboardState.data(), m_keyboardState.data(), sizeof(Uint8) * keyCount);
        bSwaped = true;
    }

    void InputDeviceSDL2::readMouse()
    {
    }

    void InputDeviceSDL2::updateSDL2()
    {
        int keyCount = 0;
        auto pSDLStates = SDL_GetKeyboardState(&keyCount);

        m_previousKeyboardState.resize(keyCount, 0);
        m_keyboardState.resize(keyCount, 0);

        for (int i = 0; i < keyCount; ++i)
        {
            auto sdlState = pSDLStates[i];
            if (sdlState || bSwaped)
            {
                m_keyboardState[i] = sdlState;
            }
        }

        bSwaped = false;
    }
}
