#pragma once

#include <onut/Maths.h>

// Forward Declaration
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Collider2DComponent);
OForwardDeclare(Component);
OForwardDeclare(Entity);
OForwardDeclare(EntityManager);

namespace onut
{
    class Component : public std::enable_shared_from_this<Component>
    {
    public:
        virtual ~Component();

        const OEntityRef& getEntity() const;

        bool isEnabled() const;
        void setEnabled(bool isEnabled);

        // Entity mirror functions for convenience
        template<typename Tcomponent>
        std::shared_ptr<Tcomponent> getComponent() const
        {
            return getEntity()->getComponent<Tcomponent>();
        }
        template<typename Tcomponent>
        std::shared_ptr<Tcomponent> getParentComponent() const
        {
            return getEntity()->getParentComponent<Tcomponent>();
        }
        const OEntityManagerRef& getEntityManager() const;
        void sendMessage(int messageId, void* pData = nullptr);
        void destroy();
        const Matrix& getLocalTransform() const;
        const Matrix& getWorldTransform();
        void setLocalTransform(const Matrix& localTransform);
        void setWorldTransform(const Matrix& worldTransform);

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
