#pragma once

// STL
#include <vector>

// Forward declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Collider2DComponent);
OForwardDeclare(Camera2DComponent);
OForwardDeclare(Component);
OForwardDeclare(Entity);
OForwardDeclare(EntityManager);
class b2Contact;
class b2World;

namespace onut
{
    class Physic2DContactListener;

    class EntityManager final : public std::enable_shared_from_this<EntityManager>
    {
    public:
        static OEntityManagerRef create();

        ~EntityManager();

        void update();
        void render();

        void setActiveCamera2D(const OCamera2DComponentRef& pActiveCamera2D);

        OEntityRef findEntity(const std::string& name) const;

        b2World* getPhysic2DWorld() const;

    private:
        friend class Entity;
        friend class Component;
        friend class Physic2DContactListener;

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

        struct Contact2D
        {
            enum class Type
            {
                Begin,
                End
            };

            Type type;
            OCollider2DComponentRef pColliderA;
            OCollider2DComponentRef pColliderB;
        };

        using ComponentActions = std::vector<ComponentAction>;
        using Contact2Ds = std::vector<Contact2D>;

        void addEntity(const OEntityRef& pEntity);
        void removeEntity(const OEntityRef& pEntity);

        void addComponentAction(const OComponentRef& pComponent, Components& list, ComponentAction::Action action);
        void performComponentActions();

        void addComponent(const OComponentRef& pComponent, Components& to);
        void removeComponent(const OComponentRef& pComponent, Components& from);

        void begin2DContact(b2Contact* pContact);
        void end2DContact(b2Contact* pContact);
        void performContacts();

        EntityManager();

        Entities m_entities;
        Components m_componentUpdates;
        Components m_componentRenders;
        Components m_componentJustCreated;
        ComponentActions m_componentActions;
        Contact2Ds m_contact2Ds;
        OCamera2DComponentRef m_pActiveCamera2D;

        b2World* m_pPhysic2DWorld;
        Physic2DContactListener* m_pPhysic2DContactListener;
    };
};

extern OEntityManagerRef oEntityManager;
