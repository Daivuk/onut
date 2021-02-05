#include "_2DRendererComponent.h"
#include "SceneManager.h"

void _2DRendererComponent::onEnable()
{
    _2D_renderers.push_back(this);
}

void _2DRendererComponent::onDisable()
{
    auto it = std::find(_2D_renderers.begin(), _2D_renderers.end(), this);
    if (it != _2D_renderers.end())
        _2D_renderers.erase(it);
}
