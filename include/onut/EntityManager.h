#pragma once

// STL
#include <vector>

// Forward declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Camera2DComponent);
OForwardDeclare(Component);
OForwardDeclare(Entity);
OForwardDeclare(EntityManager);

namespace onut
{
    class EntityManager final : public std::enable_shared_from_this<EntityManager>
    {
    public:
        static OEntityManagerRef create();

        ~EntityManager();

        void update();
        void render();

        void setActiveCamera2D(const OCamera2DComponentRef& pActiveCamera2D);

        OEntityRef findEntity(const std::string& name) const;

    private:
        friend class Entity;
        friend class Component;

        using Entities = std::vector<OEntityRef>;
        using Components = std::vector<OComponentRef>;

        struct ComponentAction
        {
            enum class Action
            {
                Add,
                Remove
            };

            Action action;
            OComponentRef pComponent;
            Components* pTargetList;
        };

        using ComponentActions = std::vector<ComponentAction>;

        void addEntity(const OEntityRef& pEntity);
        void removeEntity(const OEntityRef& pEntity);

        void addComponentAction(const OComponentRef& pComponent, Components& list, ComponentAction::Action action);
        void performComponentActions();

        void addComponent(const OComponentRef& pComponent, Components& to);
        void removeComponent(const OComponentRef& pComponent, Components& from);

        EntityManager();

        Entities m_entities;
        Components m_componentUpdates;
        Components m_componentRenders;
        ComponentActions m_componentActions;
        OCamera2DComponentRef m_pActiveCamera2D;
    };
};

extern OEntityManagerRef oEntityManager;
