#include "Entity.h"
#include "GUIContext.h"
#include "SceneManager.h"
#include "Component.h"

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

void Entity::setWorldTransform(const Matrix& world_transform)
{
    Matrix parent_world;
    auto parent = getParent();
    if (parent) parent_world = parent->getWorldTransform();
    auto inv_parent_world = parent_world.Invert();

    m_local_transform = inv_parent_world * world_transform;
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

#if BARK_EDITOR
void Entity::drawProperties(GUIContext* ctx)
{
    ctx->pushEnableState(false);
    int id_int = (int)id; //TODO: Don't truncate Uint64
    ctx->int_prop(&id_int, "ID");
    ctx->popEnableState();

    ctx->std_string_prop(&name, "Name");

    auto pos = m_local_transform.Translation();
    if (ctx->Vector3_prop(&pos, "Position"))
    {
        m_local_transform.Translation(pos);
        is_world_dirty = true;
    }
}
#endif

#if BARK_EDITOR
Rect Entity::getWorldRect()
{
    if (components.empty()) return Rect(Vector2(getWorldTransform().Translation()), Vector2::Zero).Grow(16.0f);

    Rect rect;
    bool first = true;
    for (const auto& kv : components)
    {
        auto component_rect = kv.second->getWorldRect();
        if (first)
        {
            rect  = component_rect;
            first = false;
        }
        else 
        {
            rect = rect.Merge(component_rect);
        }
    }

    return rect;
}
#endif

#if BARK_EDITOR
Json::Value Entity::serialize()
{
    Json::Value json;
    
    setJson_uint64_t(json, "id", id);
    setJson_std_string(json, "name", name);
    setJson_Matrix(json, "transform", getLocalTransform());

    return std::move(json);
}
#endif

void Entity::deserialize(const Json::Value& in_json)
{
    json = in_json;

    id   = getJson_uint64_t(json, "id", 0);
    name = getJson_std_string(json, "name", "Entity");
    setLocalTransform(getJson_Matrix(json, "transform"));
}
