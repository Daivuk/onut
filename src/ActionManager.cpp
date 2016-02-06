#include "ActionManager.h"

namespace onut
{
    IAction::IAction(const std::string& name) :
        m_name(name)
    {
    }

    const std::string& IAction::getName() const
    {
        return m_name;
    }

    ActionGroup::ActionGroup(const std::string& name, const std::vector<IAction*>& actions) :
        IAction(name),
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

    void ActionGroup::addAction(IAction* pAction)
    {
        m_actions.push_back(pAction);
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

    Action::Action(const std::string& name,
                   const std::function<void()>& onRedo,
                   const std::function<void()>& onUndo,
                   const std::function<void()>& onInit,
                   const std::function<void()>& onDestroy) :
                   IAction(name),
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

    ActionManager::ActionManager(uint32_t maxHistory) :
        m_maxHistory(maxHistory)
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
        if (m_maxHistory)
        {
            while (static_cast<decltype(m_maxHistory)>(m_history.size()) > m_maxHistory)
            {
                auto pFront = m_history.front();
                delete pFront;
                m_history.erase(m_history.begin());
            }
        }
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

    IAction* ActionManager::getRedo() const
    {
        if (!canRedo()) return nullptr;
        return *m_position;
    }

    IAction* ActionManager::getUndo() const
    {
        if (!canUndo()) return nullptr;
        return *(m_position - 1);
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

    void ActionManager::clear()
    {
        for (auto pAction : m_history)
        {
            delete pAction;
        }
        m_history.clear();
        m_position = m_history.begin();
    }
}
