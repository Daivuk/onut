#pragma once
// Onut includes
#include <onut/Maths.h>

// STL
#include <vector>

// Forward declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Component);
OForwardDeclare(Entity);

namespace onut
{
    class Entity final
    {
    public:
        static OEntityRef create();

        ~Entity();

        OEntityRef copy() const;

        const Matrix& getLocalTransform() const;
        const Matrix& getWorldTransform();
        void setLocalTransform(const Matrix& localTransform);
        void setWorldTransform(const Matrix& worldTransform);

        void add(const OEntityRef& pChild);
        void remove(const OEntityRef& pChild);
        OEntityRef getParent() const;

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
        void addComponent(const OComponentRef& pComponent);

        void update();
        void render();

    private:
        Entity();

        void updateTree();
        void dirtyWorld();

        std::vector<OComponentRef> m_components;
        std::vector<OEntityRef> m_children;
        bool m_isWorldDirty = true;
        Matrix m_localTransform;
        Matrix m_worldTransform;
        OEntityWeak m_pParent;

        enum class ActionType
        {
            Add,
            Remove
        };

        struct Action
        {
            ActionType action;
            OEntityRef pParent;
            OEntityRef pChild;
        };

        static std::vector<Action> s_actions;
    };
};
