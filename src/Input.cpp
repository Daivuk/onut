// Onut
#include <onut/GamePad.h>
#include <onut/Input.h>
#include <onut/Window.h>

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
        m_pInputDevice = OInputDevice::create(this, pWindow);

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

        memcpy(m_prevStates, m_curStates, sizeof(InputState) * STATE_COUNT);
        memcpy(m_curStates, m_states, sizeof(InputState) * STATE_COUNT);
        memcpy(m_states, m_statesRestore, sizeof(InputState) * STATE_COUNT);

        for (auto& gamePad : m_gamePads)
        {
            gamePad->update();
        }

        ++m_frameId;
    }

    const OGamePadRef& Input::getGamePad(int index) const
    {
        assert(index >= 0 && index < GAME_PAD_COUNT && "Invalid GamePad Index");
        return m_gamePads[index];
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
