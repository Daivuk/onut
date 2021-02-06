#pragma once

#include <onut/ForwardDeclaration.h>
#include <onut/Vector2.h>
#include "Panel.h"
#include "SceneManager.h"

OForwardDeclare(Texture);

class SceneViewPanel final : public Panel
{
public:
    SceneManager    scene_mgr;
    OTextureRef     game_content_rt;
    Vector2         camera_2d_pos   = Vector2::Zero;
    float           camera_2d_zoom  = 2.0f;

    SceneViewPanel(const std::string& filename);

    void render(GUIContext* ctx) override;
};
