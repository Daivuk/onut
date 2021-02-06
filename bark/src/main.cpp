#include <onut/Files.h>
#include <onut/Log.h>
#include <onut/Settings.h>
#include <onut/Strings.h>
#include <onut/Timing.h>
#include <onut/ContentManager.h>
#include <json/json.h>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "Entity.h"
#include "Component.h"
#include "Script.h"
#include "Bindings.h"
#include "ComponentFactory.h"
#include "SceneManager.h"
#include "ScriptComponent.h"
#include "JSObject.h"
#include "globals.h"

static Json::Value project_json;
static std::string starting_scene;

void initSettings()
{
    oSettings->setAutoLoadScripts(false); // Don't load script the onut way. They will be loaded by components here

    // Load settings from project file
    {
        std::ifstream project_file("project.json");
        if (!project_file.is_open())
        {
            OLogE("Game's root directory must contain project.json");
            return;
        }

        try
        {
            project_file >> project_json;
        }
        catch (...)
        {
            project_file.close();
            OLogE("Failed to parse project.json");
            return;
        }
        project_file.close();

        auto json_settings = project_json["settings"];

        auto keys = json_settings.getMemberNames();
        for (const auto& key : keys)
        {
            auto what = onut::toUpper(key);
            const auto& json_value = json_settings[key];

            if (what == "RESOLUTION")
            {
                oSettings->setResolution({json_value["x"].asInt(), json_value["y"].asInt()});
            }
            else if (what == "RETRORESOLUTION")
            {
                oSettings->setRetroResolution({json_value["x"].asInt(), json_value["y"].asInt()});
            }
            else if (what == "RETROMODE")
            {
                oSettings->setIsRetroMode(json_value.asBool());
            }
            else if (what == "GAMENAME")
            {
                oSettings->setGameName(json_value.asString());
            }
            else if (what == "RESIZABLEWINDOW")
            {
                oSettings->setIsResizableWindow(json_value.asBool());
            }
            else if (what == "FULLSCREEN")
            {
                oSettings->setBorderlessFullscreen(json_value.asBool());
            }
            else if (what == "SHOWFPS")
            {
                oSettings->setShowFPS(json_value.asBool());
            }
            else if (what == "FIXEDSTEP")
            {
                oSettings->setIsFixedStep(json_value.asBool());
            }
            else if (what == "UPDATEFPS")
            {
                oTiming = OTiming::create((uint32_t)json_value.asUInt());
            }
            else if (what == "APPID")
            {
                oSettings->setAppId(json_value.asString());
            }
            else if (what == "APPSECRET")
            {
                oSettings->setAppSecret(json_value.asString());
            }
            else if (what == "ONSCREENLOG")
            {
                oSettings->setShowOnScreenLog(json_value.asBool());
            }
            else if (what == "EDITORMODE")
            {
                oSettings->setIsEditorMode(json_value.asBool());
            }
        }
    }

    starting_scene = project_json["startScene"].asString();
}

void init()
{
    g_content_mgr = oContentManager;

    JSObject::initJSObjects();
    initComponentFactory();
    createBindings();

    g_scene_mgr = new SceneManager();
    g_scene_mgr->loadScene(starting_scene);
}

void shutdown()
{
    delete g_scene_mgr;
    shutdownComponentFactory();
}

void update()
{
    g_scene_mgr->update(ODT);
}

void render()
{
    g_scene_mgr->render();
}

void postRender()
{
}

void renderUI()
{
}
