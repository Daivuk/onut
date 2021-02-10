#include "Entity.h"

#if !BARK_EDITOR
extern void* pEntityPrototype;
#endif

Entity::Entity()
{
#if !BARK_EDITOR
    initJSObject(pEntityPrototype);
#endif
}

Entity::~Entity()
{
}

const Matrix& Entity::getWorldTransform()
{
    if (is_world_dirty)
    {
        Matrix parent_world;
        auto parent = getParent();
        if (parent)
        {
            parent_world = parent->getWorldTransform();
        }
        m_world_transform = parent_world * m_local_transform;
        is_world_dirty = false;
    }
    return m_world_transform;
}

const Matrix& Entity::getLocalTransform() const
{
    return m_local_transform;
}

void Entity::setLocalTransform(const Matrix& local_transform)
{
    m_local_transform = local_transform;
    is_world_dirty = true;
}

EntityRef Entity::getRoot()
{
    auto parent = getParent();
    if (!parent) return OThis;
    return parent->getRoot();
}

EntityRef Entity::getParent()
{
    return m_parent.lock();
}

void Entity::add(const EntityRef& child)
{
    // Make sure he's not already child!
    if (this == child->getParent().get()) return;

    children.push_back(child);
    child->m_parent = OThis;
}
