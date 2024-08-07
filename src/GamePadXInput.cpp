// Internal
#include "GamePadXInput.h"


#define GAMEPAD_LEFT_THUMB_PRESSED_DEADZONE  (7849 * 3)
#define GAMEPAD_RIGHT_THUMB_PRESSED_DEADZONE (8689 * 3)


namespace onut
{
    OGamePadRef GamePad::create(int index)
    {
        return std::shared_ptr<GamePadXInput>(new GamePadXInput(index));
    }

    GamePadXInput::GamePadXInput(int index)
        : GamePad(index)
    {
        memset(&m_currentState, 0, sizeof(m_currentState));
        memset(&m_previousState, 0, sizeof(m_previousState));
    }

    GamePadXInput::~GamePadXInput()
    {
    }

    void GamePadXInput::update()
    {
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
    }

    bool GamePadXInput::isConnected() const
    {
        return m_isConnected;
    }

    bool GamePadXInput::isPressed(Button button) const
    {
        return isPressed(button, m_currentState);
    }

    bool GamePadXInput::isJustPressed(Button button) const
    {
        return !isPressed(button, m_previousState) && isPressed(button, m_currentState);
    }

    bool GamePadXInput::isJustReleased(Button button) const
    {
        return isPressed(button, m_previousState) && !isPressed(button, m_currentState);
    }

    bool GamePadXInput::isPressed(Button button, const XINPUT_STATE& state) const
    {
        // Thumb directional presses
        bool leftThumbDirs[4] = { false };
        bool rightThumbDirs[4] = { false };

        const float leftLen = m_cachedLeftThumb.LengthSquared();
        const float rightLen = m_cachedRightThumb.LengthSquared();

        const float leftDeadZone = (float)GAMEPAD_LEFT_THUMB_PRESSED_DEADZONE / (float)std::numeric_limits<short>::max();
        const float rightDeadZone = (float)GAMEPAD_RIGHT_THUMB_PRESSED_DEADZONE / (float)std::numeric_limits<short>::max();

        if (leftLen >= leftDeadZone * leftDeadZone)
        {
            auto angle = OConvertToDegrees(Vector2::Angle(Vector2::Zero, m_cachedLeftThumb));
            leftThumbDirs[0] = angle < -22.5f && angle > -157.5f;
            leftThumbDirs[1] = angle > -67.5f && angle < 67.5f;
            leftThumbDirs[2] = angle > 22.5f && angle < 157.5f;
            leftThumbDirs[3] = angle > 112.5 || angle < -112.5f;
        }

        if (rightLen >= rightDeadZone * rightDeadZone)
        {
            auto angle = OConvertToDegrees(Vector2::Angle(Vector2::Zero, m_cachedRightThumb));
            rightThumbDirs[0] = angle < -22.5f && angle > -157.5f;
            rightThumbDirs[1] = angle > -67.5f && angle < 67.5f;
            rightThumbDirs[2] = angle > 22.5f && angle < 157.5f;
            rightThumbDirs[3] = angle > 112.5 || angle < -112.5f;
        }

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
                return leftThumbDirs[3];
            case LeftThumbStickRight:
                return leftThumbDirs[1];
            case LeftThumbStickUp:
                return leftThumbDirs[0];
            case LeftThumbStickDown:
                return leftThumbDirs[2];
            case RightThumbStickLeft:
                return rightThumbDirs[3];
            case RightThumbStickRight:
                return rightThumbDirs[1];
            case RightThumbStickUp:
                return rightThumbDirs[0];
            case RightThumbStickDown:
                return rightThumbDirs[2];

            // Old way, can't do diagonal
            //case LeftThumbStickLeft:
            //    return (state.Gamepad.sThumbLX <= -GAMEPAD_LEFT_THUMB_PRESSED_DEADZONE);
            //case LeftThumbStickRight:
            //    return (state.Gamepad.sThumbLX >= GAMEPAD_LEFT_THUMB_PRESSED_DEADZONE);
            //case LeftThumbStickUp:
            //    return (state.Gamepad.sThumbLY >= GAMEPAD_LEFT_THUMB_PRESSED_DEADZONE);
            //case LeftThumbStickDown:
            //    return (state.Gamepad.sThumbLY <= -GAMEPAD_LEFT_THUMB_PRESSED_DEADZONE);
            //case RightThumbStickLeft:
            //    return (state.Gamepad.sThumbRX <= -GAMEPAD_RIGHT_THUMB_PRESSED_DEADZONE);
            //case RightThumbStickRight:
            //    return (state.Gamepad.sThumbRX >= GAMEPAD_RIGHT_THUMB_PRESSED_DEADZONE);
            //case RightThumbStickUp:
            //    return (state.Gamepad.sThumbRY >= GAMEPAD_RIGHT_THUMB_PRESSED_DEADZONE);
            //case RightThumbStickDown:
            //    return (state.Gamepad.sThumbRY <= -GAMEPAD_RIGHT_THUMB_PRESSED_DEADZONE);
        }
        return false;
    }

    const char* GamePad::getButtonName(Button button)
    {
        switch (button)
        {
            case Button::A: return "A";
            case Button::B: return "B";
            case Button::X: return "X";
            case Button::Y: return "Y";
            case Button::DPadUp: return "DPad Up";
            case Button::DPadDown: return "DPad Down";
            case Button::DPadLeft: return "DPad Left";
            case Button::DPadRight: return "DPad Right";
            case Button::LeftTrigger: return "Left Trigger";
            case Button::LeftBumper: return "Left Bumper";
            case Button::RightTrigger: return "Right Trigger";
            case Button::RightBumper: return "Right Bumper";
            case Button::LeftThumbStick: return "Left Stick";
            case Button::RightThumbStick: return "Right Stick";
            case Button::Start: return "Start";
            case Button::Back: return "Back";
            case Button::LeftThumbStickLeft: return "Left Stick Left";
            case Button::LeftThumbStickRight: return "Left Stick Right";
            case Button::LeftThumbStickUp: return "Left Stick Up";
            case Button::LeftThumbStickDown: return "Left Stick Down";
            case Button::RightThumbStickLeft: return "Right Stick Left";
            case Button::RightThumbStickRight: return "Right Stick Right";
            case Button::RightThumbStickUp: return "Right Stick Up";
            case Button::RightThumbStickDown: return "Right Stick Down";
        }
        return "UNKNOWN";
    }
}
