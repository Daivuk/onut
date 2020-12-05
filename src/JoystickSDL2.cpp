// Internal
#include "JoystickSDL2.h"
#include <onut/Log.h>

static const char* getSDLJoystickTypeName(SDL_JoystickType type)
{
    switch (type)
    {
        case SDL_JOYSTICK_TYPE_GAMECONTROLLER:
            return "Game Controller";
        case SDL_JOYSTICK_TYPE_WHEEL:
            return "Wheel";
        case SDL_JOYSTICK_TYPE_ARCADE_STICK:
            return "Arcade Stick";
        case SDL_JOYSTICK_TYPE_FLIGHT_STICK:
            return "Flight Stick";
        case SDL_JOYSTICK_TYPE_DANCE_PAD:
            return "Dance Pad";
        case SDL_JOYSTICK_TYPE_GUITAR:
            return "Guitar";
        case SDL_JOYSTICK_TYPE_DRUM_KIT:
            return "Drum Kit";
        case SDL_JOYSTICK_TYPE_ARCADE_PAD:
            return "Arcade Pad";
        case SDL_JOYSTICK_TYPE_THROTTLE:
            return "Throttle";
        default:
        case SDL_JOYSTICK_TYPE_UNKNOWN:
            return "Unknown";
    }
}

namespace onut
{
    OJoystickRef Joystick::create(int index)
    {
        return std::shared_ptr<JoystickSDL2>(new JoystickSDL2(index));
    }

    JoystickSDL2::JoystickSDL2(int index)
        : Joystick(index)
        , m_pSDLJoystick(nullptr)
        , m_axisStates(nullptr)
        , m_state(nullptr)
        , m_previousState(nullptr)
    {
        m_pSDLJoystick = SDL_JoystickOpen(index);
        if (m_pSDLJoystick)
        {
            m_buttonCount = SDL_JoystickNumButtons(m_pSDLJoystick);
            m_hatCount = SDL_JoystickNumHats(m_pSDLJoystick);
            m_axisCount = SDL_JoystickNumAxes(m_pSDLJoystick);
            auto type = SDL_JoystickGetType(m_pSDLJoystick);

            auto buttonCount = getButtonCount();

            m_state = new Uint8[buttonCount];
            m_previousState = new Uint8[buttonCount];
            m_axisStates = new float[m_axisCount];

            memset(m_state, 0, sizeof(Uint8) * buttonCount);
            memset(m_previousState, 0, sizeof(Uint8) * buttonCount);
            memset(m_axisStates, 0, sizeof(int) * m_axisCount);

            switch (type)
            {
                case SDL_JOYSTICK_TYPE_ARCADE_STICK:
                case SDL_JOYSTICK_TYPE_FLIGHT_STICK:
                    m_type = Type::Stick;
                    break;
                case SDL_JOYSTICK_TYPE_THROTTLE:
                    m_type = Type::Throttle;
                    break;
                case SDL_JOYSTICK_TYPE_WHEEL:
                case SDL_JOYSTICK_TYPE_GAMECONTROLLER:
                case SDL_JOYSTICK_TYPE_DANCE_PAD:
                case SDL_JOYSTICK_TYPE_GUITAR:
                case SDL_JOYSTICK_TYPE_DRUM_KIT:
                case SDL_JOYSTICK_TYPE_ARCADE_PAD:
                default:
                    m_type = Type::Other;
                    break;
            }

            m_name = SDL_JoystickName(m_pSDLJoystick);

            OLog("Joystick: " + m_name + 
                 ", type = " + getSDLJoystickTypeName(type) +
                 ", axis = " + std::to_string(m_axisCount) +
                 ", buttons = " + std::to_string(m_buttonCount) + " + " + std::to_string(m_axisCount * 2) +
                 ", balls = " + std::to_string(SDL_JoystickNumBalls(m_pSDLJoystick)) +
                 ", hats = " + std::to_string(m_hatCount));
        }
    }

    JoystickSDL2::~JoystickSDL2()
    {
        if (m_pSDLJoystick)
        {
            SDL_JoystickClose(m_pSDLJoystick);
        }
        if (m_state) delete[] m_state;
        if (m_previousState) delete[] m_previousState;
    }

    void JoystickSDL2::update()
    {
        for (int i = 0; i < m_axisCount; ++i)
        {
            m_axisStates[i] = (float)SDL_JoystickGetAxis(m_pSDLJoystick, i) / 32768.0f;
        }

        if (bSwaped)
        {
            memcpy(m_previousState, m_state, sizeof(Uint8) * getButtonCount());
        }
        bSwaped = true;
    }
    
    void JoystickSDL2::updateSDL2()
    {
        if (!m_pSDLJoystick) return;

        if (bSwaped)
        {
            memcpy(m_previousState, m_state, sizeof(Uint8) * getButtonCount());
        }

        // Buttons
        for (int i = 0; i < m_buttonCount; ++i)
        {
            auto sdlState = SDL_JoystickGetButton(m_pSDLJoystick, i);
            if (sdlState || bSwaped)
            {
                m_state[i] = sdlState;
            }
        }

        // Hats
        auto hatBase = m_buttonCount;
        for (int i = 0; i < m_hatCount; ++i)
        {
            auto hatState = SDL_JoystickGetHat(m_pSDLJoystick, i);
            m_state[hatBase + i * 4 + 0] = (hatState & SDL_HAT_UP) ? 1 : 0;
            m_state[hatBase + i * 4 + 1] = (hatState & SDL_HAT_RIGHT) ? 1 : 0;
            m_state[hatBase + i * 4 + 2] = (hatState & SDL_HAT_DOWN) ? 1 : 0;
            m_state[hatBase + i * 4 + 3] = (hatState & SDL_HAT_LEFT) ? 1 : 0;
        }

        // Axis
        auto axisBase = m_buttonCount + m_hatCount * 4;
        for (int i = 0; i < m_axisCount; ++i)
        {
            auto value = (float)SDL_JoystickGetAxis(m_pSDLJoystick, i) / 32768.0f;
            m_state[axisBase + i * 2 + 0] = (value < -0.3f) ? 1 : 0;
            m_state[axisBase + i * 2 + 1] = (value > 0.3f) ? 1 : 0;
        }

        bSwaped = false;
    }

    bool JoystickSDL2::isPressed(int button) const
    {
        return isPressed(button, m_state);
    }

    bool JoystickSDL2::isJustPressed(int button) const
    {
        return !isPressed(button, m_previousState) && isPressed(button, m_state);
    }

    bool JoystickSDL2::isJustReleased(int button) const
    {
        return isPressed(button, m_previousState) && !isPressed(button, m_state);
    }

    float JoystickSDL2::getAxis(int id) const
    {
        if (id < 0 || id >= getAxisCount()) return 0.0f;
        return m_axisStates[id];
    }

    int JoystickSDL2::getAxisCount() const
    {
        return m_axisCount;
    }

    int JoystickSDL2::getButtonCount() const
    {
        return m_buttonCount + m_hatCount * 4 + m_axisCount * 2;
    }

    int JoystickSDL2::getHatButtonBase() const
    {
        return m_buttonCount;
    }

    int JoystickSDL2::getAxisButtonBase() const
    {
        return m_buttonCount + m_hatCount * 4;
    }

    bool JoystickSDL2::isPressed(int button, const Uint8* state) const
    {
        if (button < 0 || button >= getButtonCount()) return false;
        return state[button] ? true : false;
    }
}
