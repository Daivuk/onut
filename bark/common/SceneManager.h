#pragma once

#include <onut/ForwardDeclaration.h>
#include <string>
#include <vector>
#include "JsonHelper.h"

class Component;
class Camera2DComponent;
class _2DRendererComponent;

#if !BARK_EDITOR
class b2World;
#else
class Gizmo2DRenderer;
class Gizmo2DContext;
#endif

ForwardDeclare(Entity);
ForwardDeclare(Component);

class SceneManager final
{
public:
    EntityRef   root;
    std::string scene_to_load;
    float       pixelsIn1Meter = 1.0f / 16.0f;

#if !BARK_EDITOR
    b2World*    world = nullptr;
#endif

    // Component lists
    std::vector<Component*> active_components;
    std::vector<Camera2DComponent*> active_camera2Ds;
    std::vector<_2DRendererComponent*> _2D_renderers;
#if BARK_EDITOR
    std::vector<Gizmo2DRenderer*> gizmo_2D_renderers;
#endif

    std::vector<ComponentRef> created_components;
    std::vector<ComponentRef> enabled_components;
    std::vector<ComponentRef> disabled_components;

    SceneManager();
    ~SceneManager();

#if !BARK_EDITOR
    void update(float dt);
#endif
    void render(Matrix* transform = nullptr);

#if BARK_EDITOR
    Json::Value serialize();
#endif
    void deserialize(const Json::Value& json);

    bool loadScene(const std::string& name);
    void loadNode(const EntityRef& entity, const Json::Value& json_node);
    void loadComponent(const EntityRef& entity, const std::string& type, const Json::Value& json_properties);

    Matrix getCamera2DTransform(Matrix* override_with) const;
    void clearScreen();
    void sort2DRenderers();
    void draw2DRenderers(const Matrix& transform);
#if BARK_EDITOR
    void draw2DGizmos(Matrix* override_with, Gizmo2DContext* ctx);
#endif
};
