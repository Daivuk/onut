#include "Camera2DComponent.h"
#include "SceneManager.h"

void Camera2DComponent::onEnable()
{
    Component::onEnable();

    active_camera2Ds.push_back(this);
}

void Camera2DComponent::onDisable()
{
    Component::onDisable();

    auto it = std::find(active_camera2Ds.begin(), active_camera2Ds.end(), this);
    if (it != active_camera2Ds.end())
        active_camera2Ds.erase(it);
}
