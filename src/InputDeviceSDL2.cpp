// Onut
#include <onut/ContentManager.h>
#include <onut/Files.h>
#include <onut/Input.h>
#include <onut/Images.h>
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
        m_mouseState[0] = 0;
        m_mouseState[1] = 1;
        m_mouseState[2] = 2;
        m_previousMouseState[0] = 0;
        m_previousMouseState[1] = 1;
        m_previousMouseState[2] = 2;

        SDL_Init(SDL_INIT_GAMECONTROLLER);

        m_pArrowCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    }

    InputDeviceSDL2::~InputDeviceSDL2()
    {
        for (const auto& kv : m_cursors)
        {
            SDL_FreeCursor(kv.second);
        }
        SDL_FreeCursor(m_pArrowCursor);
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
        if (!m_previousMouseState[0] && m_mouseState[0])
        {
            m_pInput->setStateDown(OMouse1);
        }
        else if (m_previousMouseState[0] && !m_mouseState[0])
        {
            m_pInput->setStateUp(OMouse1);
        }
        if (!m_previousMouseState[1] && m_mouseState[1])
        {
            m_pInput->setStateDown(OMouse2);
        }
        else if (m_previousMouseState[1] && !m_mouseState[1])
        {
            m_pInput->setStateUp(OMouse2);
        }
        if (!m_previousMouseState[2] && m_mouseState[2])
        {
            m_pInput->setStateDown(OMouse3);
        }
        else if (m_previousMouseState[2] && !m_mouseState[2])
        {
            m_pInput->setStateUp(OMouse3);
        }

        memcpy(m_previousMouseState, m_mouseState, sizeof(Uint8) * 3);
        bSwaped = true;
    }

    void InputDeviceSDL2::updateSDL2()
    {
        // Keyboard
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

        // Mouse
        int x, y;
        auto mouseBtnStates = SDL_GetMouseState(&x, &y);

        m_pInput->mousePos.x = x;
        m_pInput->mousePos.y = y;
        m_pInput->mousePosf.x = static_cast<float>(x);
        m_pInput->mousePosf.y = static_cast<float>(y);

        m_pInput->setStateValue(OMouseX, (float)x);
        m_pInput->setStateValue(OMouseY, (float)y);

        bool mouseL =  (mouseBtnStates & SDL_BUTTON(SDL_BUTTON_LEFT)) ? true : false;
        bool mouseM =  (mouseBtnStates & SDL_BUTTON(SDL_BUTTON_MIDDLE)) ? true : false;
        bool mouseR =  (mouseBtnStates & SDL_BUTTON(SDL_BUTTON_RIGHT)) ? true : false;
        if (mouseL || bSwaped) m_mouseState[0] = mouseL ? 1 : 0;
        if (mouseR || bSwaped) m_mouseState[1] = mouseR ? 1 : 0;
        if (mouseM || bSwaped) m_mouseState[2] = mouseM ? 1 : 0;

        bSwaped = false;
    }

    void InputDeviceSDL2::setMouseVisible(bool isCursorVisible)
    {
        SDL_ShowCursor(isCursorVisible ? SDL_ENABLE : SDL_DISABLE);
    }

    void InputDeviceSDL2::setMouseIcon(const std::string& name, const Point& hotSpot)
    {
        auto it = m_cursors.find(name);
        if (it == m_cursors.end())
        {
            auto fullPath = oContentManager->findResourceFile(name);
            if (!fullPath.empty())
            {
                Point size;
                auto pngData = onut::loadPNG(fullPath, size);
                if (!pngData.empty())
                {
                    auto pSurface = SDL_CreateRGBSurfaceFrom(pngData.data(),
                                      size.x,
                                      size.y,
                                      32,
                                      size.x * 4,
                                      0x000000ff,
                                      0x0000ff00,
                                      0x00ff0000,
                                      0xff000000);
                    if (pSurface)
                    {
                        auto pCursor = SDL_CreateColorCursor(pSurface, hotSpot.x, hotSpot.y);
                        if (pCursor)
                        {
                            m_cursors[name] = pCursor;
                            SDL_SetCursor(pCursor);
                        }
                        SDL_FreeSurface(pSurface);
                    }
                }
            }
        }
        else
        {
            SDL_SetCursor(it->second);
        }
    }

    void InputDeviceSDL2::unsetMouseIcon()
    {
        SDL_SetCursor(m_pArrowCursor);
    }

    void InputDeviceSDL2::setMousePos(const Point& mousePos)
    {
        // ...
    }
}
