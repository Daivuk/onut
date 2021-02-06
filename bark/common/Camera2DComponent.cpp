#include "Camera2DComponent.h"
#include "SceneManager.h"
#include "globals.h"
#include "Entity.h"

void Camera2DComponent::onEnable()
{
    Component::onEnable();

    entity->scene_mgr->active_camera2Ds.push_back(this);
}

void Camera2DComponent::onDisable()
{
    Component::onDisable();

    auto it = std::find(entity->scene_mgr->active_camera2Ds.begin(), entity->scene_mgr->active_camera2Ds.end(), this);
    if (it != entity->scene_mgr->active_camera2Ds.end())
        entity->scene_mgr->active_camera2Ds.erase(it);
}
