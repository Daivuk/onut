#pragma once

namespace onut
{
    class IState
    {
    public:
        virtual ~IState() {}

        virtual void init() {}
        virtual void update() {}
        virtual void render() {}
    };

    template<typename TstateEnum>
    class State : public IState
    {
    public:
        State(TstateEnum startingState)
        {
            m_state = startingState;
        }
        virtual ~State() {}

        TstateEnum getState() const { return m_state; }
        void changeState(TstateEnum newState)
        {
            if (newState == m_state)
            {
                return;
            }

            if (onLeaveState(m_state, newState))
            {
                m_state = newState;
                onEnterState(m_state);
            }
        }

        IState* pParentState = nullptr;

    protected:
        virtual bool onLeaveState(TstateEnum oldState, TstateEnum newState) { return true; }
        virtual void onEnterState(TstateEnum newState) {}

    private:
        TstateEnum m_state;
    };
}
