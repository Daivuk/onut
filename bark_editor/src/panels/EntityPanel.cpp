#include "EntityPanel.h"
#include "SceneViewPanel.h"
#include "globals.h"
#include "PanelsManager.h"
#include "GUIContext.h"
#include "Entity.h"
#include "Theme.h"
#include "Component.h"

EntityPanel::EntityPanel()
{
    name = "Entity";
}

void EntityPanel::render(GUIContext* ctx)
{
    auto scene_view = ODynamicCast<SceneViewPanel>(g_panels_mgr->focussed_scene_view);
    if (!scene_view) return; // We display properties of selected entities, in the focused scene view
    
    ctx->drawArea();

    if (scene_view->selected_entities.empty())
    {
        ctx->drawText(ctx->theme->font, "Nothing Selected", ctx->rect.Center(), OCenter, ctx->theme->disabled_text_color);
        return;
    }

    const auto& entity = scene_view->selected_entities.front();

    ctx->beginVScrollArea(entity->scroll);
    {
        ctx->pushEnableState(false);
        int id_int = (int)entity->id; //TODO: Don't truncate Uint64
        ctx->int_prop(&id_int, "ID");
        ctx->popEnableState();

        ctx->std_string_prop(&entity->name, "Name");

        for (const auto& kv : entity->components)
        {
            if (ctx->header(kv.first, &kv.second->expanded))
            {
                kv.second->drawProperties(ctx);
            }
        }
    }
    ctx->endVScrollArea(&entity->scroll);
}
