#ifndef UPDATER_H_INCLUDED
#define UPDATER_H_INCLUDED

// STL
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(UpdateTarget)
OForwardDeclare(Updater)

namespace onut
{
    class UpdateTarget
    {
    public:
        UpdateTarget();
        UpdateTarget(const UpdateTarget& other);

        virtual ~UpdateTarget();

        bool isUpdateTargetRegistered() const;
        const OUpdaterRef& getUpdater() const;

    protected:
        virtual void update() = 0;

    private:
        friend class Updater;

        bool m_isRegister = false;
        OUpdaterRef m_pUpdater;
    };

    class Updater final : public std::enable_shared_from_this<Updater>
    {
    public:
        using Targets = std::vector<OUpdateTarget*>;
        
        static OUpdaterRef create();

        void update();

        void registerTarget(OUpdateTarget* pTarget);
        void unregisterTarget(OUpdateTarget* pTarget);

        int size() const;

        Targets getTargets();

    private:
        Updater() {}

        Targets m_toAdd;
        Targets m_targets;
    };
};

extern OUpdaterRef oUpdater;

#endif
