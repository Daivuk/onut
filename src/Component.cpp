// Onut includes
#include <onut/Component.h>
#include <onut/Entity.h>
#include <onut/SceneManager.h>

// STL
#include <atomic>

namespace onut
{
#if defined(_DEBUG)
    std::atomic<int> g_componentCount;
#endif

    Component::Component(int flags)
        : m_flags(flags)
    {
#if defined(_DEBUG)
        ++g_componentCount;
#endif
    }

    Component::~Component()
    {
#if defined(_DEBUG)
        --g_componentCount;
#endif
    }

    const OEntityRef& Component::getEntity() const
    {
        return m_pEntity;
    }

    bool Component::isEnabled() const
    {
        return m_isEnabled;
    }

    void Component::setEnabled(bool isEnabled)
    {
        if (m_isEnabled == isEnabled) return;
        if (m_pEntity->m_pSceneManager && m_flags & FLAG_UPDATABLE)
        {
            if (m_pEntity->isEnabled() && !m_pEntity->isStatic())
            {
                if (m_isEnabled)
                {
                    m_pEntity->m_pSceneManager->addComponentAction(OThis, SceneManager::ComponentAction::Action::RemoveUpdate);
                }
                else if (!m_isEnabled)
                {
                    m_pEntity->m_pSceneManager->addComponentAction(OThis, SceneManager::ComponentAction::Action::AddUpdate);
                }
            }
            if (m_pEntity->isVisible())
            {
                if (m_flags & FLAG_RENDERABLE)
                {
                    if (m_isEnabled)
                    {
                        m_pEntity->m_pSceneManager->addComponentAction(OThis, SceneManager::ComponentAction::Action::RemoveRender);
                    }
                    else if (!m_isEnabled)
                    {
                        m_pEntity->m_pSceneManager->addComponentAction(OThis, SceneManager::ComponentAction::Action::AddRender);
                    }
                }
                if (m_flags & FLAG_RENDERABLE_2D)
                {
                    if (m_isEnabled)
                    {
                        m_pEntity->m_pSceneManager->addComponentAction(OThis, SceneManager::ComponentAction::Action::RemoveRender2D);
                    }
                    else if (!m_isEnabled)
                    {
                        m_pEntity->m_pSceneManager->addComponentAction(OThis, SceneManager::ComponentAction::Action::AddRender2D);
                    }
                }
            }
        }

        m_isEnabled = isEnabled;

        if (m_isEnabled)
        {
            onEnable();
        }
        else
        {
            onDisable();
        }
    }

    const OSceneManagerRef& Component::getSceneManager() const
    {
        return getEntity()->getSceneManager();
    }

    void Component::sendMessage(int messageId, void* pData)
    {
        getEntity()->sendMessage(messageId, pData);
    }

    void Component::boardcastMessage(int messageId, void* pData)
    {
        getEntity()->getSceneManager()->boardcastMessage(messageId, pData);
    }

    void Component::destroy()
    {
        getEntity()->destroy();
    }

    const Matrix& Component::getLocalTransform() const
    {
        return getEntity()->getLocalTransform();
    }

    const Matrix& Component::getWorldTransform()
    {
        return getEntity()->getWorldTransform();
    }

    void Component::setLocalTransform(const Matrix& localTransform)
    {
        getEntity()->setLocalTransform(localTransform);
    }

    void Component::setWorldTransform(const Matrix& worldTransform)
    {
        getEntity()->setWorldTransform(worldTransform);
    }
};
