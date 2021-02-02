#pragma once

#include <onut/Matrix.h>
#include <onut/Vector3.h>
#include <onut/ForwardDeclaration.h>
#include <vector>
#include <string>
#include <map>
#include "JSObject.h"

ForwardDeclare(Entity);
ForwardDeclare(Component);

class Entity final : public JSObject, public std::enable_shared_from_this<Entity>
{
public:
    Entity();
    ~Entity();

    const Matrix& getWorldTransform();
    const Matrix& getLocalTransform() const;
    void setLocalTransform(const Matrix& local_transform);

    EntityRef getParent();
    void add(const EntityRef& child);

    std::string name;
    std::map<std::string, ComponentRef> components;
    std::vector<EntityRef> children;

private:
    Matrix m_world_transform;
    Matrix m_local_transform;
    EntityWeak m_parent;

    bool is_world_dirty = true;
};
