#pragma once

#include <onut/ForwardDeclaration.h>

class Entity;
OForwardDeclare(PrimitiveBatch);
namespace onut
{
    class RenderStates;
};

class Gizmo2DRenderer
{
public:
    virtual ~Gizmo2DRenderer() {}

    void enableGizmo(Entity* entity);
    void disableGizmo(Entity* entity);

    virtual void renderGizmo(onut::RenderStates& rs, OPrimitiveBatch* pb) = 0;
};
