// Onut includes
#include <onut/Component.h>
#include <onut/Entity.h>
#include <onut/SceneManager.h>

// STL
#include <atomic>

namespace onut
{
#if defined(_DEBUG)
    std::atomic<int> g_entityCount = 0;
#endif

    OEntityRef Entity::create(const OSceneManagerRef& in_pSceneManager)
    {
        auto pSceneManager = in_pSceneManager;
        if (!pSceneManager) pSceneManager = oSceneManager;
        auto pEntity = std::shared_ptr<OEntity>(new OEntity());
        pSceneManager->addEntity(pEntity);
        return pEntity;
    }

    Entity::Entity()
    {
#if defined(_DEBUG)
        ++g_entityCount;
#endif
    }

    Entity::~Entity()
    {
#if defined(_DEBUG)
        --g_entityCount;
#endif
    }

    void Entity::destroy()
    {
        auto pThis = OThis;
        m_pSceneManager->removeEntity(pThis);
        auto pParent = getParent();
        if (pParent)
        {
            pParent->remove(pThis);
        }
        m_components.clear();
    }

    OEntityRef Entity::copy() const
    {
        return nullptr;
    }

    const Matrix& Entity::getLocalTransform() const
    {
        return m_localTransform;
    }

    const Matrix& Entity::getWorldTransform()
    {
        if (m_isWorldDirty)
        {
            auto pParent = getParent();
            if (pParent)
            {
                m_worldTransform = m_localTransform * pParent->getWorldTransform();
            }
            else
            {
                m_worldTransform = m_localTransform;
            }
            m_isWorldDirty = false;
        }
        return m_worldTransform;
    }

    void Entity::setLocalTransform(const Matrix& localTransform)
    {
        m_localTransform = localTransform;
        dirtyWorld();
    }

    void Entity::setWorldTransform(const Matrix& worldTransform)
    {
        Matrix parentWorld;
        auto pParent = getParent();
        if (pParent)
        {
            parentWorld = pParent->getWorldTransform();
        }
        auto invParentWorld = parentWorld.Invert();
        m_localTransform = worldTransform * invParentWorld;
        m_isWorldDirty = true;
    }

    void Entity::dirtyWorld()
    {
        m_isWorldDirty = true;
        for (auto& pChild : m_children)
        {
            pChild->dirtyWorld();
        }
    }

    void Entity::add(const OEntityRef& pChild)
    {
        m_children.push_back(pChild);
        auto pChildParent = pChild->getParent();
        if (pChildParent)
        {
            pChildParent->remove(pChild);
        }
        pChild->dirtyWorld();
        pChild->m_pParent = OThis;
        for (auto& pComponent : m_components)
        {
            pComponent->onAddChild(pChild);
        }
    }

    void Entity::remove(const OEntityRef& pChild)
    {
        for (auto it = m_children.begin(); it != m_children.end(); ++it)
        {
            if (*it == pChild)
            {
                for (auto& pComponent : m_components)
                {
                    pComponent->onRemoveChild(pChild);
                }
                pChild->m_pParent.reset();
                m_children.erase(it);
                pChild->dirtyWorld();
                return;
            }
        }
    }

    void Entity::remove()
    {
        auto pParent = getParent();
        if (pParent)
        {
            pParent->remove(OThis);
        }
    }

    OEntityRef Entity::getParent() const
    {
        return m_pParent.lock();
    }

    bool Entity::isEnabled() const
    {
        return m_isEnabled;
    }

    void Entity::setEnabled(bool isEnabled)
    {
        if (!m_isStatic)
        {
            if (m_isEnabled && !isEnabled)
            {
                for (auto& pComponent : m_components)
                {
                    if (pComponent->isEnabled() && pComponent->m_flags & Component::FLAG_UPDATABLE)
                    {
                        m_pSceneManager->addComponentAction(pComponent, SceneManager::ComponentAction::Action::RemoveUpdate);
                    }
                }
            }
            else if (!m_isEnabled && isEnabled)
            {
                for (auto& pComponent : m_components)
                {
                    if (pComponent->isEnabled() && pComponent->m_flags & Component::FLAG_UPDATABLE)
                    {
                        m_pSceneManager->addComponentAction(pComponent, SceneManager::ComponentAction::Action::AddUpdate);
                    }
                }
            }
        }
        m_isEnabled = isEnabled;
    }

    bool Entity::isVisible() const
    {
        return m_isVisible;
    }

    void Entity::setVisible(bool isVisible)
    {
        if (m_isVisible && !isVisible)
        {
            for (auto& pComponent : m_components)
            {
                if (pComponent->isEnabled() && pComponent->m_flags & Component::FLAG_RENDERABLE)
                {
                    m_pSceneManager->addComponentAction(pComponent, SceneManager::ComponentAction::Action::RemoveRender);
                }
            }
        }
        else if (!m_isVisible && isVisible)
        {
            for (auto& pComponent : m_components)
            {
                if (pComponent->isEnabled() && pComponent->m_flags & Component::FLAG_RENDERABLE)
                {
                    m_pSceneManager->addComponentAction(pComponent, SceneManager::ComponentAction::Action::AddRender);
                }
            }
        }
        m_isVisible = isVisible;
    }

    bool Entity::isStatic() const
    {
        return m_isStatic;
    }

    void Entity::setStatic(bool isStatic)
    {
        if (m_isEnabled)
        {
            if (m_isStatic && !isStatic)
            {
                for (auto& pComponent : m_components)
                {
                    if (pComponent->isEnabled() && pComponent->m_flags & Component::FLAG_UPDATABLE)
                    {
                        m_pSceneManager->addComponentAction(pComponent, SceneManager::ComponentAction::Action::RemoveUpdate);
                    }
                }
            }
            else if (!m_isStatic && isStatic)
            {
                for (auto& pComponent : m_components)
                {
                    if (pComponent->isEnabled() && pComponent->m_flags & Component::FLAG_UPDATABLE)
                    {
                        m_pSceneManager->addComponentAction(pComponent, SceneManager::ComponentAction::Action::AddUpdate);
                    }
                }
            }
        }
        m_isStatic = isStatic;
    }

    void Entity::addComponent(const OComponentRef& pComponent)
    {
        pComponent->m_pEntity = OThis;
        if (m_pSceneManager)
        {
            m_pSceneManager->m_componentJustCreated.push_back(pComponent);
        }
        m_components.push_back(pComponent);
        if (pComponent->isEnabled())
        {
            if (m_isEnabled && !m_isStatic && pComponent->m_flags & Component::FLAG_UPDATABLE)
            {
                m_pSceneManager->addComponentAction(pComponent, SceneManager::ComponentAction::Action::AddUpdate);
            }
            if (m_isVisible && pComponent->m_flags & Component::FLAG_RENDERABLE)
            {
                m_pSceneManager->addComponentAction(pComponent, SceneManager::ComponentAction::Action::AddRender);
            }
        }
    }

    void Entity::render2d()
    {
        for (auto& pComponent : m_components)
        {
            if (pComponent->isEnabled())
            {
                pComponent->onRender2d();
            }
        }
        for (auto& pChild : m_children)
        {
            if (pChild->isVisible())
            {
                pChild->render2d();
            }
        }
    }

    const OSceneManagerRef& Entity::getSceneManager() const
    {
        return m_pSceneManager;
    }

    const std::string Entity::getName() const
    {
        return m_name;
    }

    void Entity::setName(const std::string& name)
    {
        m_name = name;
    }

    void Entity::sendMessage(int messageId, void* pData)
    {
        for (auto& pComponent : m_components)
        {
            pComponent->onMessage(messageId, pData);
        }
    }

    void Entity::onTriggerEnter(const OCollider2DComponentRef& pCollider)
    {
        for (auto& pComponent : m_components)
        {
            pComponent->onTriggerEnter(pCollider);
        }
    }

    void Entity::onTriggerLeave(const OCollider2DComponentRef& pCollider)
    {
        for (auto& pComponent : m_components)
        {
            pComponent->onTriggerLeave(pCollider);
        }
    }
};
