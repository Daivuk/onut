#include <onut/ContentManager.h>
#include "GUIContext.h"
#include "Theme.h"
#include "PanelsManager.h"
#include "Project.h"
#include "Assets.h"
#include "Entity.h"
#include "PanelsManager.h"
#include "SceneViewPanel.h"

Theme*              g_theme       = nullptr;
GUIContext*         g_gui_ctx     = nullptr;
PanelsManager*      g_panels_mgr  = nullptr;
OContentManagerRef  g_content_mgr;
Project*            g_project     = nullptr;
Assets*             g_assets      = nullptr;

void focusOn(const EntityRef& entity)
{
    if (g_panels_mgr->focussed_scene_view)
    {
        g_panels_mgr->focussed_scene_view->focusOn({ entity });
    }
}

void focusOn(const std::vector<EntityRef>& entities)
{
    if (g_panels_mgr->focussed_scene_view)
    {
        g_panels_mgr->focussed_scene_view->focusOn(entities);
    }
}

void focusOnSelectedEntities()
{
    if (g_panels_mgr->focussed_scene_view)
    {
        g_panels_mgr->focussed_scene_view->focusOn(g_panels_mgr->focussed_scene_view->selected_entities);
    }
}
