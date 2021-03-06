// Onut
#include <onut/GamePad.h>
#include <onut/Input.h>
#include <onut/Joystick.h>
#include <onut/Renderer.h>

// Private
#include "InputDevice.h"

// STL
#include <cassert>

OInputRef oInput;

namespace onut
{
    OInputRef Input::create(const OWindowRef& pWindow)
    {
        return std::make_shared<OInput>(pWindow);
    }

    Input::Input(const OWindowRef& pWindow)
    {
        m_pInputDevice = OInputDevice::create(this);
        oInputDevice = m_pInputDevice;

        m_prevStates = new InputState[STATE_COUNT];
        m_curStates = new InputState[STATE_COUNT];
        m_states = new InputState[STATE_COUNT];
        m_statesRestore = new InputState[STATE_COUNT];
        m_frameId = 1;

        for (int i = 0; i < GAME_PAD_COUNT; ++i)
        {
            m_gamePads[i] = OGamePad::create(i);
        }
    }

    Input::~Input()
    {
        oInputDevice = nullptr;
        delete[] m_prevStates;
        delete[] m_curStates;
        delete[] m_states;
        delete[] m_statesRestore;
    }

    void Input::setStateDown(State in_stateId)
    {
        assert(static_cast<int>(in_stateId) >= 0 && static_cast<int>(in_stateId) < STATE_COUNT && "Invalid Input State");
        auto& state = m_states[static_cast<int>(in_stateId)];
        m_statesRestore[static_cast<int>(in_stateId)].isDown = true;
        if (state.frameId != m_frameId)
        {
            state.isDown = true;
            state.frameId = m_frameId;
        }
    }

    void Input::setStateUp(State in_stateId)
    {
        assert(static_cast<int>(in_stateId) >= 0 && static_cast<int>(in_stateId) < STATE_COUNT && "Invalid Input State");
        auto& state = m_states[static_cast<int>(in_stateId)];
        m_statesRestore[static_cast<int>(in_stateId)].isDown = false;
        if (state.frameId != m_frameId)
        {
            state.isDown = false;
            state.frameId = m_frameId;
        }
    }

    void Input::setStateValue(State in_stateId, float in_value)
    {
        assert(static_cast<int>(in_stateId) >= 0 && static_cast<int>(in_stateId) < STATE_COUNT && "Invalid Input State");
        m_statesRestore[static_cast<int>(in_stateId)].value = in_value;
        m_states[static_cast<int>(in_stateId)].value = in_value;
    }

    bool Input::isStateDown(State in_stateId) const
    {
        assert(static_cast<int>(in_stateId) >= 0 && static_cast<int>(in_stateId) < STATE_COUNT && "Invalid Input State");
        return m_curStates[static_cast<int>(in_stateId)].isDown;
    }

    bool Input::isStateUp(State in_stateId) const
    {
        assert(static_cast<int>(in_stateId) >= 0 && static_cast<int>(in_stateId) < STATE_COUNT && "Invalid Input State");
        return !m_curStates[static_cast<int>(in_stateId)].isDown;
    }

    bool Input::isStateJustDown(State in_stateId) const
    {
        assert(static_cast<int>(in_stateId) >= 0 && static_cast<int>(in_stateId) < STATE_COUNT && "Invalid Input State");
        return (!m_prevStates[static_cast<int>(in_stateId)].isDown && m_curStates[static_cast<int>(in_stateId)].isDown);
    }

    bool Input::isStateJustUp(State in_stateId) const
    {
        assert(static_cast<int>(in_stateId) >= 0 && static_cast<int>(in_stateId) < STATE_COUNT && "Invalid Input State");
        return (m_prevStates[static_cast<int>(in_stateId)].isDown && !m_curStates[static_cast<int>(in_stateId)].isDown);
    }

    float Input::getStateValue(State in_stateId) const
    {
        assert(static_cast<int>(in_stateId) >= 0 && static_cast<int>(in_stateId) < STATE_COUNT && "Invalid Input State");
        return m_states[static_cast<int>(in_stateId)].value;
    }

    void Input::update()
    {
        m_pInputDevice->update();

        for (auto& gamePad : m_gamePads)
        {
            gamePad->update();
        }

        for (auto& joystick : m_joysticks)
        {
            joystick->update();
        }

        memcpy(m_prevStates, m_curStates, sizeof(InputState) * STATE_COUNT);
        memcpy(m_curStates, m_states, sizeof(InputState) * STATE_COUNT);
        memcpy(m_states, m_statesRestore, sizeof(InputState) * STATE_COUNT);

        if (m_fpsMouse)
        {
            mouseDelta = mousePos - (OScreen / 2);
            setMousePos(OScreen / 2);
        }
        else mouseDelta = {0, 0};

        ++m_frameId;
        m_lastDoubleClicked = m_doubleClicked;
        m_doubleClicked = false;
    }

    OGamePadRef Input::getGamePad(int index) const
    {
        if (index < 0 || index >= 4) return nullptr;
        return m_gamePads[index];
    }

    OJoystickRef Input::getJoystick(int index) const
    {
        if (index < 0 || index >= (int)m_joysticks.size()) return nullptr;
        return m_joysticks[index];
    }

    int Input::getJoystickCount() const
    {
        return (int)m_joysticks.size();
    }

    bool Input::getDoubleClicked() const
    {
        return m_lastDoubleClicked;
    }

    void Input::onDoubleClicked()
    {
        m_doubleClicked = true;
    }

    void Input::activateJoystick(int index)
    {
        m_joysticks.push_back(OJoystick::create(index));
        std::sort(m_joysticks.begin(), m_joysticks.end(), [](const OJoystickRef& a, const OJoystickRef& b)
        {
            return (int)a->getType() < (int)b->getType();
        });
    }

    void Input::deactivateJoystick(int index)
    {
        for (auto it = m_joysticks.begin(); it != m_joysticks.end(); ++it)
        {
            auto pJoystick = *it;
            if (pJoystick->getIndex() == index)
            {
                m_joysticks.erase(it);
                return;
            }
        }
    }

    void Input::setMouseVisible(bool isCursorVisible)
    {
        if (m_isCursorVisible != isCursorVisible)
        {
            m_pInputDevice->setMouseVisible(isCursorVisible);
            m_isCursorVisible = isCursorVisible;
        }
    }

    void Input::setFpsMouse(bool fpsMouse)
    {
        if (m_fpsMouse != fpsMouse)
        {
            if (fpsMouse)
            {
                setMouseVisible(false);
                setMousePos(OScreen / 2);
                mouseDelta = {0, 0};
            }
            else setMouseVisible(true);
            m_fpsMouse = fpsMouse;
        }
    }

    void Input::setMouseIcon(const std::string& name, const Point& hotSpot)
    {
        m_pInputDevice->setMouseIcon(name, hotSpot);
    }

    void Input::unsetMouseIcon()
    {
        m_pInputDevice->unsetMouseIcon();
    }

    void Input::setMousePos(const Point& mousePos)
    {
        m_pInputDevice->setMousePos(mousePos);
    }
}

const Vector2& OGetMousePos()
{
    return oInput->mousePosf;
}

bool OInputPressed(onut::Input::State state)
{
    return oInput->isStateDown(state);
}

bool OInputJustPressed(onut::Input::State state)
{
    return oInput->isStateJustDown(state);
}

bool OInputJustReleased(onut::Input::State state)
{
    return oInput->isStateJustUp(state);
}

bool OInputReleased(onut::Input::State state)
{
    return oInput->isStateUp(state);
}
