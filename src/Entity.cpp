// Onut includes
#include <onut/Entity.h>

namespace onut
{
    OEntityRef Entity::create()
    {
        return std::shared_ptr<OEntity>(new OEntity());
    }

    Entity::Entity()
    {
    }

    Entity::~Entity()
    {
    }

    OEntityRef Entity::copy() const
    {
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
    }

    void Entity::remove(const OEntityRef& pChild)
    {
    }

    OEntityRef Entity::getParent() const
    {
        return m_pParent.lock();
    }

    void Entity::update()
    {
        m_childrenToRemove.clear();
        m_childrenToAdd.clear();
    }
    static std::vector<OEntityRef> m_childrenToRemove;
    static std::vector<OEntityRef> m_childrenToAdd;

    void Entity::updateTree()
    {
    }

    void Entity::render()
    {
    }

    void Entity::addComponent(const OComponentRef& pComponent)
    {
        m_components.push_back(pComponent);
    }
};
