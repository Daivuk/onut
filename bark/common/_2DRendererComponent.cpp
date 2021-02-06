#include "_2DRendererComponent.h"
#include "SceneManager.h"
#include "globals.h"
#include "Entity.h"

void _2DRendererComponent::onEnable()
{
    entity->scene_mgr->_2D_renderers.push_back(this);
}

void _2DRendererComponent::onDisable()
{
    auto it = std::find(entity->scene_mgr->_2D_renderers.begin(), entity->scene_mgr->_2D_renderers.end(), this);
    if (it != entity->scene_mgr->_2D_renderers.end())
        entity->scene_mgr->_2D_renderers.erase(it);
}
