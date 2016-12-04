#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

// Onut includes
#include <onut/Maths.h>

// Third parties
#include <list/List.h>

// STL
#include <unordered_map>
#include <vector>

// Forward declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Collider2DComponent);
OForwardDeclare(Component);
OForwardDeclare(Entity);
OForwardDeclare(SceneManager);

namespace onut
{
    class Entity final : public std::enable_shared_from_this<Entity>
    {
    public:
        using Entities = std::vector<OEntityRef>;

        static OEntityRef create(const OSceneManagerRef& pSceneManager = nullptr);

        ~Entity();

        void destroy();
        OEntityRef copy() const;

        const Matrix& getLocalTransform() const;
        const Matrix& getWorldTransform();
        void setLocalTransform(const Matrix& localTransform);
        void setWorldTransform(const Matrix& worldTransform);

        void add(const OEntityRef& pChild);
        void remove(const OEntityRef& pChild);
        void remove();
        OEntityRef getParent() const;

        bool isEnabled() const;
        void setEnabled(bool isEnabled);
        bool isVisible() const;
        void setVisible(bool isVisible);
        bool isStatic() const;
        void setStatic(bool isStatic);

        const std::string getName() const;
        void setName(const std::string& name);

        template<typename Tcomponent>
        std::shared_ptr<Tcomponent> getComponent() const
        {
            for (auto& pComponent : m_components)
            {
                if (dynamic_cast<Tcomponent*>(pComponent.get()))
                {
                    return ODynamicCast<Tcomponent>(pComponent);
                }
            }
            return nullptr;
        }

        template<typename Tcomponent>
        std::shared_ptr<Tcomponent> getParentComponent() const
        {
            for (auto& pComponent : m_components)
            {
                if (dynamic_cast<Tcomponent*>(pComponent.get()))
                {
                    return ODynamicCast<Tcomponent>(pComponent);
                }
            }
            auto pParent = getParent();
            if (pParent)
            {
                return pParent->getParentComponent<Tcomponent>();
            }
            return nullptr;
        }

        template<typename Tcomponent>
        std::shared_ptr<Tcomponent> addComponent()
        {
            auto pComponent = getComponent<Tcomponent>();
            if (pComponent) return pComponent;
            pComponent = std::shared_ptr<Tcomponent>(new Tcomponent());
            addComponent(pComponent);
            return pComponent;
        }

        void addComponent(const OComponentRef& pComponent);

        const OSceneManagerRef& getSceneManager() const;
        void sendMessage(int messageId, void* pData = nullptr);

        int getDrawIndex() const;
        void setDrawIndex(int drawIndex);

        const Entities& getChildren() const;

    private:
        friend class Component;
        friend class SceneManager;

        using Components = std::vector<OComponentRef>;

        Entity();

        void dirtyWorld();
        void render2d();
        void onTriggerEnter(const OCollider2DComponentRef& pCollider);
        void onTriggerLeave(const OCollider2DComponentRef& pCollider);

        bool m_isWorldDirty = true;
        Matrix m_localTransform;
        Matrix m_worldTransform;
        Components m_components;
        Entities m_children;
        OEntityWeak m_pParent;
        OSceneManagerRef m_pSceneManager;
        bool m_isEnabled = true;
        bool m_isVisible = true;
        bool m_isStatic = false;
        int m_drawIndex = 0;
        std::string m_name;
    };
};

#endif
