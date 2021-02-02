#pragma once

#include <onut/ForwardDeclaration.h>
#include <string>
#include <vector>

ForwardDeclare(Camera2DComponent);

void initSceneManager();
void shutdownSceneManager();

void loadScene(const std::string& name);

void updateSceneManager(float dt);
void renderSceneManager();

extern std::vector<Camera2DComponentRef> active_camera2Ds;
