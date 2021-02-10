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
#include "globals.h"

#if !BARK_EDITOR
#include <box2d/b2_world.h>
#endif

void loadNode(const EntityRef& entity, const Json::Value& json_node);
void loadComponent(const EntityRef& entity, const std::string& type, const Json::Value& json_properties);

SceneManager::SceneManager()
{
#if !BARK_EDITOR
    world = new b2World(b2Vec2(0, 0));
#endif
    root = OMake<Entity>();
    root->scene_mgr = this;
}

SceneManager::~SceneManager()
{
#if !BARK_EDITOR
    delete world;
#endif
}

#if BARK_EDITOR
Json::Value SceneManager::serialize()
{
    Json::Value json;;
    return std::move(json);
}
#endif

void SceneManager::deserialize(const Json::Value& json)
{
}

void SceneManager::loadScene(const std::string& name)
{
    Json::Value json_scene;

    auto full_path = g_content_mgr->findResourceFile(name);
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
    if (root->name == "Entity") root->name = "root";
    root->is_scene_root = true;

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
}

void SceneManager::loadNode(const EntityRef& entity, const Json::Value& json_node)
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
        child->scene_mgr = this;
        loadNode(child, json_child);
        entity->add(child);
    }
}

void SceneManager::loadComponent(const EntityRef& entity, const std::string& type, const Json::Value& json_properties)
{
    ComponentRef component;

    if (onut::getExtension(type) == "JS")
    {
        // Special case for scripts, can have multiple of same type bound to the entity
        auto script_component = std::dynamic_pointer_cast<ScriptComponent>(createComponentByName("Script"));
#if !BARK_EDITOR
        script_component->initJSObject(script_component->getJSPrototype());
#endif
        script_component->script = g_content_mgr->getResourceAs<Script>(type);
#if !BARK_EDITOR
        script_component->addScriptPropertiesToJSObject();
#endif
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

#if !BARK_EDITOR
void SceneManager::update(float dt)
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
#if !BARK_EDITOR
    {
        for (const auto& component : active_components)
        {
            component->onUpdate(dt);
        }
    }
#endif

    // Load scene if requested
    if (!scene_to_load.empty())
    {
        auto name = scene_to_load;
        scene_to_load.clear();
        loadScene(name);
    }
}
#endif

void SceneManager::render(Matrix* transform)
{
    auto r = oRenderer.get();
    auto& rs = r->renderStates;
    auto sb = oSpriteBatch.get();

    // Setup 2D camera
    Matrix camera_transform;
    if (transform)
    {
        camera_transform = *transform;
    }
#if !BARK_EDITOR
    else if (!active_camera2Ds.empty())
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
#endif

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
