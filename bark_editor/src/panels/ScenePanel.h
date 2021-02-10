#pragma once

#include <onut/ForwardDeclaration.h>
#include "Panel.h"

ForwardDeclare(Entity);

class ScenePanel final : public Panel
{
public:
    float scroll = 0.0f;

    ScenePanel();

    void render(GUIContext* ctx) override;
    void renderEntity(GUIContext* ctx, const EntityRef& parent, int indent);

    void addSelection(GUIContext* ctx, const EntityRef& parent);
    void deselectAll(const EntityRef& entity);
};
