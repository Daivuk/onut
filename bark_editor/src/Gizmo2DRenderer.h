#pragma once

#include <onut/ForwardDeclaration.h>

class Entity;
class Gizmo2DContext;

class Gizmo2DRenderer
{
public:
    virtual ~Gizmo2DRenderer() {}

    void enableGizmo(Entity* entity);
    void disableGizmo(Entity* entity);

    virtual void renderGizmo(Gizmo2DContext* ctx) = 0;
};
