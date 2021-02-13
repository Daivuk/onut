#pragma once

#include <onut/ForwardDeclaration.h>
#include <onut/Vector2.h>
#include "Panel.h"
#include "SceneManager.h"
#include <vector>

OForwardDeclare(Texture);
ForwardDeclare(Entity);

class SceneViewPanel final : public Panel
{
public:
    std::string     filename; // Asset's filename
    SceneManager    scene_mgr;
    OTextureRef     game_content_rt;
    Vector2         camera_2d_pos   = Vector2::Zero;
    int             camera_2d_zoom  = 3;
    bool            panning         = false;
    Vector2         camera_2d_pos_on_down;
    Vector2         pan_mouse_start;
    Vector2         mouse_pos;

    std::vector<EntityRef> selected_entities;

    SceneViewPanel(const std::string& filename);

    void render(GUIContext* ctx) override;
};
