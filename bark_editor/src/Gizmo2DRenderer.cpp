#include <onut/PrimitiveBatch.h>
#include "Gizmo2DRenderer.h"
#include "SceneManager.h"
#include "globals.h"
#include "Entity.h"

void Gizmo2DRenderer::enableGizmo(Entity* entity)
{
    entity->scene_mgr->gizmo_2D_renderers.push_back(this);
}

void Gizmo2DRenderer::disableGizmo(Entity* entity)
{
    auto it = std::find(entity->scene_mgr->gizmo_2D_renderers.begin(), entity->scene_mgr->gizmo_2D_renderers.end(), this);
    if (it != entity->scene_mgr->gizmo_2D_renderers.end())
        entity->scene_mgr->gizmo_2D_renderers.erase(it);
}
