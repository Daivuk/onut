#include <onut/ContentManager.h>
#include <onut/Settings.h>
#include <onut/Renderer.h>
#include "globals.h"
#include "Theme.h"
#include "GUIContext.h"
#include "PanelsManager.h"
#include "SceneManager.h"
#include "Project.h"
#include "ComponentFactory.h"

static int invalidate_frames = 0;

void invalidate(int frame_count)
{
    if (frame_count < 0) return;
    invalidate_frames = std::max(invalidate_frames, frame_count);
    if (invalidate_frames) oSettings->setIsEditorMode(false);
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
    g_gui_ctx->update();
}

void render()
{
    // Draw game view to a render target

    // Draw UIs
    oRenderer->clear(g_theme->window_color);
    g_panels_mgr->render(g_gui_ctx);

    if (invalidate_frames)
    {
        --invalidate_frames;
        if (invalidate_frames == -1)
        {
            oSettings->setIsEditorMode(true);
        }
    }
}

void postRender()
{
}

void renderUI()
{
}
