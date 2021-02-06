#include <fstream>
#include <onut/Log.h>
#include <onut/ContentManager.h>
#include "Project.h"
#include "globals.h"
#include "PanelsManager.h"
#include "SceneViewPanel.h"

Project::Project()
{
}

Project::~Project()
{
}

void Project::openFolder(const std::string& in_path)
{
    path = in_path;
    g_content_mgr->clearSearchPaths();
    g_content_mgr->addSearchPath(path + "/assets");

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
    g_panels_mgr->document_zone->panels.push_back(OMake<SceneViewPanel>(filename));
    g_panels_mgr->document_zone->active_panel = (int)g_panels_mgr->document_zone->panels.size() - 1;
}
