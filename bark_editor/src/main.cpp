#include <onut/ActionManager.h>
#include <onut/Input.h>
#include <onut/ContentManager.h>
#include <onut/Settings.h>
#include <onut/Renderer.h>
#include <onut/Log.h>
#include <imgui/imgui.h>
#include "globals.h"
#include "Theme.h"
#include "GUIContext.h"
#include "PanelsManager.h"
#include "SceneManager.h"
#include "Project.h"
#include "ComponentFactory.h"

static int invalidate_frames = 0;

void invalidate()
{
    invalidate_frames = 1;
}

void initSettings()
{
    oSettings->setGameName("Bark Editor");
    oSettings->setIsResizableWindow(true);
    oSettings->setResolution({1280, 720});
    oSettings->setIsEditorMode(true);
    oSettings->setStartMaximized(false);
    oSettings->setIsFixedStep(false);
}

void init()
{
    // Custom content manager for game assets
    g_content_mgr = OContentManager::create();
    g_content_mgr->clearSearchPaths();

    initComponentFactory();

    g_theme       = new Theme();
    g_gui_ctx     = new GUIContext();
    g_panels_mgr  = new PanelsManager();

    g_project     = new Project();
    g_project->openFolder("C:/github/onut/barkSamples/Dungeon"); // Hardcode it to this for now
}

void shutdown()
{
    delete g_project;
    delete g_panels_mgr;
    delete g_gui_ctx;
    delete g_theme;

    g_content_mgr = nullptr;

    shutdownComponentFactory();
}

void update()
{
    if (OInputPressed(OKeyLeftControl) && 
        OInputReleased(OKeyLeftShift) && 
        OInputReleased(OKeyLeftAlt) &&
        OInputJustPressed(OKeyZ))
    {
        oActionManager->undo();
    }
    if ((OInputPressed(OKeyLeftControl) && 
        OInputPressed(OKeyLeftShift) && 
        OInputReleased(OKeyLeftAlt) &&
        OInputJustPressed(OKeyZ)) ||
        OInputPressed(OKeyLeftControl) && 
        OInputReleased(OKeyLeftShift) && 
        OInputReleased(OKeyLeftAlt) &&
        OInputJustPressed(OKeyY))
    {
        oActionManager->redo();
    }

    g_gui_ctx->update();
}

void render()
{
    invalidate_frames++; // We draw at least 1 frame
    while (invalidate_frames)
    {
        invalidate_frames--;

        oRenderer->clear(g_theme->window_color);
        g_panels_mgr->render(g_gui_ctx);
        g_gui_ctx->reset();
    }
}

void postRender()
{
}

void renderUI()
{
    //if (ImGui::Begin("Theme"))
    //{
    //    ImGui::DragFloat("panel_margin", &g_theme->panel_margin);
    //    ImGui::DragFloat("panel_padding", &g_theme->panel_padding);
    //    ImGui::DragFloat("control_height", &g_theme->control_height);
    //    ImGui::DragFloat("list_item_height", &g_theme->list_item_height);
    //    ImGui::DragFloat("border_size", &g_theme->border_size);
    //    ImGui::DragFloat("tab_spacing", &g_theme->tab_spacing);
    //    ImGui::DragFloat("tab_padding", &g_theme->tab_padding);
    //    ImGui::DragFloat("tool_button_size", &g_theme->tool_button_size);
    //    ImGui::DragFloat("tool_bar_height", &g_theme->tool_bar_height);
    //    ImGui::DragFloat("tree_indent", &g_theme->tree_indent);
    //    ImGui::DragFloat("min_h_size", &g_theme->min_h_size);
    //    ImGui::DragFloat("min_v_size", &g_theme->min_v_size);
    //    ImGui::DragFloat("control_margin", &g_theme->control_margin);
    //    ImGui::DragFloat("control_padding", &g_theme->control_padding);
    //    ImGui::DragFloat("control_spacing", &g_theme->control_spacing);
    //    ImGui::DragFloat("numeric_control_width", &g_theme->numeric_control_width);
    //    ImGui::DragFloat("bool_control_width", &g_theme->bool_control_width);
    //    ImGui::DragFloat("text_control_width", &g_theme->text_control_width);

    //    ImGui::ColorEdit4("window_color", &g_theme->window_color.r);
    //    ImGui::ColorEdit4("dark_color", &g_theme->dark_color.r);
    //    ImGui::ColorEdit4("panel_color", &g_theme->panel_color.r);
    //    ImGui::ColorEdit4("panel_border_color", &g_theme->panel_border_color.r);
    //    ImGui::ColorEdit4("area_color", &g_theme->area_color.r);
    //    ImGui::ColorEdit4("area_border_color", &g_theme->area_border_color.r);
    //    ImGui::ColorEdit4("control_color", &g_theme->control_color.r);
    //    ImGui::ColorEdit4("control_border_color", &g_theme->control_border_color.r);
    //    ImGui::ColorEdit4("active_color", &g_theme->active_color.r);
    //    ImGui::ColorEdit4("disabled_control_color", &g_theme->disabled_control_color.r);
    //    ImGui::ColorEdit4("disabled_control_border_color", &g_theme->disabled_control_border_color.r);
    //    ImGui::ColorEdit4("separator_color", &g_theme->separator_color.r);
    //    ImGui::ColorEdit4("text_color", &g_theme->text_color.r);
    //    ImGui::ColorEdit4("text_over_color", &g_theme->text_over_color.r);
    //    ImGui::ColorEdit4("disabled_text_color", &g_theme->disabled_text_color.r);
    //    ImGui::ColorEdit4("tool_button_color", &g_theme->tool_button_color.r);
    //    ImGui::ColorEdit4("tool_button_hover", &g_theme->tool_button_hover.r);
    //    ImGui::ColorEdit4("tool_button_down", &g_theme->tool_button_down.r);
    //    ImGui::ColorEdit4("inactive_tab_color", &g_theme->inactive_tab_color.r);
    //    ImGui::ColorEdit4("dock_color", &g_theme->dock_color.r);
    //    ImGui::ColorEdit4("disabled_tint", &g_theme->disabled_tint.r);
    //    ImGui::ColorEdit4("header_color", &g_theme->header_color.r);
    //}
    //ImGui::End();
}
