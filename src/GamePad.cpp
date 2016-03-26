// Onut
#include <onut/GamePad.h>
#include <onut/Input.h>

namespace onut
{
    OGamePadRef GamePad::create(int index)
    {
        return std::make_shared<OGamePad>(index);
    }

    GamePad::GamePad(int index) :
        m_index(index)
    {
#if defined(WIN32)
        memset(&m_currentState, 0, sizeof(m_currentState));
        memset(&m_previousState, 0, sizeof(m_previousState));
#endif
    }

    void GamePad::update()
    {
#if defined(WIN32)
        m_previousState = m_currentState;
        memset(&m_currentState, 0, sizeof(m_currentState));
        auto result = XInputGetState(m_index, &m_currentState);
        m_isConnected = result == ERROR_SUCCESS;

        // Update thumbs
        {
            static float deadZone = static_cast<float>(static_cast<double>(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) / 32768.0);

            m_cachedLeftThumb = {
                static_cast<float>(static_cast<double>(m_currentState.Gamepad.sThumbLX) / 32768.0),
                -static_cast<float>(static_cast<double>(m_currentState.Gamepad.sThumbLY) / 32768.0)};

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
                static_cast<float>(static_cast<double>(m_currentState.Gamepad.sThumbRX) / 32768.0),
                -static_cast<float>(static_cast<double>(m_currentState.Gamepad.sThumbRY) / 32768.0)};

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
#endif
    }

    bool GamePad::isConnected() const
    {
        return m_isConnected;
    }

    bool GamePad::isPressed(Button button) const
    {
        return isPressed(button, m_currentState);
    }

    bool GamePad::isPressed(Button button, const XINPUT_STATE& state) const
    {
#if defined(WIN32)
        switch (button)
        {
            case A:
                return (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) ? true : false;
            case B:
                return (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) ? true : false;
            case X:
                return (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) ? true : false;
            case Y:
                return (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) ? true : false;
            case DPadUp:
                return (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) ? true : false;
            case DPadDown:
                return (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? true : false;
            case DPadLeft:
                return (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? true : false;
            case DPadRight:
                return (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? true : false;
            case LeftTrigger:
                return (state.Gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
            case LeftBumper:
                return (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? true : false;
            case RightTrigger:
                return (state.Gamepad.bRightTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
            case RightBumper:
                return (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? true : false;
            case LeftThumbStick:
                return (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? true : false;
            case RightThumbStick:
                return (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) ? true : false;
            case Start:
                return (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) ? true : false;
            case Back:
                return (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) ? true : false;
            case LeftThumbStickLeft:
                return (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
            case LeftThumbStickRight:
                return (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
            case LeftThumbStickUp:
                return (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
            case LeftThumbStickDown:
                return (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
            case RightThumbStickLeft:
                return (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
            case RightThumbStickRight:
                return (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
            case RightThumbStickUp:
                return (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
            case RightThumbStickDown:
                return (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
        }
#endif
        return false;
    }

    bool GamePad::isJustPressed(Button button) const
    {
        return !isPressed(button, m_previousState) && isPressed(button, m_currentState);
    }

    bool GamePad::isJustReleased(Button button) const
    {
        return isPressed(button, m_previousState) && !isPressed(button, m_currentState);
    }
}

const OGamePadRef& OGetGamePad(int index)
{
    return oInput->getGamePad(index);
}

bool OGamePadPressed(onut::GamePad::Button button, int gamePadIndex)
{
    return OGetGamePad(gamePadIndex)->isPressed(button);
}

bool OGamePadJustPressed(onut::GamePad::Button button, int gamePadIndex)
{
    return OGetGamePad(gamePadIndex)->isJustPressed(button);
}

bool OGamePadJustReleased(onut::GamePad::Button button, int gamePadIndex)
{
    return OGetGamePad(gamePadIndex)->isJustReleased(button);
}

const Vector2& OGetGamePadLeftThumb(int gamePadIndex)
{
    return OGetGamePad(gamePadIndex)->getLeftThumb();
}

const Vector2& OGetGamePadRightThumb(int gamePadIndex)
{
    return OGetGamePad(gamePadIndex)->getRightThumb();
}
