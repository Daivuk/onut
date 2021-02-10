#include <onut/ActionManager.h>
#include "ProjectPanel.h"
#include "GUIContext.h"
#include "Theme.h"
#include "Project.h"
#include "globals.h"

ProjectPanel::ProjectPanel()
{
    name = "Project";
}

void ProjectPanel::render(GUIContext* ctx)
{
    if (!g_project) return;

    bool changes = false;
    
    if (ctx->edit_state == GUIContext::State::None) original_json = g_project->json;

    ctx->drawArea();

    ctx->beginVScrollArea(scroll);
    {
        auto& json = g_project->json;

        {
            auto value = getJson_std_string(json, "gameName", "Untitled Game");
            changes |= ctx->std_string_prop(&value, "Game name");
            setJson_std_string(json, "gameName", value);
        }
        {
            auto value = getJson_Point(json, "resolution", {800, 600});
            changes |= ctx->Point_prop(&value, "Resolution");
            setJson_Point(json, "resolution", value);
        }
        {
            auto retro_mode = getJson_bool(json, "retroMode", false);
            changes |= ctx->bool_prop(&retro_mode, "Retro mode");
            setJson_bool(json, "retroMode", retro_mode);
            
            ctx->indent();
            ctx->pushEnableState(retro_mode);
            auto value = getJson_Point(json, "retroResolution", {256, 160});
            changes |= ctx->Point_prop(&value, "Retro resolution");
            setJson_Point(json, "retroResolution", value);
            ctx->popEnableState();
            ctx->unindent();
        }
        {
            auto value = getJson_bool(json, "fullscreen", false);
            changes |= ctx->bool_prop(&value, "Fullscreen");
            setJson_bool(json, "fullscreen", value);
        }
        {
            auto value = getJson_bool(json, "resizableWindow", false);
            changes |= ctx->bool_prop(&value, "Resizable window");
            setJson_bool(json, "resizableWindow", value);
        }
        {
            auto fixed_step = getJson_bool(json, "fixedStep", false);
            changes |= ctx->bool_prop(&fixed_step, "Fixed step");
            setJson_bool(json, "fixedStep", fixed_step);

            ctx->indent();
            ctx->pushEnableState(fixed_step);
            auto value = getJson_int(json, "updateFPS", 120);
            changes |= ctx->int_prop(&value, "Update FPS");
            setJson_int(json, "updateFPS", value);
            ctx->popEnableState();
            ctx->unindent();
        }
        {
            auto value = getJson_bool(json, "showFPS", false);
            changes |= ctx->bool_prop(&value, "Show FPS");
            setJson_bool(json, "showFPS", value);
        }
        {
            auto value = getJson_bool(json, "onScreenLog", false);
            changes |= ctx->bool_prop(&value, "On-screen log");
            setJson_bool(json, "onScreenLog", value);
        }
    }
    ctx->endVScrollArea(&scroll);

    if (changes)
    {
        auto json_before = original_json;
        auto json_after  = g_project->json;
        oActionManager->addAction("Project Settings Change", [=]
        {
            g_project->json = json_after;
        }, [=]
        {
            g_project->json = json_before;
        });
    }
}
