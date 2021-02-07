#include <fstream>
#include <onut/Log.h>
#include <onut/Files.h>
#include <onut/ContentManager.h>
#include "Project.h"
#include "globals.h"
#include "PanelsManager.h"
#include "SceneViewPanel.h"
#include "Assets.h"

Project::Project()
{
}

Project::~Project()
{
    delete g_assets;
}

void Project::openFolder(const std::string& in_path)
{
    path = in_path;

    g_content_mgr->clearSearchPaths();
    g_content_mgr->addSearchPath(path + "/assets");

    delete g_assets;
    g_assets = new Assets(path + "/assets");

    // Load project file, or create one if not already there
    std::ifstream project_file(path + "/project.json");
    if (project_file.is_open())
    {
        try
        {
            project_file >> json;
        }
        catch (...)
        {
            project_file.close();
            OLogE("Failed to parse project.json");
            return;
        }
        project_file.close();
    }

    auto starting_scene = getJson_std_string(json, "startScene");
    if (!starting_scene.empty())
    {
        openScene(starting_scene);
    }
}

void Project::openScene(const std::string& filename)
{
    int i = 0;
    for (const auto& panel : g_panels_mgr->document_zone->panels)
    {
        auto scene_view_panel = ODynamicCast<SceneViewPanel>(panel);
        if (scene_view_panel && 
            onut::getFilename(scene_view_panel->filename) == onut::getFilename(filename))
        {
            // Already open, just focus
            g_panels_mgr->document_zone->active_panel = i;
            return;
        }
        ++i;
    }

    auto scene_view = OMake<SceneViewPanel>(filename);
    g_panels_mgr->document_zone->panels.push_back(scene_view);
    g_panels_mgr->document_zone->active_panel = (int)g_panels_mgr->document_zone->panels.size() - 1;
    g_panels_mgr->focussed_scene_view = scene_view;
}
