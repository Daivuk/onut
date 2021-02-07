#include "ScenePanel.h"
#include "GUIContext.h"
#include "Theme.h"
#include "SceneManager.h"
#include "globals.h"
#include "PanelsManager.h"
#include "SceneViewPanel.h"
#include "Entity.h"

ScenePanel::ScenePanel()
{
    name = "Scene";
}

void ScenePanel::render(GUIContext* ctx)
{
    // Get focussed scene view panel
    if (!g_panels_mgr->focussed_scene_view) return;
    auto scene_view = ODynamicCast<SceneViewPanel>(g_panels_mgr->focussed_scene_view);
    if (!scene_view) return;
    auto root = scene_view->scene_mgr.root.get();

    ctx->drawArea();
    ctx->rect = ctx->rect.Grow(-ctx->theme->border_size);

    bool redraw = true;
    auto draw_point = ctx->draw_calls.size();
    while (redraw)
    {
        ctx->draw_calls.resize(draw_point);
        redraw = false;

        GUI_START_V_SCROLLABLE(ctx, scroll);
        {
            ctx->rect.w = ctx->theme->list_item_height - 1;

            if (renderEntity(ctx, root, 0))
            {
                ctx->clicked = false; // Otherwise infinite loop
                redraw = true;
            }
        }
        GUI_END_V_SCROLLABLE(ctx, scroll);
    }
}

bool ScenePanel::renderEntity(GUIContext* ctx, Entity* parent, int indent)
{
    bool ret = false;

    if (ctx->drawListItem(parent->name, nullptr, indent, true, parent->selected) == eUIState::Clicked)
    {
        if (!ctx->ctrl && !ctx->shift) parent->expanded = !parent->expanded;
        //g_assets->addSelection(ctx, &dir);
        ret = true;
    }

    ctx->rect.y += ctx->theme->list_item_height;

    for (auto child : parent->children)
    {
        renderEntity(ctx, child.get(), indent + 1);
    }

    return ret;
}
