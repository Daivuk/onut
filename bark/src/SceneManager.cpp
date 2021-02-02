#include <onut/ContentManager.h>
#include <onut/Strings.h>
#include <onut/Log.h>
#include <onut/Renderer.h>
#include <fstream>
#include <vector>
#include "SceneManager.h"
#include "JsonHelper.h"
#include "Entity.h"
#include "Component.h"
#include "ComponentFactory.h"
#include "Script.h"
#include "ScriptComponent.h"
#include "Camera2DComponent.h"

std::vector<Camera2DComponentRef> active_camera2Ds;
std::vector<ComponentRef> created_components;
std::vector<ComponentRef> enabled_components;
std::vector<ComponentRef> disabled_components;
std::string scene_to_load;

static EntityRef root;

void loadNode(const EntityRef& entity, const Json::Value& json_node);
void loadComponent(const EntityRef& entity, const std::string& type, const Json::Value& json_properties);

void initSceneManager()
{
    root = OMake<Entity>();
}

void shutdownSceneManager()
{
    created_components.clear();
    enabled_components.clear();
    disabled_components.clear();
    active_camera2Ds.clear();
    root = nullptr;
}

void loadScene(const std::string& name)
{
    Json::Value json_scene;

    auto full_path = oContentManager->findResourceFile(name);
    if (full_path.empty())
    {
        OLogE("Cannot find scene: " + name);
        return;
    }

    std::fstream file(full_path);
    if (!file.is_open())
    {
        OLogE("Failed to load scene: " + name);
        return;
    }
    else
    {
        try
        {
            file >> json_scene;
        }
        catch (...)
        {
            OLogE("Failed to parse scene: " + name);
            file.close();
            return;
        }
        file.close();
    }

    root->children.clear();
    root->components.clear();
    loadNode(root, json_scene);

    // Call onCreate
    {
        static decltype(created_components) copy;
        copy = created_components;
        created_components.clear();
        for (const auto& component : copy)
        {
            component->onCreate();
            if (component->enabled) enabled_components.push_back(component);
        }
        copy.clear();
    }
}

void loadNode(const EntityRef& entity, const Json::Value& json_node)
{
    entity->name = json_node["name"].asString();

    const auto& json_components = json_node["components"];
    auto component_names = json_components.getMemberNames();
    for (const auto& key : component_names)
    {
        const auto& json_properties = json_components[key];
        loadComponent(entity, key, json_properties);
    }

    const auto& json_children = json_node["children"];
    for (const auto& json_child : json_children)
    {
        auto child = OMake<Entity>();
        loadNode(child, json_child);
        entity->add(child);
    }
}

void loadComponent(const EntityRef& entity, const std::string& type, const Json::Value& json_properties)
{
    ComponentRef component = createComponentByName(type);

    if (component)
    {
        component->deserialize(json_properties);
        component->entity = entity;
        entity->components[type] = component;
        created_components.push_back(component);
    }
}

void updateSceneManager(float dt)
{
    // Call onEnabled
    {
        static decltype(enabled_components) copy;
        copy = enabled_components;
        enabled_components.clear();
        for (const auto& component : copy)
        {
            component->onEnable();
        }
        copy.clear();
    }

    // Call onDisabled
    {
        static decltype(disabled_components) copy;
        copy = disabled_components;
        disabled_components.clear();
        for (const auto& component : copy)
        {
            component->onDisable();
        }
        copy.clear();
    }

    if (!scene_to_load.empty())
    {
        auto name = scene_to_load;
        scene_to_load.clear();
        loadScene(name);
    }
}

void renderSceneManager()
{
    if (!active_camera2Ds.empty())
    {
        const auto& active_camera2D = active_camera2Ds.back();
        if (active_camera2D->clearScreen)
            oRenderer->clear(active_camera2D->clearColor);
    }
}
