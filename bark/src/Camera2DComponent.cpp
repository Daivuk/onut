#include "Camera2DComponent.h"
#include "SceneManager.h"

void Camera2DComponent::onEnable()
{
    active_camera2Ds.push_back(OThis);
}

void Camera2DComponent::onDisable()
{
    auto it = std::find(active_camera2Ds.begin(), active_camera2Ds.end(), OThis);
    if (it != active_camera2Ds.end())
        active_camera2Ds.erase(it);
}
