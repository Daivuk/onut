#pragma once

#include <string>
#include <vector>
#include <onut/ForwardDeclaration.h>
#include "JsonHelper.h"

ForwardDeclare(SceneViewPanel);

class Project final
{
public:
    std::string                     path;
    Json::Value                     json;
    std::vector<SceneViewPanelRef>  loaded_scene_views;

    Project();
    ~Project();

    void openFolder(const std::string& path);
    bool openScene(const std::string& filename);
    std::string getSceneViewFilename();
};
