// Internal
#include "GamePadSDL2.h"
#include <onut/Log.h>

// STL
#include <memory.h>

// We will use the official values from microsoft for dead zones
#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

namespace onut
{
    OGamePadRef GamePad::create(int index)
    {
        return std::shared_ptr<GamePadSDL2>(new GamePadSDL2(index));
    }

    GamePadSDL2::GamePadSDL2(int index)
        : GamePad(index)
    {
        m_previousState.resize(SDL_CONTROLLER_BUTTON_MAX, 0);
        m_state.resize(SDL_CONTROLLER_BUTTON_MAX, 0);
    }

    GamePadSDL2::~GamePadSDL2()
    {
        if (m_pSDLGamePad)
        {
            SDL_GameControllerClose(m_pSDLGamePad);
            m_pSDLGamePad = nullptr;
        }
    }

    void GamePadSDL2::update()
    {
        if (!m_pSDLGamePad) return;

        auto lx = SDL_GameControllerGetAxis(m_pSDLGamePad, SDL_CONTROLLER_AXIS_LEFTX);
        auto ly = SDL_GameControllerGetAxis(m_pSDLGamePad, SDL_CONTROLLER_AXIS_LEFTY);
        auto rx = SDL_GameControllerGetAxis(m_pSDLGamePad, SDL_CONTROLLER_AXIS_RIGHTX);
        auto ry = SDL_GameControllerGetAxis(m_pSDLGamePad, SDL_CONTROLLER_AXIS_RIGHTY);
        auto lt = SDL_GameControllerGetAxis(m_pSDLGamePad, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
        auto rt = SDL_GameControllerGetAxis(m_pSDLGamePad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

        // Update thumbs
        {
            static float deadZone = static_cast<float>(static_cast<double>(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) / 32768.0);

            m_cachedLeftThumb = {
                static_cast<float>(static_cast<double>(lx) / 32768.0),
                static_cast<float>(static_cast<double>(ly) / 32768.0)};

            float len = m_cachedLeftThumb.Length();
            if (len <= deadZone)
            {
                m_cachedLeftThumb = {};
            }
            else
            {
                float percent = (len - deadZone) / (1 - deadZone);
                m_cachedLeftThumb.Normalize();
                m_cachedLeftThumb *= percent;
            }
        }

        {
            static float deadZone = static_cast<float>(static_cast<double>(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) / 32768.0);

            m_cachedRightThumb = {
                static_cast<float>(static_cast<double>(rx) / 32768.0),
                static_cast<float>(static_cast<double>(ry) / 32768.0)};

            float len = m_cachedRightThumb.Length();
            if (len <= deadZone)
            {
                m_cachedRightThumb = {};
            }
            else
            {
                float percent = (len - deadZone) / (1 - deadZone);
                m_cachedRightThumb.Normalize();
                m_cachedRightThumb *= percent;
            }
        }

        // Update triggers
        {
            static float deadZone = static_cast<float>(static_cast<double>(XINPUT_GAMEPAD_TRIGGER_THRESHOLD) / 32768.0);

            m_lTrigger = (float)((double)lt / 32768.0);
            m_rTrigger = (float)((double)rt / 32768.0);

            if (m_lTrigger <= deadZone) m_lTrigger = 0;
            else
            {
                m_lTrigger = (m_lTrigger - deadZone) / (1 - deadZone);
            }
            if (m_rTrigger <= deadZone) m_rTrigger = 0;
            else
            {
                m_rTrigger = (m_rTrigger - deadZone) / (1 - deadZone);
            }
        }

        if (bSwaped)
        {
            memcpy(m_previousState.data(), m_state.data(), sizeof(Uint8) * SDL_CONTROLLER_BUTTON_MAX);
        }
        bSwaped = true;
    }

    void GamePadSDL2::updateSDL2()
    {
        if (!m_pSDLGamePad) return;

        if (bSwaped)
        {
            memcpy(m_previousState.data(), m_state.data(), sizeof(Uint8) * SDL_CONTROLLER_BUTTON_MAX);
        }

        for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
        {
            auto sdlState = SDL_GameControllerGetButton(m_pSDLGamePad, (SDL_GameControllerButton)i);
            if (sdlState || bSwaped)
            {
                m_state[i] = sdlState;
            }
        }

        bSwaped = false;
    }

    bool GamePadSDL2::isConnected() const
    {
        if (m_pSDLGamePad)
        {
            return SDL_GameControllerGetAttached(m_pSDLGamePad) == SDL_TRUE;
        }
        return false;
    }

    void GamePadSDL2::onAdded()
    {
        if (m_pSDLGamePad)
        {
            SDL_GameControllerClose(m_pSDLGamePad);
            m_pSDLGamePad = nullptr;
        }
        m_pSDLGamePad = SDL_GameControllerOpen(m_index);
    }

    void GamePadSDL2::onRemoved()
    {
        if (m_pSDLGamePad)
        {
            SDL_GameControllerClose(m_pSDLGamePad);
            m_pSDLGamePad = nullptr;
        }
    }

    void GamePadSDL2::setSDLController(SDL_GameController* pSDLController)
    {
        m_pSDLGamePad = pSDLController;
    }

    SDL_GameController* GamePadSDL2::getSDLController() const
    {
        return m_pSDLGamePad;
    }

    bool GamePadSDL2::isPressed(Button button) const
    {
        return isPressed(button, m_state);
    }

    bool GamePadSDL2::isJustPressed(Button button) const
    {
        return !isPressed(button, m_previousState) && isPressed(button, m_state);
    }

    bool GamePadSDL2::isJustReleased(Button button) const
    {
        return isPressed(button, m_previousState) && !isPressed(button, m_state);
    }

    bool GamePadSDL2::isPressed(Button button, const std::vector<Uint8>& state) const
    {
        switch (button)
        {
            case A:
                return (state[SDL_CONTROLLER_BUTTON_A]) ? true : false;
            case B:
                return (state[SDL_CONTROLLER_BUTTON_B]) ? true : false;
            case X:
                return (state[SDL_CONTROLLER_BUTTON_X]) ? true : false;
            case Y:
                return (state[SDL_CONTROLLER_BUTTON_Y]) ? true : false;
            case DPadUp:
                return (state[SDL_CONTROLLER_BUTTON_DPAD_UP]) ? true : false;
            case DPadDown:
                return (state[SDL_CONTROLLER_BUTTON_DPAD_DOWN]) ? true : false;
            case DPadLeft:
                return (state[SDL_CONTROLLER_BUTTON_DPAD_LEFT]) ? true : false;
            case DPadRight:
                return (state[SDL_CONTROLLER_BUTTON_DPAD_RIGHT]) ? true : false;
            case LeftTrigger:
                return (m_lTrigger > 0);
            case LeftBumper:
                return (state[SDL_CONTROLLER_BUTTON_LEFTSHOULDER]) ? true : false;
            case RightTrigger:
                return (m_rTrigger > 0);
            case RightBumper:
                return (state[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER]) ? true : false;
            case LeftThumbStick:
                return (state[SDL_CONTROLLER_BUTTON_LEFTSTICK]) ? true : false;
            case RightThumbStick:
                return (state[SDL_CONTROLLER_BUTTON_RIGHTSTICK]) ? true : false;
            case Start:
                return (state[SDL_CONTROLLER_BUTTON_START]) ? true : false;
            case Back:
                return (state[SDL_CONTROLLER_BUTTON_BACK]) ? true : false;
            case LeftThumbStickLeft:
                return (m_cachedLeftThumb.x < 0);
            case LeftThumbStickRight:
                return (m_cachedLeftThumb.x > 0);
            case LeftThumbStickUp:
                return (m_cachedLeftThumb.y > 0);
            case LeftThumbStickDown:
                return (m_cachedLeftThumb.y < 0);
            case RightThumbStickLeft:
                return (m_cachedRightThumb.x < 0);
            case RightThumbStickRight:
                return (m_cachedRightThumb.x > 0);
            case RightThumbStickUp:
                return (m_cachedRightThumb.y > 0);
            case RightThumbStickDown:
                return (m_cachedRightThumb.y < 0);
            default:
                return false;
        }
        return false;
    }
}
