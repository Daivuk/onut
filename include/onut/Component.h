#pragma once

// Forward Declaration
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Collider2DComponent);
OForwardDeclare(Component);
OForwardDeclare(Entity);

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

        virtual void onCreate() {}
        virtual void onUpdate() {}
        virtual void onRender() {}
        virtual void onRender2d() {}
        virtual void onMessage(int messageId, void* pData) {}
        virtual void onAddChild(const OEntityRef& pChild) {}
        virtual void onRemoveChild(const OEntityRef& pChild) {}
        virtual void onTriggerEnter(const OCollider2DComponentRef& pCollider) {}
        virtual void onTriggerLeave(const OCollider2DComponentRef& pCollider) {}

    private:
        friend class Entity;
        friend class EntityManager;

        OEntityRef m_pEntity;
        bool m_isEnabled = true;
    };
};
