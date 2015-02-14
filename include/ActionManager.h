#pragma once

#include <functional>
#include <cinttypes>
#include <vector>

namespace onut
{
    class IAction
    {
    public:
        virtual ~IAction() {}
        virtual void undo() = 0;
        virtual void redo() = 0;
    };

    class ActionGroup : public IAction
    {
    public:
        ActionGroup(const std::vector<IAction*>& actions);
        virtual ~ActionGroup();
        void undo() override;
        void redo() override;

    private:
        std::vector<IAction*> m_actions;
    };

    class Action : public IAction
    {
    public:
        Action(const std::function<void()>& onRedo,
               const std::function<void()>& onUndo,
               const std::function<void()>& onInit = nullptr,
               const std::function<void()>& onDestroy = nullptr);
        virtual ~Action();
        void undo() override;
        void redo() override;

    private:
        std::function<void()> m_onRedo;
        std::function<void()> m_onUndo;
        std::function<void()> m_onDestroy;
    };

    class ActionManager
    {
    public:
        ActionManager(uint32_t maxHistory = 1000);
        virtual ~ActionManager();

        void doAction(IAction* pAction);

        bool canRedo() const;
        bool canUndo() const;

        void redo();
        void undo();

        void clear();

    private:
        using HistoryVector = std::vector < IAction* >;
        HistoryVector           m_history;
        HistoryVector::iterator m_position;

        uint32_t                m_maxHistory;
    };
}
