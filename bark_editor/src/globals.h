#pragma once

#include <vector>
#include <onut/ForwardDeclaration.h>

OForwardDeclare(ContentManager);
ForwardDeclare(Entity);

void invalidate();

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

void focusOn(const EntityRef& entity);
void focusOn(const std::vector<EntityRef>& entities);
void focusOnSelectedEntities();
