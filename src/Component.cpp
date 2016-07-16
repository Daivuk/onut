// Onut includes
#include <onut/Component.h>
#include <onut/Entity.h>
#include <onut/EntityManager.h>

namespace onut
{
    Component::Component()
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
        if (m_pEntity->m_pEntityManager)
        {
            if (m_pEntity->isEnabled() && !m_pEntity->isStatic())
            {
                if (m_isEnabled && !isEnabled)
                {
                    m_pEntity->m_pEntityManager->addComponentAction(OThis, m_pEntity->m_pEntityManager->m_componentUpdates, EntityManager::ComponentAction::Action::Remove);
                }
                else if (!m_isEnabled && isEnabled)
                {
                    m_pEntity->m_pEntityManager->addComponentAction(OThis, m_pEntity->m_pEntityManager->m_componentUpdates, EntityManager::ComponentAction::Action::Add);
                }
            }
        }
        m_isEnabled = isEnabled;
    }

    const OEntityManagerRef& Component::getEntityManager() const
    {
        return getEntity()->getEntityManager();
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
