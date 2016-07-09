// onut includes
#include <onut/Component.h>
#include <onut/Entity.h>
#include <onut/EntityManager.h>
#include <onut/SpriteBatch.h>

OEntityManagerRef oEntityManager;

namespace onut
{
    OEntityManagerRef EntityManager::create()
    {
        return std::shared_ptr<EntityManager>(new EntityManager());
    }

    EntityManager::EntityManager()
    {
    }

    EntityManager::~EntityManager()
    {
    }

    void EntityManager::add(const OEntityRef& pEntity)
    {
        if (pEntity->m_pEntityManager)
        {
            auto pEntityRef = pEntity;
            pEntityRef->m_pEntityManager->remove(pEntityRef);
            pEntityRef->m_pEntityManager = OThis;
            m_entities.push_back(pEntityRef);
        }
        else
        {
            pEntity->m_pEntityManager = OThis;
            m_entities.push_back(pEntity);
        }
    }

    void EntityManager::remove(const OEntityRef& pEntity)
    {
        for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            if (*it == pEntity)
            {
                m_entities.erase(it);
                return;
            }
        }
    }

    void EntityManager::update()
    {
        for (auto& componentList : m_componentLists)
        {
            for (auto& pComponent : componentList.components)
            {
                pComponent->update();
            }
        }
    }

    void EntityManager::render()
    {
        oSpriteBatch->begin();
        for (auto& componentList : m_componentLists)
        {
            for (auto& pComponent : componentList.components)
            {
                pComponent->render();
            }
        }
        oSpriteBatch->end();
    }
};
