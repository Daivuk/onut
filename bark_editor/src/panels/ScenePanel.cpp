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
    ctx->drawArea();

    // Get focussed scene view panel
    if (!g_panels_mgr->focussed_scene_view) return;
    auto scene_view = ODynamicCast<SceneViewPanel>(g_panels_mgr->focussed_scene_view);
    if (!scene_view) return;
    auto root = scene_view->scene_mgr.root.get();

    ctx->rect = ctx->rect.Grow(-ctx->theme->border_size);

    ctx->beginVScrollArea(scroll);
    {
        ctx->rect.w = ctx->theme->list_item_height - 1;
        renderEntity(ctx, root, 0);
    }
    ctx->endVScrollArea(&scroll);
}

void ScenePanel::renderEntity(GUIContext* ctx, Entity* parent, int indent)
{
    if (ctx->drawListItem(parent->name, nullptr, indent, true, parent->selected) == eUIState::Clicked)
    {
        if (!ctx->ctrl && !ctx->shift) parent->expanded = !parent->expanded;
        //g_assets->addSelection(ctx, &dir);
    }

    ctx->rect.y += ctx->theme->list_item_height;

    for (auto child : parent->children)
    {
        renderEntity(ctx, child.get(), indent + 1);
    }
}
