#include <onut/ContentManager.h>
#include <onut/Settings.h>
#include <onut/Renderer.h>
#include "globals.h"
#include "Theme.h"
#include "GUIContext.h"
#include "PanelsManager.h"

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
    g_theme       = new Theme();
    g_gui_ctx     = new GUIContext();
    g_panels_mgr  = new PanelsManager();

    // Custom content manager for game assets
    g_content_mgr = OContentManager::create();
    g_content_mgr->clearSearchPaths();
    g_content_mgr->addSearchPath("C:/github/onut/barkSamples/Dungeon/assets"); // Hardcoded for now
}

void shutdown()
{
    g_content_mgr = nullptr;
    delete g_panels_mgr;
    delete g_gui_ctx;
    delete g_theme;
}

void update()
{
    g_gui_ctx->update();
}

void render()
{
    // Draw game view to a render target

    // Draw UIs
    oRenderer->clear(g_theme->window_color);
    g_panels_mgr->render(g_gui_ctx);
}

void postRender()
{
}

void renderUI()
{
}
