#include "ActionManager.h"

namespace onut
{
    ActionGroup::ActionGroup(const std::vector<IAction*>& actions) :
        m_actions(actions)
    {
    }

    ActionGroup::~ActionGroup()
    {
        for (auto pAction : m_actions)
        {
            delete pAction;
        }
    }

    void ActionGroup::undo()
    {
        for (auto it = m_actions.rbegin(); it != m_actions.rend(); ++it)
        {
            (*it)->undo();
        }
    }

    void ActionGroup::redo()
    {
        for (auto pAction : m_actions)
        {
            pAction->redo();
        }
    }

    Action::Action(const std::function<void()>& onRedo,
                   const std::function<void()>& onUndo,
                   const std::function<void()>& onInit,
                   const std::function<void()>& onDestroy) :
                   m_onRedo(onRedo),
                   m_onUndo(onUndo),
                   m_onDestroy(onDestroy)
    {
        if (onInit)
        {
            onInit();
        }
    }

    Action::~Action()
    {
        if (m_onDestroy)
        {
            m_onDestroy();
        }
    }

    void Action::undo()
    {
        if (m_onUndo)
        {
            m_onUndo();
        }
    }

    void Action::redo()
    {
        if (m_onRedo)
        {
            m_onRedo();
        }
    }

    ActionManager::ActionManager()
    {
        m_position = m_history.begin();
    }

    ActionManager::~ActionManager()
    {
        for (auto pAction : m_history)
        {
            delete pAction;
        }
    }

    void ActionManager::doAction(IAction* pAction)
    {
        for (auto it = m_position; it < m_history.end(); ++it)
        {
            delete *it;
        }
        m_history.resize(m_position - m_history.begin());
        m_history.push_back(pAction);
        m_position = m_history.end();
        pAction->redo();
    }
    
    bool ActionManager::canRedo() const
    {
        return (m_position != m_history.end());
    }

    bool ActionManager::canUndo() const
    {
        return (m_position != m_history.begin());
    }
    
    void ActionManager::redo()
    {
        if (canRedo())
        {
            (*m_position)->redo();
            ++m_position;
        }
    }

    void ActionManager::undo()
    {
        if (canUndo())
        {
            --m_position;
            (*m_position)->undo();
        }
    }
}
