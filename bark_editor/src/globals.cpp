#include <onut/ContentManager.h>
#include "GUIContext.h"
#include "Theme.h"
#include "PanelsManager.h"
#include "Project.h"
#include "Assets.h"

Theme*              g_theme       = nullptr;
GUIContext*         g_gui_ctx     = nullptr;
PanelsManager*      g_panels_mgr  = nullptr;
OContentManagerRef  g_content_mgr;
Project*            g_project     = nullptr;
Assets*             g_assets      = nullptr;
