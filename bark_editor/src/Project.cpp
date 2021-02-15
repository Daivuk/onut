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
            OLogE("Failed to parse project.json"); // It will initialize a default one
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

std::string Project::getSceneViewFilename()
{
    if (!g_panels_mgr->focussed_scene_view) return "";
    return g_panels_mgr->focussed_scene_view->filename;
}

bool Project::openScene(const std::string& filename)
{
    SceneViewPanelRef found_scene_view;
    for (const auto& scene_view_panel : loaded_scene_views)
    {
        if (onut::getFilename(scene_view_panel->filename) == onut::getFilename(filename))
        {
            found_scene_view = scene_view_panel;
            break;
        }
    }

    if (found_scene_view)
    {
        // Find it, focus it. If not in the UI, add it
        int index = -1;
        auto dock_zone = g_panels_mgr->find(found_scene_view, &index);
        if (dock_zone)
        {
            g_panels_mgr->document_zone->active_panel = index;
        }
        else 
        {
            g_panels_mgr->document_zone->panels.push_back(found_scene_view);
            g_panels_mgr->document_zone->active_panel = (int)g_panels_mgr->document_zone->panels.size() - 1;
        }
        g_panels_mgr->focussed_scene_view = found_scene_view;

        return true;
    }

    // Not found, load the scene
    auto scene_view = OMake<SceneViewPanel>(filename);
    if (!scene_view->valid) return false;

    loaded_scene_views.push_back(scene_view);
    g_panels_mgr->document_zone->panels.push_back(scene_view);
    g_panels_mgr->document_zone->active_panel = (int)g_panels_mgr->document_zone->panels.size() - 1;
    g_panels_mgr->focussed_scene_view = scene_view;

    return true;
}
