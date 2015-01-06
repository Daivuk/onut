#include <assert.h>
#include <memory.h>
#include <Windows.h>
#include "Input.h"

namespace onut
{
    Input::Input(int in_stateCount)
    {
        m_stateCount = in_stateCount;
        m_prevStates = new sInputState[m_stateCount];
        m_curStates = new sInputState[m_stateCount];
        m_states = new sInputState[m_stateCount];
        m_statesRestore = new sInputState[m_stateCount];
        m_frameId = 1;
    }

    Input::~Input()
    {
        delete[] m_prevStates;
        delete[] m_curStates;
        delete[] m_states;
        delete[] m_statesRestore;
    }
    void Input::setStateDown(int in_stateId)
    {
        assert(in_stateId >= 0 && in_stateId < m_stateCount && "Invalid Input State");
        auto& state = m_states[in_stateId];
        m_statesRestore[in_stateId].isDown = true;
        if (state.frameId != m_frameId)
        {
            state.isDown = true;
            state.frameId = m_frameId;
        }
    }

    void Input::setStateUp(int in_stateId)
    {
        assert(in_stateId >= 0 && in_stateId < m_stateCount && "Invalid Input State");
        auto& state = m_states[in_stateId];
        m_statesRestore[in_stateId].isDown = false;
        if (state.frameId != m_frameId)
        {
            state.isDown = false;
            state.frameId = m_frameId;
        }
    }

    void Input::setStateValue(int in_stateId, float in_value)
    {
        assert(in_stateId >= 0 && in_stateId < m_stateCount && "Invalid Input State");
        m_statesRestore[in_stateId].value = in_value;
        m_states[in_stateId].value = in_value;
    }

    bool Input::isStateDown(int in_stateId) const
    {
        assert(in_stateId >= 0 && in_stateId < m_stateCount && "Invalid Input State");
        return m_curStates[in_stateId].isDown;
    }

    bool Input::isStateUp(int in_stateId) const
    {
        assert(in_stateId >= 0 && in_stateId < m_stateCount && "Invalid Input State");
        return !m_curStates[in_stateId].isDown;
    }

    bool Input::isStateJustDown(int in_stateId) const
    {
        assert(in_stateId >= 0 && in_stateId < m_stateCount && "Invalid Input State");
        return (!m_prevStates[in_stateId].isDown && m_curStates[in_stateId].isDown);
    }

    bool Input::isStateJustUp(int in_stateId) const
    {
        assert(in_stateId >= 0 && in_stateId < m_stateCount && "Invalid Input State");
        return (m_prevStates[in_stateId].isDown && !m_curStates[in_stateId].isDown);
    }

    float Input::getStateValue(int in_stateId) const
    {
        assert(in_stateId >= 0 && in_stateId < m_stateCount && "Invalid Input State");
        return m_states[in_stateId].value;
    }

    void Input::update()
    {
        memcpy(m_prevStates, m_curStates, sizeof(sInputState) * m_stateCount);
        memcpy(m_curStates, m_states, sizeof(sInputState) * m_stateCount);
        memcpy(m_states, m_statesRestore, sizeof(sInputState) * m_stateCount);
        ++m_frameId;
    }
}
