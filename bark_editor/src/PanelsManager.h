#pragma once

#include <memory>
#include <vector>
#include <onut/ForwardDeclaration.h>
#include <onut/Vector2.h>

ForwardDeclare(DockNode);
ForwardDeclare(DockZone);
ForwardDeclare(Panel);
ForwardDeclare(AssetPanel);
ForwardDeclare(EntityPanel);
ForwardDeclare(ScenePanel);
ForwardDeclare(TimelinePanel);
ForwardDeclare(AssetsPanel);
ForwardDeclare(SceneViewPanel);

class GUIContext;
struct DockContext;

enum class eDockMagnet
{
    Left    = 0,
    Top     = 0,
    Middle  = 1,
    Right   = 2,
    Bottom  = 2
};

enum class eDockPosition
{
    Center  = 0,
    Top     = 1,
    Right   = 2,
    Bottom  = 3,
    Left    = 4,
    Tab     = 5
};

class DockNode
{
public:
    virtual void render(GUIContext* ctx) = 0;
    virtual void dock(GUIContext* ctx, DockContext* dock_ctx) = 0;
    virtual void undockPanel(const PanelRef& panel) = 0;
    virtual DockNodeRef clean() = 0;
    virtual DockNodeRef dockPanel(const PanelRef& panel, const DockContext& dock_ctx) = 0;
};

class DockNull final : public DockNode
{
public:
    void render(GUIContext* ctx) override {};
    void dock(GUIContext* ctx, DockContext* dock_ctx) override {};
    void undockPanel(const PanelRef& panel) override {};
    DockNodeRef clean() override { return nullptr; };
    DockNodeRef dockPanel(const PanelRef& panel, const DockContext& dock_ctx) override { return nullptr; };
};

// Dock zone is a leaf node. Containing one or many panels with tabs
class DockZone : public DockNode, public std::enable_shared_from_this<DockZone>
{
public:
    std::vector<PanelRef>   panels;
    int                     active_panel = 0;

    DockZone(const std::vector<PanelRef>& panels, int active_panel);

    void render(GUIContext* ctx) override;
    void dock(GUIContext* ctx, DockContext* dock_ctx) override;
    void undockPanel(const PanelRef& panel) override;
    DockNodeRef clean() override;
    DockNodeRef dockPanel(const PanelRef& panel, const DockContext& dock_ctx) override;
};

class DockKeepAround final : public DockZone
{
public:
    std::string text;

    DockKeepAround(const std::string& text, const std::vector<PanelRef>& panels, int active_panel);

    void render(GUIContext* ctx) override;
    DockNodeRef clean() override;
};

class DockHSplit final : public DockNode, public std::enable_shared_from_this<DockHSplit>
{
public:
    DockNodeRef left;
    DockNodeRef right;
    float       amount = 300.0f;
    eDockMagnet magnet = eDockMagnet::Left;

    DockHSplit(DockNodeRef left, DockNodeRef right, float amount, eDockMagnet magnet);

    void render(GUIContext* ctx) override;
    void dock(GUIContext* ctx, DockContext* dock_ctx) override;
    void undockPanel(const PanelRef& panel) override;
    DockNodeRef clean() override;
    DockNodeRef dockPanel(const PanelRef& panel, const DockContext& dock_ctx) override;
};

class DockVSplit final : public DockNode, public std::enable_shared_from_this<DockVSplit>
{
public:
    DockNodeRef top;
    DockNodeRef bottom;
    float       amount = 200.0f;
    eDockMagnet magnet = eDockMagnet::Bottom;

    DockVSplit(DockNodeRef top, DockNodeRef bottom, float amount, eDockMagnet magnet);

    void render(GUIContext* ctx) override;
    void dock(GUIContext* ctx, DockContext* result) override;
    void undockPanel(const PanelRef& panel) override;
    DockNodeRef clean() override;
    DockNodeRef dockPanel(const PanelRef& panel, const DockContext& dock_ctx) override;
};

struct DockContext
{
    DockZoneRef     target;
    eDockPosition   position    = eDockPosition::Center;
    int             tab_index   = 0;
    float           amount      = 0.5f;
    eDockMagnet     magnet      = eDockMagnet::Middle;
    bool            left_most   = true;
    bool            right_most  = true;
    bool            top_most    = true;
    bool            bottom_most = true;
};

class PanelsManager final
{
public:
    AssetPanelRef           project_panel;
    EntityPanelRef          properties_panel;
    ScenePanelRef           scene_panel;
    TimelinePanelRef        timeline_panel;
    AssetsPanelRef          assets_panel;
    DockNodeRef             dock_root;
    PanelRef                dragging_panel;
    DockNodeRef             dragging_split;
    DockZoneRef             document_zone;
    Rect                    dragging_split_rect;
    bool                    dropped_panel   = false;
    bool                    dropped_split   = false;
    bool                    closed_panel    = false;
    SceneViewPanelRef       focussed_scene_view;

    PanelsManager();
    ~PanelsManager();

    void undockPanel(const PanelRef& panel);
    void dockPanel(const PanelRef& panel, const DockContext& dock_ctx); 
    void cleanDock();

    void render(GUIContext* ctx);
};
