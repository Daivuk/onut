#ifndef COMPONENT_H_INCLUDED
#define COMPONENT_H_INCLUDED


// Onut includes
#include <onut/Entity.h>
#include <onut/Maths.h>

// Third parties
#include <list/List.h>

// Forward Declaration
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Collider2DComponent);
OForwardDeclare(Component);
OForwardDeclare(SceneManager);

namespace onut
{
    class Component : public std::enable_shared_from_this<Component>
    {
    public:
        enum Flag : int
        {
            FLAG_NONE = 0,
            FLAG_RENDERABLE = 1,
            FLAG_UPDATABLE = 2,
            FLAG_RENDERABLE_2D = 4,
            FLAG_BROADCAST_LISTENER = 8,
        };

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
        const OSceneManagerRef& getSceneManager() const;
        void sendMessage(int messageId, void* pData = nullptr);
        void boardcastMessage(int messageId, void* pData = nullptr);
        void destroy();
        const Matrix& getLocalTransform() const;
        const Matrix& getWorldTransform();
        void setLocalTransform(const Matrix& localTransform);
        void setWorldTransform(const Matrix& worldTransform);

    protected:
        Component(int flags = FLAG_NONE);

        virtual void onCreate() {}
        virtual void onUpdate() {}
        virtual void onRender() {}
        virtual void onRender2d() {}
        virtual void onMessage(int messageId, void* pData) {}
        virtual void onAddChild(const OEntityRef& pChild) {}
        virtual void onRemoveChild(const OEntityRef& pChild) {}
        virtual void onTriggerEnter(const OCollider2DComponentRef& pCollider) {}
        virtual void onTriggerLeave(const OCollider2DComponentRef& pCollider) {}
        virtual void onEnable() {}
        virtual void onDisable() {}
        virtual void onDestroy() {}

    private:
        friend class Entity;
        friend class SceneManager;

        OEntityRef m_pEntity;
        bool m_isEnabled = true;
        int m_flags = FLAG_NONE;

        // List links
        LIST_LINK(Component) m_updateLink;
        LIST_LINK(Component) m_renderLink;
        LIST_LINK(Component) m_render2DLink;
    };
};

#endif
