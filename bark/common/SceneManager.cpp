#include <onut/ContentManager.h>
#include <onut/Strings.h>
#include <onut/Files.h>
#include <onut/Log.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include <onut/PrimitiveBatch.h>
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
#include "Theme.h"

#if !BARK_EDITOR
#include <box2d/b2_world.h>
#else
#include "Gizmo2DRenderer.h"
#include "Gizmo2DContext.h"
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

bool SceneManager::loadScene(const std::string& name)
{
    Json::Value json_scene;

    auto full_path = g_content_mgr->findResourceFile(name);
    if (full_path.empty())
    {
        OLogE("Cannot find scene: " + name);
        return false;
    }

    std::fstream file(full_path);
    if (!file.is_open())
    {
        OLogE("Failed to load scene: " + name);
        return false;
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
            return false;
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

    return true;
}

void SceneManager::loadNode(const EntityRef& entity, const Json::Value& json)
{
    entity->scene_mgr = this;
    entity->deserialize(json);

    const auto& json_components = json["components"];
    auto component_names = json_components.getMemberNames();
    for (const auto& key : component_names)
    {
        const auto& json_properties = json_components[key];
        loadComponent(entity, key, json_properties);
    }

    const auto& json_children = json["children"];
    for (const auto& json_child : json_children)
    {
        auto child = OMake<Entity>();
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

Matrix SceneManager::getCamera2DTransform(Matrix* override_with) const
{
    Matrix camera_transform;
    if (override_with)
    {
        camera_transform = *override_with;
    }
#if !BARK_EDITOR
    else if (!active_camera2Ds.empty())
    {
        auto active_camera2D = active_camera2Ds.back();
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
    return camera_transform;
}

void SceneManager::clearScreen()
{
    bool should_clear_screen = false;
    Color clear_color = OColorHex(262c3b);
    if (!active_camera2Ds.empty())
    {
        auto active_camera2D = active_camera2Ds.back();
        should_clear_screen = active_camera2D->clearScreen;
        if (active_camera2D->clearScreen)
        {
            should_clear_screen = true;
            clear_color         = active_camera2D->clearColor;
        }
    }
#if !BARK_EDITOR
    if (should_clear_screen)
#endif
    {
        oRenderer->clear(clear_color);
    }
}

void SceneManager::sort2DRenderers()
{
    std::sort(_2D_renderers.begin(), _2D_renderers.end(), 
              [](const _2DRendererComponent* a, const _2DRendererComponent* b) -> bool
    {
        const auto& a_t = a->entity->getWorldTransform();
        const auto& b_t = b->entity->getWorldTransform();

        //TODO: Make this configurable for the scene
        if (a_t._43 < b_t._43) return true; // z Sort (layers)
        return (a_t._42 < b_t._42);         // y Sort
    });
}

void SceneManager::draw2DRenderers(const Matrix& transform)
{
    auto  r     = oRenderer.get();
    auto& rs    = r->renderStates;
    auto  sb    = oSpriteBatch.get();

    sb->begin(transform);
    for (auto _2d_renderer : _2D_renderers)
    {
        _2d_renderer->render(rs, sb);
    }
    sb->end();
}

#if BARK_EDITOR
void SceneManager::draw2DGizmos(Matrix* override_with, Gizmo2DContext* ctx)
{
    auto transform = getCamera2DTransform(override_with);

    auto  r     = oRenderer.get();
    auto& rs    = r->renderStates;
    auto  pb    = oPrimitiveBatch.get();

    rs.sampleFiltering  = OFilterNearest;
    rs.blendMode        = OBlendPreMultiplied;

    // Grid
    {
        auto inv_transform = transform.Invert();

        Vector2 top_left        = Vector2::Transform(Vector2::Zero, inv_transform);
        Vector2 bottom_right    = Vector2::Transform(OScreenf, inv_transform);

        pb->draw(Vector2(top_left.x, 0), Color(0.35f));
        pb->draw(Vector2(bottom_right.x, 0), Color(0.35f));
        pb->draw(Vector2(0, top_left.y), Color(0.35f));
        pb->draw(Vector2(0, bottom_right.y), Color(0.35f));
    }

    for (auto gizmo_renderer : gizmo_2D_renderers)
    {
        gizmo_renderer->renderGizmo(ctx);
    }
}
#endif

void SceneManager::render(Matrix* transform)
{
    auto camera2D_transform = getCamera2DTransform(transform);
    clearScreen();
    //TODO: Cull 2D objects
    sort2DRenderers();
    draw2DRenderers(camera2D_transform);
}
