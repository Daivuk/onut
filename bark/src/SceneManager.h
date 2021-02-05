#pragma once

#include <onut/ForwardDeclaration.h>
#include <string>
#include <vector>

class Component;
class Camera2DComponent;
class _2DRendererComponent;

ForwardDeclare(Entity);

extern std::vector<Camera2DComponent*> active_camera2Ds;
extern std::vector<_2DRendererComponent*> _2D_renderers;
extern std::vector<Component*> active_components;
extern EntityRef root;

void initSceneManager();
void shutdownSceneManager();

void loadScene(const std::string& name);

void updateSceneManager(float dt);
void renderSceneManager();
