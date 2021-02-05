#include <onut/ContentManager.h>
#include <onut/Strings.h>
#include <onut/Files.h>
#include <onut/Log.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
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
#include "SpriteRendererComponent.h"

#include <box2d/b2_world.h>

// Component lists
std::vector<Component*> active_components;
std::vector<Camera2DComponent*> active_camera2Ds;
std::vector<_2DRendererComponent*> _2D_renderers;

std::vector<ComponentRef> created_components;
std::vector<ComponentRef> enabled_components;
std::vector<ComponentRef> disabled_components;

float pixelsIn1Meter = 1.0f / 16.0f;
b2World* world = nullptr;

std::string scene_to_load;

EntityRef root;

void loadNode(const EntityRef& entity, const Json::Value& json_node);
void loadComponent(const EntityRef& entity, const std::string& type, const Json::Value& json_properties);

void initSceneManager()
{
    world = new b2World(b2Vec2(0, 0));
    root = OMake<Entity>();
}

void shutdownSceneManager()
{
    created_components.clear();
    enabled_components.clear();
    disabled_components.clear();
    active_camera2Ds.clear();
    _2D_renderers.clear();
    root = nullptr;
    delete world;
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

    pixelsIn1Meter = getJson_float(json_scene, "pixelsIn1Meter", 16.0f);

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
    entity->name = getJson_std_string(json_node, "name", "Entity");
    entity->id = getJson_uint64_t(json_node, "id", 0);
    entity->setLocalTransform(getJson_Matrix(json_node, "transform"));

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
    ComponentRef component;

    if (onut::getExtension(type) == "JS")
    {
        // Special case for scripts, can have multiple of same type bound to the entity
        auto script_component = std::dynamic_pointer_cast<ScriptComponent>(createComponentByName("Script"));
        script_component->initJSObject(script_component->getJSPrototype());
        script_component->script = oContentManager->getResourceAs<Script>(type);
        script_component->addScriptPropertiesToJSObject();
        component = script_component;
    }
    else
    {
        component = createComponentByName(type);
    }

    if (component)
    {
        component->deserialize(json_properties);
        component->entity = entity.get();
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

    // Update active components
    {
        for (const auto& component : active_components)
        {
            component->onUpdate(dt);
        }
    }

    // Load scene if requested
    if (!scene_to_load.empty())
    {
        auto name = scene_to_load;
        scene_to_load.clear();
        loadScene(name);
    }
}

void renderSceneManager()
{
    auto r = oRenderer.get();
    auto& rs = r->renderStates;
    auto sb = oSpriteBatch.get();

    // Setup 2D camera
    Matrix camera_transform;
    if (!active_camera2Ds.empty())
    {
        auto active_camera2D = active_camera2Ds.back();
        if (active_camera2D->clearScreen)
        {
            r->clear(active_camera2D->clearColor);
        }

        auto pos = active_camera2D->entity->getWorldTransform().Translation();
        auto zoom = active_camera2D->zoom;
        const auto& origin = active_camera2D->origin;
        auto res = OScreenf;

        camera_transform = Matrix(
            zoom, 0, 0, 0,
            0, zoom, 0, 0,
            0, 0, zoom, 0,
            -pos.x * zoom + res.x * origin.x, -pos.y * zoom + res.y * origin.y, 0, 1);
    }

    //TODO: Cull 2D objects

    // Sort 2D renderers by z and y. Y is direct comparison, z is the "layer".
    std::sort(_2D_renderers.begin(), _2D_renderers.end(), 
              [](const _2DRendererComponent* a, const _2DRendererComponent* b) -> bool
    {
        const auto& a_t = a->entity->getWorldTransform();
        const auto& b_t = b->entity->getWorldTransform();
        if (a_t._43 < b_t._43) return true; // Smaller layer # always go before
        return a_t._42 < b_t._42; // y Sort
    });

    // Draw 2D elements
    sb->begin(camera_transform);
    for (auto _2d_renderer : _2D_renderers)
    {
        _2d_renderer->render(rs, sb);
    }
    sb->end();
}
