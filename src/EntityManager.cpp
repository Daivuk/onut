// onut includes
#include <onut/Camera2DComponent.h>
#include <onut/Component.h>
#include <onut/Entity.h>
#include <onut/EntityManager.h>
#include <onut/Renderer.h>
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

    void EntityManager::addEntity(const OEntityRef& pEntity)
    {
        if (pEntity->m_pEntityManager)
        {
            auto pEntityRef = pEntity;
            pEntityRef->m_pEntityManager->removeEntity(pEntityRef);
            pEntityRef->m_pEntityManager = OThis;
            m_entities.push_back(pEntityRef);
        }
        else
        {
            pEntity->m_pEntityManager = OThis;
            m_entities.push_back(pEntity);
        }
    }

    void EntityManager::removeEntity(const OEntityRef& pEntity)
    {
        // Remove components
        if (pEntity->isEnabled() && !pEntity->isStatic())
        {
            for (auto& pComponent : pEntity->m_components)
            {
                if (pComponent->isEnabled())
                {
                    addComponentAction(pComponent, m_componentUpdates, ComponentAction::Action::Remove);
                }
            }
        }
        if (pEntity->isVisible())
        {
            for (auto& pComponent : pEntity->m_components)
            {
                addComponentAction(pComponent, m_componentRenders, ComponentAction::Action::Remove);
            }
        }

        // Remove entity
        for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            if (*it == pEntity)
            {
                m_entities.erase(it);
                pEntity->m_pEntityManager = nullptr;
                return;
            }
        }
    }

    void EntityManager::addComponentAction(const OComponentRef& pComponent, Components& list, ComponentAction::Action action)
    {
        m_componentActions.push_back({action, pComponent, &list});
    }

    void EntityManager::performComponentActions()
    {
        for (auto& componentAction : m_componentActions)
        {
            switch (componentAction.action)
            {
                case ComponentAction::Action::Add:
                    addComponent(componentAction.pComponent, *componentAction.pTargetList);
                    break;
                case ComponentAction::Action::Remove:
                    removeComponent(componentAction.pComponent, *componentAction.pTargetList);
                    break;
            }
        }
        m_componentActions.clear();
    }

    void EntityManager::addComponent(const OComponentRef& pComponent, Components& to)
    {
        to.push_back(pComponent);
    }

    void EntityManager::removeComponent(const OComponentRef& pComponent, Components& from)
    {
        for (auto it = from.begin(); it != from.end(); ++it)
        {
            if (*it == pComponent)
            {
                from.erase(it);
                return;
            }
        }
    }

    void EntityManager::setActiveCamera2D(const OCamera2DComponentRef& pActiveCamera2D)
    {
        m_pActiveCamera2D = pActiveCamera2D;
    }

    void EntityManager::update()
    {
        performComponentActions();
        for (auto& pComponent : m_componentUpdates)
        {
            pComponent->update();
        }
        performComponentActions();
    }

    void EntityManager::render()
    {
        for (auto& pComponent : m_componentRenders)
        {
            pComponent->render();
        }

        Matrix transform;
        if (m_pActiveCamera2D)
        {
            if (m_pActiveCamera2D->getClear())
            {
                oRenderer->clear(m_pActiveCamera2D->getClearColor());
            }
            transform = m_pActiveCamera2D->getEntity()->getWorldTransform().Invert();
            transform *= Matrix::CreateScale(m_pActiveCamera2D->getZoom());
            transform *= Matrix::CreateTranslation(OScreenCenterf);
        }
        oSpriteBatch->begin(transform);
        for (auto& pEntity : m_entities)
        {
            if (!pEntity->getParent() && pEntity->isVisible())
            {
                pEntity->render2d();
            }
        }
        oSpriteBatch->end();
    }
};
