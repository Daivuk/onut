#pragma once

// Forward Declaration
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Entity);
OForwardDeclare(Component);

namespace onut
{
    class Component : public std::enable_shared_from_this<Component>
    {
    public:
        virtual ~Component();

        const OEntityRef& getEntity() const;

        bool isEnabled() const;
        void setEnabled(bool isEnabled);

        template<typename Tcomponent>
        std::shared_ptr<Tcomponent> getComponent() const
        {
            return getEntity()->getComponent<Tcomponent>();
        }

    protected:
        Component();

        virtual void update() {}
        virtual void render() {}
        virtual void render2d() {}

    private:
        friend class Entity;
        friend class EntityManager;

        OEntityRef m_pEntity;
        bool m_isEnabled = true;
    };
};
