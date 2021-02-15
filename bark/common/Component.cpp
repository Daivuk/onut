#include "Component.h"
#include "SceneManager.h"
#include "globals.h"
#include "Entity.h"

void Component::onEnable()
{
    entity->scene_mgr->active_components.push_back(this);
}

void Component::onDisable()
{
    auto it = std::find(entity->scene_mgr->active_components.begin(), entity->scene_mgr->active_components.end(), this);
    if (it != entity->scene_mgr->active_components.end())
        entity->scene_mgr->active_components.erase(it);
}

Rect Component::getWorldRect()
{
    return Rect(Vector2(entity->getWorldTransform().Translation()), Vector2::Zero);
}
