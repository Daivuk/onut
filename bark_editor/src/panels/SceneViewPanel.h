#pragma once

#include <vector>
#include <onut/ForwardDeclaration.h>
#include <onut/Vector2.h>
#include "Panel.h"
#include "SceneManager.h"
#include "Gizmo2DContext.h"

OForwardDeclare(Texture);
ForwardDeclare(Entity);

class SceneViewPanel final : public Panel, OEnableOThis(SceneViewPanel)
{
public:
    std::string     filename; // Asset's filename
    SceneManager    scene_mgr;
    Gizmo2DContext  gizmo_2D_context;
    OTextureRef     game_content_rt;
    Vector2         camera_2d_pos   = Vector2::Zero;
    int             camera_2d_zoom  = 6;
    bool            panning         = false;
    Vector2         camera_2d_pos_on_down;
    Vector2         pan_mouse_start;
    Vector2         mouse_pos;
    bool            valid           = true;
    Rect            rect;

    std::vector<EntityRef> selected_entities;

    SceneViewPanel(const std::string& filename);

    void render(GUIContext* ctx) override;

    void addSelectAction(std::vector<EntityRef> selection_before, std::vector<EntityRef> selection_after);
    void focusOn(const std::vector<EntityRef>& entities);
};
