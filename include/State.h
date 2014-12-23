#pragma once

namespace onut {
    template<typename TstateEnum>
    class State {
    public:
        State(TstateEnum startingState) {
            m_state = startingState;
        }
        virtual ~State() {}

        TstateEnum getState() const { return m_state; }
        void changeState(TstateEnum newState) {
            if (newState == m_state) {
                return;
            }

            if (onLeaveState(m_state, newState)) {
                m_state = newState;
                onEnterState(m_state);
            }
        }

    protected:
        virtual bool onLeaveState(TstateEnum oldState, TstateEnum newState) { return true; }
        virtual void onEnterState(TstateEnum newState) {}

    private:
        TstateEnum m_state;
    };
}
