#include "Component.h"
#include "SceneManager.h"

void Component::onEnable()
{
    active_components.push_back(this);
}

void Component::onDisable()
{
    auto it = std::find(active_components.begin(), active_components.end(), this);
    if (it != active_components.end())
        active_components.erase(it);
}
