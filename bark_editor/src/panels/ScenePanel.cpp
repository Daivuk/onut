#include <algorithm>
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
    auto root = scene_view->scene_mgr.root;

    ctx->rect = ctx->rect.Grow(-ctx->theme->border_size);

    ctx->beginVScrollArea(scroll);
    {
        ctx->rect.w = ctx->theme->list_item_height - 1;
        renderEntity(ctx, root, 0);
    }
    ctx->endVScrollArea(&scroll);
}

void ScenePanel::renderEntity(GUIContext* ctx, const EntityRef& entity, int indent)
{
    if (entity->is_scene_root)
    {
        for (auto child : entity->children)
        {
            renderEntity(ctx, child, indent);
        }
    }
    else
    {
        auto state = ctx->drawListItem(entity->name, nullptr, indent, true, entity->selected);
        if (state == eUIState::Clicked)
        {
            if (!ctx->keys.ctrl && !ctx->keys.shift) entity->expanded = !entity->expanded;
            addSelection(ctx, entity);
        }
        else if (state == eUIState::DoubleClicked)
        {
            if (!ctx->keys.ctrl && !ctx->keys.shift) focusOn(entity);
        }

        ctx->rect.y += ctx->theme->list_item_height;

        for (auto child : entity->children)
        {
            renderEntity(ctx, child, indent + 1);
        }
    }
}

void ScenePanel::deselectAll(const EntityRef& entity)
{
    entity->selected = false;
    for (auto child : entity->children)
    {
        deselectAll(child);
    }
}

void ScenePanel::addSelection(GUIContext* ctx, const EntityRef& entity)
{
    auto scene_view = g_panels_mgr->focussed_scene_view;
    if (!scene_view) return;
    auto root = scene_view->scene_mgr.root;

    auto selection_before = scene_view->selected_entities;

    if (ctx->keys.ctrl)
    {
        if (ctx->keys.shift)
        {
        }
        else
        {
            entity->selected = !entity->selected;
            if (entity->selected) scene_view->selected_entities.push_back(entity);
            else scene_view->selected_entities.erase(std::remove(scene_view->selected_entities.begin(), scene_view->selected_entities.end(), entity));
        }
    }
    else
    {
        if (ctx->keys.shift)
        {
        }
        else
        {
            deselectAll(root);
            entity->selected = true;
            scene_view->selected_entities = { entity };
        }
    }

    scene_view->addSelectAction(selection_before, scene_view->selected_entities);
}
