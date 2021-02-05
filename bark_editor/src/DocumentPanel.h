#pragma once

#include <onut/ForwardDeclaration.h>
#include "Panel.h"

OForwardDeclare(Texture);

class DocumentPanel final : public Panel
{
public:
    DocumentPanel(const std::string& name);

    void renderGameContent(GUIContext* ctx) override;
    void render(GUIContext* ctx) override;

    OTextureRef game_content_rt;
    Vector2     camera_2d_pos   = Vector2::Zero;
    float       camera_2d_zoom  = 2.0f;
};
