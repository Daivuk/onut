#pragma once

// STL
#include <vector>

// Forward declarations
#include <onut/ForwardDeclaration.h>
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

    private:
        struct ComponentList
        {
            using Components = std::vector<OComponentRef>;

            template<typename Tcomponent>
            ComponentList(const std::shared_ptr<Tcomponent>& pComponent)
                : type(typeid(Tcomponent).hash_code())
            {
                components.push_back(pComponent);
            }

            Components components;
            size_t type;
        };

    public:
        template<typename Tcomponent>
        void addComponent(const std::shared_ptr<Tcomponent>& pComponent)
        {
            auto typeCode = typeid(Tcomponent).hash_code();
            for (auto& componentList : m_componentLists)
            {
                if (componentList.type == typeCode)
                {
                    componentList.components.push_back(pComponent);
                    return;
                }
            }
            m_componentLists.push_back(ComponentList(pComponent));
        }

    private:
        friend class Entity;

        using Entities = std::vector<OEntityRef>;
        using ComponentLists = std::vector<ComponentList>;

        void add(const OEntityRef& pEntity);
        void remove(const OEntityRef& pEntity);

        EntityManager();

        Entities m_entities;
        ComponentLists m_componentLists;
    };
};

extern OEntityManagerRef oEntityManager;
