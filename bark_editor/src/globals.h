#pragma once

#include <onut/ForwardDeclaration.h>

OForwardDeclare(ContentManager);

class Theme;
extern Theme* g_theme;

class GUIContext;
extern GUIContext* g_gui_ctx;

class PanelsManager;
extern PanelsManager* g_panels_mgr;

extern OContentManagerRef g_content_mgr;
