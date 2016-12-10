#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED


// Third parties
#include <list/List.h>

// STL
#include <set>
#include <vector>

// Forward declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Collider2DComponent);
OForwardDeclare(Camera2DComponent);
OForwardDeclare(Component);
OForwardDeclare(Entity);
OForwardDeclare(SceneManager);
OForwardDeclare(Updater);
class b2Contact;
class b2World;

namespace onut
{
    class Physic2DContactListener;

    class SceneManager final : public std::enable_shared_from_this<SceneManager>
    {
    public:
        static OSceneManagerRef create();

        ~SceneManager();

        void update();
        void render();

        void setActiveCamera2D(const OCamera2DComponentRef& pActiveCamera2D);

        OEntityRef findEntity(const std::string& name) const;

        b2World* getPhysic2DWorld() const;

        bool getPause() const;
        void setPause(bool pause);

        OUpdaterRef getUpdater() const;

        void boardcastMessage(int messageId, void* pData = nullptr);

        // This will only display info in _DEBUG
        void DrawDebugInfo();

    private:
        friend class Entity;
        friend class Component;
        friend class Physic2DContactListener;

        using Components = std::vector<OComponentRef>;
        using EntitySet = std::set<OEntityRef>;
        using Entities = std::vector<OEntityRef>;

        struct ComponentAction
        {
            enum class Action
            {
                AddUpdate,
                RemoveUpdate,
                AddRender,
                RemoveRender,
                AddRender2D,
                RemoveRender2D,
            };

            Action action;
            OComponentRef pComponent;
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

        void addComponentAction(const OComponentRef& pComponent, ComponentAction::Action action);
        void performComponentActions();
        void performEntityActions();

        void begin2DContact(b2Contact* pContact);
        void end2DContact(b2Contact* pContact);
        void performContacts();

        SceneManager();

        EntitySet m_entities;
        TList<Component> *m_pComponentUpdates;
        TList<Component> *m_pComponentRenders;
        TList<Component> *m_pComponentRender2Ds;
        Components m_componentJustCreated;
        ComponentActions m_componentActions;
        Contact2Ds m_contact2Ds;
        OCamera2DComponentRef m_pActiveCamera2D;
        Entities m_entitiesToRemove;
        bool m_pause = false;

        b2World* m_pPhysic2DWorld;
        Physic2DContactListener* m_pPhysic2DContactListener;
        OUpdaterRef m_pUpdater;

#if defined(_DEBUG)
        int m_renderCount = 0;
        int m_render2DCount = 0;
#endif
    };
};

extern OSceneManagerRef oSceneManager;

#endif
