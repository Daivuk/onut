// Onut includes
#include <onut/Component.h>
#include <onut/Entity.h>
#include <onut/SceneManager.h>

namespace onut
{
    Component::Component(int flags)
        : m_flags(flags)
    {
    }

    Component::~Component()
    {
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
        if (m_pEntity->m_pSceneManager && m_flags & FLAG_UPDATABLE)
        {
            if (m_pEntity->isEnabled() && !m_pEntity->isStatic())
            {
                if (m_isEnabled && !isEnabled)
                {
                    m_pEntity->m_pSceneManager->addComponentAction(OThis, m_pEntity->m_pSceneManager->m_componentUpdates, SceneManager::ComponentAction::Action::Remove);
                }
                else if (!m_isEnabled && isEnabled)
                {
                    m_pEntity->m_pSceneManager->addComponentAction(OThis, m_pEntity->m_pSceneManager->m_componentUpdates, SceneManager::ComponentAction::Action::Add);
                }
            }
        }
        m_isEnabled = isEnabled;
    }

    const OSceneManagerRef& Component::getSceneManager() const
    {
        return getEntity()->getSceneManager();
    }

    void Component::sendMessage(int messageId, void* pData)
    {
        getEntity()->sendMessage(messageId, pData);
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
