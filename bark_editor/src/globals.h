#pragma once

#include <onut/ForwardDeclaration.h>

OForwardDeclare(ContentManager);

void invalidate(int frame_count = 1);

class Theme;
class GUIContext;
class PanelsManager;
class Project;
class Assets;

extern Theme*               g_theme;
extern GUIContext*          g_gui_ctx;
extern PanelsManager*       g_panels_mgr;
extern OContentManagerRef   g_content_mgr;
extern Project*             g_project;
extern Assets*              g_assets;
