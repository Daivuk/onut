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
};
