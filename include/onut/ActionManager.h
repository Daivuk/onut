#ifndef ACTIONMANAGER_H_INCLUDED
#define ACTIONMANAGER_H_INCLUDED

// STL
#include <functional>
#include <cinttypes>
#include <string>
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(IAction)
OForwardDeclare(ActionGroup)
OForwardDeclare(Action)
OForwardDeclare(ActionManager)

namespace onut
{
    /**
     * @class IAction
     * @ingroup Utility
     * An Undoable and Redoable action.
     */
    class IAction
    {
    public:
        IAction(const std::string& name);
        virtual ~IAction() {}

        virtual void undo() = 0;
        virtual void redo() = 0;

        const std::string& getName() const;

    private:
        std::string m_name;
    };

    using Actions = std::vector<OIActionRef>;

    class ActionGroup final : public IAction
    {
    public:
        ActionGroup(const std::string& name, const Actions& actions = {});

        void addAction(const OIActionRef& pAction);
        void undo() override;
        void redo() override;

    private:
        Actions m_actions;
    };

    /**
     * @class Action
     * @ingroup Utility
     * An Undoable and Redoable action.
     */
    class Action final : public IAction
    {
    public:
        using ActionCallBack = std::function<void()>;

        /**
         * Constructor
         * @param name Action name. Useful if you wish to display the list of undoable actions in a dropdown menu.
         * @param onRedo Called when redo is performed on this action.
         * @param onUndo Called when undo is performed on this action.
         * @param onInit Called immediately before this function returns.
         * @param onDestroy Called when the action is destroyed.
         */ 
        Action(const std::string& name,
               const ActionCallBack& onRedo,
               const ActionCallBack& onUndo,
               const ActionCallBack& onInit = nullptr,
               const ActionCallBack& onDestroy = nullptr);
        ~Action();

        void undo() override;
        void redo() override;

    private:
        ActionCallBack m_onRedo;
        ActionCallBack m_onUndo;
        ActionCallBack m_onDestroy;
    };

    /**
     * @class ActionManager
     * @ingroup Service
     * Manages undo/redo.
     */
    class ActionManager final
    {
    public:
        /**
         * Creates a new ActionManager.
         */
        static OActionManagerRef create();

        /**
         * Constructor
         * @param maxHistory How many actions to keep. If more actions are added, older ones will be lost.
         */
        ActionManager(uint32_t maxHistory = 1000);

        /**
         * Add an action to the history, and activates it.
         * @sa onut::Action
         */
        void doAction(const OIActionRef& pAction);

        /**
         * Add an action to the history, without activating it.
         * @sa onut::Action
         */
        void addAction(const OIActionRef& pAction);

        /**
         * Add an action to the history, and activates it.
         * @param name Action name. Useful if you wish to display the list of undoable actions in a dropdown menu.
         * @param onRedo Called when redo is performed on this action.
         * @param onUndo Called when undo is performed on this action.
         * @param onInit Called immediately before this function returns.
         * @param onDestroy Called when the action is destroyed.
         * @sa onut::Action
         */
        void doAction(const std::string& name,
                      const Action::ActionCallBack& onRedo,
                      const Action::ActionCallBack& onUndo,
                      const Action::ActionCallBack& onInit = nullptr,
                      const Action::ActionCallBack& onDestroy = nullptr);

        /**
         * Add an action to the history, without activating it.
         * @param name Action name. Useful if you wish to display the list of undoable actions in a dropdown menu.
         * @param onRedo Called when redo is performed on this action.
         * @param onUndo Called when undo is performed on this action.
         * @param onInit Called immediately before this function returns.
         * @param onDestroy Called when the action is destroyed.
         * @sa onut::Action
         */
        void addAction(const std::string& name,
                      const Action::ActionCallBack& onRedo,
                      const Action::ActionCallBack& onUndo,
                      const Action::ActionCallBack& onInit = nullptr,
                      const Action::ActionCallBack& onDestroy = nullptr);

        bool canRedo() const;
        bool canUndo() const;
        OIActionRef getRedo() const;
        OIActionRef getUndo() const;
        void redo();
        void undo();
        void clear();

        const Actions& getHistory() const { return m_history; }
        Actions::iterator getHistoryPosition() const { return m_position; }

        void setMaxHistory(uint32_t maxHistory);

    private:
        Actions m_history;
        Actions::iterator m_position;
        uint32_t m_maxHistory;
    };
}

extern OActionManagerRef oActionManager;

#endif
