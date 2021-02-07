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

ForwardDeclare(Entity);
ForwardDeclare(Component);

class SceneManager;

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

    EntityRef getParent();
    void add(const EntityRef& child);
    
    SceneManager*           scene_mgr   = nullptr;
    uint64_t                id;
    std::string             name;
    std::vector<EntityRef>  children;
    std::map<std::string, ComponentRef> components;
#if BARK_EDITOR
    bool                    expanded    = false;
    bool                    selected    = false;
#endif

private:
    Matrix m_world_transform;
    Matrix m_local_transform;
    EntityWeak m_parent;

    bool is_world_dirty = true;
};
