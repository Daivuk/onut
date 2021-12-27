#include "Camera2DComponent.h"
#include "SceneManager.h"
#include "globals.h"
#include "Entity.h"
#include "TransformHelper.h"
#if BARK_EDITOR
#include <onut/Renderer.h>
#include "Project.h"
#include "JsonHelper.h"
#include "Gizmo2DContext.h"
#else
#include <onut/Renderer.h>
#endif

void Camera2DComponent::onEnable()
{
    Component::onEnable();
#if BARK_EDITOR
    enableGizmo(entity);
#endif

    entity->scene_mgr->active_camera2Ds.push_back(this);
}

void Camera2DComponent::onDisable()
{
    Component::onDisable();
#if BARK_EDITOR
    disableGizmo(entity);
#endif

    auto it = std::find(entity->scene_mgr->active_camera2Ds.begin(), entity->scene_mgr->active_camera2Ds.end(), this);
    if (it != entity->scene_mgr->active_camera2Ds.end())
        entity->scene_mgr->active_camera2Ds.erase(it);
}

#if BARK_EDITOR
void Camera2DComponent::renderGizmo(Gizmo2DContext* ctx)
{
    if (!entity->selected) return;

    // Get resolution from the project's settings
    Point res = getJson_Point(g_project->json, "resolution", oRenderer->getResolution());
    if (getJson_bool(g_project->json, "retroMode", false))
    {
        res = getJson_Point(g_project->json, "retroResolution", {256, 240});
    }

    const auto& transform = entity->getWorldTransform();
    auto sizef = Vector2((float)res.x, (float)res.y) * (1.0f / zoom);

    ctx->drawDottedRect(transform, sizef, origin);
    ctx->drawOrigin(transform);
}
#endif

Rect Camera2DComponent::getWorldRect()
{
#if BARK_EDITOR
    // Get resolution from the project's settings
    Point res = getJson_Point(g_project->json, "resolution", oRenderer->getResolution());
    if (getJson_bool(g_project->json, "retroMode", false))
    {
        res = getJson_Point(g_project->json, "retroResolution", {256, 240});
    }
#else
    Point res = OScreen;
#endif

    const auto& transform = entity->getWorldTransform();
    auto sizef = Vector2((float)res.x, (float)res.y) * (1.0f / zoom);

    return TransformHelper::getWorldRect(transform, sizef, origin);
}
