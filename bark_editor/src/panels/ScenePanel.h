#pragma once

#include "Panel.h"

class Entity;

class ScenePanel final : public Panel
{
public:
    float scroll = 0.0f;

    ScenePanel();

    void render(GUIContext* ctx) override;
    void renderEntity(GUIContext* ctx, Entity* parent, int indent);
};
