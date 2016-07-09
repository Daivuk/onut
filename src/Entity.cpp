// Onut includes
#include <onut/Entity.h>
#include <onut/EntityManager.h>

namespace onut
{
    OEntityRef Entity::create(const OEntityManagerRef& in_pEntityManager)
    {
        auto pEntityManager = in_pEntityManager;
        if (!pEntityManager) pEntityManager = oEntityManager;
        auto pEntity = std::shared_ptr<OEntity>(new OEntity());
        pEntityManager->add(pEntity);
        return pEntity;
    }

    Entity::Entity()
    {
    }

    Entity::~Entity()
    {
    }

    void Entity::destroy()
    {
        m_pEntityManager->remove(OThis);
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
        assert(false); // Todo
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
        pChild->m_pParent = OThis;
    }

    void Entity::remove(const OEntityRef& pChild)
    {
        for (auto it = m_children.begin(); it != m_children.end(); ++it)
        {
            if (*it == pChild)
            {
                pChild->m_pParent.reset();
                m_children.erase(it);
                return;
            }
        }
    }

    OEntityRef Entity::getParent() const
    {
        return m_pParent.lock();
    }
};
