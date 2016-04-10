// Onut
#include <onut/ActionManager.h>

OActionManagerRef oActionManager;

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

    ActionGroup::ActionGroup(const std::string& name, const Actions& actions) :
        IAction(name),
        m_actions(actions)
    {
    }

    void ActionGroup::addAction(const OIActionRef& pAction)
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
                   const ActionCallBack& onRedo,
                   const ActionCallBack& onUndo,
                   const ActionCallBack& onInit,
                   const ActionCallBack& onDestroy)
        : IAction(name)
        , m_onRedo(onRedo)
        , m_onUndo(onUndo)
        , m_onDestroy(onDestroy)
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
    
    OActionManagerRef ActionManager::create()
    {
        return OMake<ActionManager>();
    }

    ActionManager::ActionManager(uint32_t maxHistory) :
        m_maxHistory(maxHistory)
    {
        m_position = m_history.begin();
    }

    void ActionManager::setMaxHistory(uint32_t maxHistory)
    {
        m_maxHistory = maxHistory;
    }

    void ActionManager::doAction(const OIActionRef& pAction)
    {
        m_history.resize(m_position - m_history.begin());
        m_history.push_back(pAction);
        if (m_maxHistory)
        {
            while (static_cast<decltype(m_maxHistory)>(m_history.size()) > m_maxHistory)
            {
                m_history.erase(m_history.begin());
            }
        }
        m_position = m_history.end();
        pAction->redo();
    }

    void ActionManager::addAction(const OIActionRef& pAction)
    {
        m_history.resize(m_position - m_history.begin());
        m_history.push_back(pAction);
        if (m_maxHistory)
        {
            while (static_cast<decltype(m_maxHistory)>(m_history.size()) > m_maxHistory)
            {
                m_history.erase(m_history.begin());
            }
        }
        m_position = m_history.end();
    }

    void ActionManager::doAction(const std::string& name,
                                 const Action::ActionCallBack& onRedo,
                                 const Action::ActionCallBack& onUndo,
                                 const Action::ActionCallBack& onInit,
                                 const Action::ActionCallBack& onDestroy)
    {
        doAction(OMake<Action>(name, onRedo, onUndo, onInit, onDestroy));
    }

    void ActionManager::addAction(const std::string& name,
                                  const Action::ActionCallBack& onRedo,
                                  const Action::ActionCallBack& onUndo,
                                  const Action::ActionCallBack& onInit,
                                  const Action::ActionCallBack& onDestroy)
    {
        addAction(OMake<Action>(name, onRedo, onUndo, onInit, onDestroy));
    }

    bool ActionManager::canRedo() const
    {
        return (m_position != m_history.end());
    }

    bool ActionManager::canUndo() const
    {
        return (m_position != m_history.begin());
    }

    OIActionRef ActionManager::getRedo() const
    {
        if (!canRedo()) return nullptr;
        return *m_position;
    }

    OIActionRef ActionManager::getUndo() const
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
        m_history.clear();
        m_position = m_history.begin();
    }
}
