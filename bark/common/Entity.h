#pragma once

#include <onut/Matrix.h>
#include <onut/Vector3.h>
#include <onut/ForwardDeclaration.h>
#include <vector>
#include <string>
#include <map>
#if !BARK_EDITOR
#include "JSObject.h"
#endif
#include "JsonHelper.h"

ForwardDeclare(Entity);
ForwardDeclare(Component);

class SceneManager;
#if BARK_EDITOR
class GUIContext;
#endif

class Entity final : 
#if !BARK_EDITOR
    public JSObject, 
#endif
    public std::enable_shared_from_this<Entity>
{
public:
    Entity();
    ~Entity();

    const Matrix& getWorldTransform();
    const Matrix& getLocalTransform() const;
    void setLocalTransform(const Matrix& local_transform);
    void setWorldTransform(const Matrix& world_transform);

    EntityRef getRoot();
    EntityRef getParent();
    void add(const EntityRef& child);
    
    SceneManager*                       scene_mgr   = nullptr;
    uint64_t                            id;
    std::string                         name;
    std::vector<EntityRef>              children;
    std::map<std::string, ComponentRef> components;
    bool                                is_scene_root   = false;
#if BARK_EDITOR
    bool                                expanded        = false;
    bool                                selected        = false;
    float                               scroll          = 0.0f; // We remember the scroll location for each entity's property page :)
    Json::Value                         json;
#endif

#if BARK_EDITOR
    void drawProperties(GUIContext* ctx);
    Rect getWorldRect();
#endif

#if BARK_EDITOR
    Json::Value serialize();
#endif
    void deserialize(const Json::Value& json);

private:
    Matrix      m_world_transform;
    Matrix      m_local_transform;
    EntityWeak  m_parent;
    bool        is_world_dirty = true;
};
