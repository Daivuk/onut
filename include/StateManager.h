#include <unordered_map>
#include <vector>
#include "State.h"

namespace onut
{
    template<typename TenumType>
    class IStateBuilder
    {
    public:
        IStateBuilder(TenumType stateId) : m_stateId(stateId) {}

        virtual IState* createState() = 0;

        TenumType getStateId() const { return m_stateId; }

    private:
        TenumType m_stateId;
    };

    template<typename TstateType, typename TenumType>
    class StateBuilder : public IStateBuilder<TenumType>
    {
    public:
        StateBuilder(TenumType stateId) : IStateBuilder(stateId) {}

        IState* createState() override
        {
            return new TstateType();
        }
    };

    template<typename TstateType, typename TenumType>
    IStateBuilder<TenumType>* DefineState(TenumType stateId)
    {
        return new StateBuilder<TstateType, TenumType>(stateId);
    }

    template<typename TstateEnum>
    class StateManager : public State < TstateEnum >
    {
    private:
        IState*                                                     m_pCurrentState;
        std::unordered_map<TstateEnum, IStateBuilder<TstateEnum>*>  m_states;

    public:
        StateManager(TstateEnum startState, const std::vector<IStateBuilder<TstateEnum>*>& states) :
            State<TstateEnum>(startState)
        {
            for (auto& state : states)
            {
                m_states[state->getStateId()] = state;
            }
        }

        virtual ~StateManager()
        {
            if (m_pCurrentState) delete m_pCurrentState;
            for (auto& kv : m_states)
            {
                delete kv.second;
            }
        }

        void init()
        {
            m_pCurrentState = m_states[getState()]->createState();
            m_pCurrentState->init();
        }

        void update()
        {
            m_pCurrentState->update();
        }

        void render()
        {
            m_pCurrentState->render();
        }

    protected:
        bool onLeaveState(TstateEnum oldState, TstateEnum newState)
        {
            if (m_pCurrentState) delete m_pCurrentState;
            m_pCurrentState = nullptr;
            return true;
        }

        void onEnterState(TstateEnum newState)
        {
            m_pCurrentState = m_states[newState]->createState();
            m_pCurrentState->init();
        }
    };
}
