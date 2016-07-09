#pragma once

// Forward Declaration
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Entity);
OForwardDeclare(Component);

namespace onut
{
    class Component
    {
    public:
        virtual ~Component();

        const OEntityRef& getEntity() const;

    protected:
        Component();

        virtual void update() {}
        virtual void render() {}

    private:
        friend class Entity;
        friend class EntityManager;

        OEntityRef m_pEntity;
    };
};
