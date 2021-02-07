#include <onut/Font.h>

#include "PanelsManager.h"
#include "GUIContext.h"
#include "Theme.h"
#include "globals.h"

#include "AssetPanel.h"
#include "EntityPanel.h"
#include "ScenePanel.h"
#include "SceneViewPanel.h"
#include "TimelinePanel.h"
#include "AssetsPanel.h"

DockZone::DockZone(const std::vector<PanelRef>& in_panels, int in_active_panel)
    : panels(in_panels)
    , active_panel(in_active_panel)
{
}

DockHSplit::DockHSplit(DockNodeRef in_left, DockNodeRef in_right, float in_amount, eDockMagnet in_magnet)
    : left(in_left)
    , right(in_right)
    , amount(in_amount)
    , magnet(in_magnet)
{
}

DockVSplit::DockVSplit(DockNodeRef in_top, DockNodeRef in_bottom, float in_amount, eDockMagnet in_magnet)
    : top(in_top)
    , bottom(in_bottom)
    , amount(in_amount)
    , magnet(in_magnet)
{
}

DockKeepAround::DockKeepAround(const std::string& in_text, const std::vector<PanelRef>& panels, int active_panel)
    : DockZone(panels, active_panel)
    , text(in_text)
{
}

void DockKeepAround::render(GUIContext* ctx)
{
    if (panels.empty())
    {
        ctx->drawText(ctx->theme->font, text, ctx->rect.Center(), OCenter, ctx->theme->disabled_text_color);
    }
    else
    {
        DockZone::render(ctx);
    }
}

void DockZone::render(GUIContext* ctx)
{
    if (panels.empty()) return;

    // Draw active panel
    {
        ctx->pushRect();
        ctx->rect.y += ctx->theme->control_height;
        ctx->rect.w -= ctx->theme->control_height;
        ctx->drawPanel();
        ctx->rect = ctx->rect.Grow(-ctx->theme->panel_padding);
        panels[active_panel]->render(ctx);
        ctx->popRect();
    }

    // Draw tabs
    {
        ctx->pushRect();
        ctx->rect.w = ctx->theme->control_height;
        float tab_offset = 0.0f;
        for (int i = 0; i < (int)panels.size() && tab_offset < ctx->rect.z; ++i)
        {
            const auto& panel = panels[i];
            eUIState ui_state;
            if (i == active_panel)
            {
                ui_state = ctx->drawActiveTab(panels[i]->name, tab_offset, panel->closable);
                if (ui_state == eUIState::Down)
                {
                    auto scene_view = ODynamicCast<SceneViewPanel>(panel);
                    if (scene_view) g_panels_mgr->focussed_scene_view = scene_view;
                }
            }
            else
            {
                auto offset_before = tab_offset;
                ui_state = ctx->drawInactiveTab(panels[i]->name, tab_offset, panel->closable);
                if (ui_state == eUIState::Down)
                {
                    active_panel = i;
                    auto scene_view = ODynamicCast<SceneViewPanel>(panel);
                    if (scene_view) g_panels_mgr->focussed_scene_view = scene_view;
                }
            }
            if (ui_state == eUIState::Close)
            {
                panels.erase(panels.begin() + i);
                g_panels_mgr->closed_panel = true;
                active_panel = std::min(i, (int)panels.size() - 1);
                --i;

                if (g_panels_mgr->focussed_scene_view == panel) g_panels_mgr->focussed_scene_view = nullptr;
                if (!panels.empty())
                {
                    auto new_active_panel = panels[active_panel];
                    auto scene_view = ODynamicCast<SceneViewPanel>(new_active_panel);
                    if (scene_view) g_panels_mgr->focussed_scene_view = scene_view;
                }
            }
            else if (ui_state == eUIState::Drag || ui_state == eUIState::Drop)
            {
                g_panels_mgr->dragging_panel = panel;
                g_panels_mgr->dropped_panel = ui_state == eUIState::Drop;
            }
        }
        ctx->popRect();
    }

    // Zone options (none for now)
    //{
    //    ctx->drawToolButton(ctx->theme->menu_icon, Vector2(
    //        ctx->rect.x + ctx->rect.z - ctx->theme->tool_button_size, 
    //        ctx->rect.y + (ctx->theme->control_height - ctx->theme->tool_button_size) * 0.5f));
    //}
}

void DockZone::dock(GUIContext* ctx, DockContext* dock_ctx)
{
    const auto& rect = ctx->rect;

    Vector2 mouse_percent(
        (ctx->mouse.x - rect.x) / rect.z,
        (ctx->mouse.y - rect.y - ctx->theme->control_height) / (rect.w - ctx->theme->control_height));

    dock_ctx->target    = OThis;
    dock_ctx->position  = eDockPosition::Center;

    float tab_offset = 0.0f;
    float tab_width = 0.0f;

    if (mouse_percent.y < 0)
    {
        dock_ctx->position = eDockPosition::Tab;
        for (dock_ctx->tab_index = 0; dock_ctx->tab_index < (int)panels.size() && tab_offset < ctx->rect.z; ++dock_ctx->tab_index)
        {
            const auto& panel = panels[dock_ctx->tab_index];
            auto text_size = ctx->theme->font->measure(panel->name);
            tab_width = text_size.x + ctx->theme->tab_padding * 2.0f + (panel->closable ? (16.0f + ctx->theme->tab_padding) : 0.0f);

            if (ctx->mouse.x <= rect.x + tab_offset + tab_width)
            {
                break;
            }
            
            tab_offset += tab_width + ctx->theme->tab_spacing;
        }
    }
    else if (rect.w > rect.z)
    {
        if (mouse_percent.y < 0.25f) dock_ctx->position = eDockPosition::Top;
        else if (mouse_percent.y > 0.75f) dock_ctx->position = eDockPosition::Bottom;
        else if (mouse_percent.x < 0.25f) dock_ctx->position = eDockPosition::Left;
        else if (mouse_percent.x > 0.75f) dock_ctx->position = eDockPosition::Right;
    }
    else
    {
        if (mouse_percent.x < 0.25f) dock_ctx->position = eDockPosition::Left;
        else if (mouse_percent.x > 0.75f) dock_ctx->position = eDockPosition::Right;
        else if (mouse_percent.y < 0.25f) dock_ctx->position = eDockPosition::Top;
        else if (mouse_percent.y > 0.75f) dock_ctx->position = eDockPosition::Bottom;
    }

    switch (dock_ctx->position)
    {
        case eDockPosition::Center:
            ctx->drawRect(nullptr, rect, ctx->theme->dock_color);
            break;
        case eDockPosition::Left:
            if (dock_ctx->left_most)
            {
                dock_ctx->amount = std::min(rect.z * 0.5f, 300.0f);
                dock_ctx->magnet = eDockMagnet::Left;
            }
            else if (dock_ctx->right_most)
            {
                dock_ctx->amount = std::min(rect.z * 0.5f, 300.0f);
                dock_ctx->magnet = eDockMagnet::Right;
            }
            ctx->drawRect(nullptr, {rect.x, rect.y, rect.z * 0.5f, rect.w}, ctx->theme->dock_color);
            break;
        case eDockPosition::Right:
            if (dock_ctx->left_most)
            {
                dock_ctx->amount = std::min(rect.z * 0.5f, 300.0f);
                dock_ctx->magnet = eDockMagnet::Left;
            }
            else if (dock_ctx->right_most)
            {
                dock_ctx->amount = std::min(rect.z * 0.5f, 300.0f);
                dock_ctx->magnet = eDockMagnet::Right;
            }
            ctx->drawRect(nullptr, {rect.x + rect.z * 0.5f, rect.y, rect.z * 0.5f, rect.w}, ctx->theme->dock_color);
            break;
        case eDockPosition::Top:
            if (dock_ctx->bottom_most)
            {
                dock_ctx->amount = std::min(rect.w * 0.5f, 200.0f);
                dock_ctx->magnet = eDockMagnet::Bottom;
            }
            else if (dock_ctx->top_most)
            {
                dock_ctx->amount = std::min(rect.w * 0.5f, 200.0f);
                dock_ctx->magnet = eDockMagnet::Top;
            }
            ctx->drawRect(nullptr, {rect.x, rect.y, rect.z, rect.w * 0.5f}, ctx->theme->dock_color);
            break;
        case eDockPosition::Bottom:
            if (dock_ctx->bottom_most)
            {
                dock_ctx->amount = std::min(rect.w * 0.5f, 200.0f);
                dock_ctx->magnet = eDockMagnet::Bottom;
            }
            else if (dock_ctx->top_most)
            {
                dock_ctx->amount = std::min(rect.w * 0.5f, 200.0f);
                dock_ctx->magnet = eDockMagnet::Top;
            }
            ctx->drawRect(nullptr, {rect.x, rect.y + rect.w * 0.5f, rect.z, rect.w * 0.5f}, ctx->theme->dock_color);
            break;
        case eDockPosition::Tab:
            ctx->drawRect(nullptr, {rect.x + tab_offset - 4.0f, rect.y, 4.0f, ctx->theme->control_height}, ctx->theme->dock_color);
            break;
    }
}

void DockHSplit::dock(GUIContext* ctx, DockContext* dock_ctx)
{
    float splitPos = (float)(int)amount;
    if (magnet == eDockMagnet::Right) splitPos = (float)(int)(ctx->rect.z - amount);
    else if (magnet == eDockMagnet::Middle) splitPos = (float)(int)(ctx->rect.z * amount);

    if (left)
    {
        ctx->pushRect();
        ctx->rect.z = splitPos - ctx->theme->panel_margin * 0.5f;
        if (ctx->rect.Contains(ctx->mouse))
        {
            dock_ctx->right_most = false;
            left->dock(ctx, dock_ctx);
        }
        ctx->popRect();
    }

    if (right)
    {
        ctx->pushRect();
        ctx->rect.x += splitPos + ctx->theme->panel_margin * 0.5f;
        ctx->rect.z -= splitPos + ctx->theme->panel_margin * 0.5f;
        if (ctx->rect.Contains(ctx->mouse))
        {
            dock_ctx->left_most = false;
            right->dock(ctx, dock_ctx);
        }
        ctx->popRect();
    }
}

void DockVSplit::dock(GUIContext* ctx, DockContext* dock_ctx)
{
    // Load of repeated code...
    float splitPos = (float)(int)amount;
    if (magnet == eDockMagnet::Bottom) splitPos = (float)(int)(ctx->rect.w - amount);
    else if (magnet == eDockMagnet::Middle) splitPos = (float)(int)(ctx->rect.w * amount);

    if (top)
    {
        ctx->pushRect();
        ctx->rect.w = splitPos - ctx->theme->panel_margin * 0.5f;
        if (ctx->rect.Contains(ctx->mouse))
        {
            dock_ctx->bottom_most = false;
            top->dock(ctx, dock_ctx);
        }
        ctx->popRect();
    }

    if (bottom)
    {
        ctx->pushRect();
        ctx->rect.y += splitPos + ctx->theme->panel_margin * 0.5f;
        ctx->rect.w -= splitPos + ctx->theme->panel_margin * 0.5f;
        if (ctx->rect.Contains(ctx->mouse))
        {
            dock_ctx->top_most = false;
            bottom->dock(ctx, dock_ctx);
        }
        ctx->popRect();
    }
}

void DockHSplit::render(GUIContext* ctx)
{
    float splitPos = (float)(int)amount;
    if (magnet == eDockMagnet::Right) splitPos = (float)(int)(ctx->rect.z - amount);
    else if (magnet == eDockMagnet::Middle) splitPos = (float)(int)(ctx->rect.z * amount);

    ctx->pushRect();
    ctx->rect = Rect(ctx->rect.x + splitPos - ctx->theme->panel_margin * 0.5f, 
                     ctx->rect.y,
                     ctx->theme->panel_margin,
                     ctx->rect.w);
    auto ui_state = ctx->drawHSplitHandle();
    ctx->popRect();

    if (left)
    {
        ctx->pushRect();
        ctx->rect.z = splitPos - ctx->theme->panel_margin * 0.5f;
        left->render(ctx);
        ctx->popRect();
    }

    if (right)
    {
        ctx->pushRect();
        ctx->rect.x += splitPos + ctx->theme->panel_margin * 0.5f;
        ctx->rect.z -= splitPos + ctx->theme->panel_margin * 0.5f;
        right->render(ctx);
        ctx->popRect();
    }
    
    if (ui_state == eUIState::Drag || ui_state == eUIState::Drop)
    {
        g_panels_mgr->dragging_split = OThis;
        g_panels_mgr->dropped_split = ui_state == eUIState::Drop;
        g_panels_mgr->dragging_split_rect = ctx->rect;
    }
}

void DockVSplit::render(GUIContext* ctx)
{
    float splitPos = (float)(int)amount;
    if (magnet == eDockMagnet::Bottom) splitPos = (float)(int)(ctx->rect.w - amount);
    else if (magnet == eDockMagnet::Middle) splitPos = (float)(int)(ctx->rect.w * amount);
        
    ctx->pushRect();
    ctx->rect = Rect(ctx->rect.x, 
                     ctx->rect.y + splitPos - ctx->theme->panel_margin * 0.5f,
                     ctx->rect.z,
                     ctx->theme->panel_margin);
    auto ui_state = ctx->drawVSplitHandle();
    ctx->popRect();

    if (top)
    {
        ctx->pushRect();
        ctx->rect.w = splitPos - ctx->theme->panel_margin * 0.5f;
        top->render(ctx);
        ctx->popRect();
    }

    if (bottom)
    {
        ctx->pushRect();
        ctx->rect.y += splitPos + ctx->theme->panel_margin * 0.5f;
        ctx->rect.w -= splitPos + ctx->theme->panel_margin * 0.5f;
        bottom->render(ctx);
        ctx->popRect();
    }
    
    if (ui_state == eUIState::Drag || ui_state == eUIState::Drop)
    {
        g_panels_mgr->dragging_split = OThis;
        g_panels_mgr->dragging_split_rect = ctx->rect;
        g_panels_mgr->dropped_split = ui_state == eUIState::Drop;
    }
}

PanelsManager::PanelsManager()
{
    // Allocate all panels
    project_panel       = OMake<AssetPanel>();
    properties_panel    = OMake<EntityPanel>();
    scene_panel         = OMake<ScenePanel>();
    timeline_panel      = OMake<TimelinePanel>();
    assets_panel        = OMake<AssetsPanel>();
    document_zone       = OMake<DockKeepAround>("Documents View", std::vector<PanelRef>{}, 0);

    // Create default dock layout
    dock_root = OMake<DockHSplit>(
        OMake<DockHSplit>(
            OMake<DockVSplit>(
                OMake<DockZone>(std::vector<PanelRef>{scene_panel}, 0),
                OMake<DockZone>(std::vector<PanelRef>{assets_panel}, 0),
                0.6f, eDockMagnet::Middle
            ),
            OMake<DockVSplit>(
                document_zone,
                OMake<DockZone>(std::vector<PanelRef>{timeline_panel}, 0),
                200.0f, eDockMagnet::Bottom
            ),
            300.0f, eDockMagnet::Left
        ),
        OMake<DockZone>(std::vector<PanelRef>{properties_panel, project_panel}, 0),
        300.0f, eDockMagnet::Right
    );
}

PanelsManager::~PanelsManager()
{
}

void PanelsManager::undockPanel(const PanelRef& panel)
{
    dock_root->undockPanel(panel);
}

void DockZone::undockPanel(const PanelRef& panel)
{
    for (int i = 0, len = (int)panels.size(); i < len; ++i)
    {
        if (panels[i] == panel)
        {
            panels[i] = nullptr;
            return;
        }
    }
}

void DockHSplit::undockPanel(const PanelRef& panel)
{
    if (left) left->undockPanel(panel);
    if (right) right->undockPanel(panel);
}

void DockVSplit::undockPanel(const PanelRef& panel)
{
    if (top) top->undockPanel(panel);
    if (bottom) bottom->undockPanel(panel);
}

void PanelsManager::dockPanel(const PanelRef& panel, const DockContext& dock_ctx)
{
    auto new_root = dock_root->dockPanel(panel, dock_ctx);
    if (new_root != dock_root) dock_root = new_root;
}

DockNodeRef DockZone::dockPanel(const PanelRef& panel, const DockContext& dock_ctx)
{
    if (dock_ctx.target.get() == this)
    {
        switch (dock_ctx.position)
        {
            case eDockPosition::Center:
                panels.push_back(panel);
                active_panel = (int)panels.size() - 1;
                break;
            case eDockPosition::Tab:
                panels.insert(panels.begin() + dock_ctx.tab_index, panel);
                active_panel = dock_ctx.tab_index;
                break;
            case eDockPosition::Left:
                return OMake<DockHSplit>(OMake<DockZone>(std::vector<PanelRef>{panel}, 0), OThis, dock_ctx.amount, dock_ctx.magnet);
            case eDockPosition::Right:
                return OMake<DockHSplit>(OThis, OMake<DockZone>(std::vector<PanelRef>{panel}, 0), dock_ctx.amount, dock_ctx.magnet);
            case eDockPosition::Top:
                return OMake<DockVSplit>(OMake<DockZone>(std::vector<PanelRef>{panel}, 0), OThis, dock_ctx.amount, dock_ctx.magnet);
            case eDockPosition::Bottom:
                return OMake<DockVSplit>(OThis, OMake<DockZone>(std::vector<PanelRef>{panel}, 0), dock_ctx.amount, dock_ctx.magnet);
        }
    }

    return OThis;
}

DockNodeRef DockHSplit::dockPanel(const PanelRef& panel, const DockContext& dock_ctx)
{
    if (left)
    {
        auto new_node = left->dockPanel(panel, dock_ctx);
        if (new_node != left) left = new_node;
    }
    if (right)
    {
        auto new_node = right->dockPanel(panel, dock_ctx);
        if (new_node != right) right = new_node;
    }
    return OThis;
}

DockNodeRef DockVSplit::dockPanel(const PanelRef& panel, const DockContext& dock_ctx)
{
    if (top)
    {
        auto new_node = top->dockPanel(panel, dock_ctx);
        if (new_node != top) top = new_node;
    }
    if (bottom)
    {
        auto new_node = bottom->dockPanel(panel, dock_ctx);
        if (new_node != bottom) bottom = new_node;
    }
    return OThis;
}

void PanelsManager::cleanDock()
{
    auto new_root = dock_root->clean();
    if (new_root) dock_root = new_root;
    if (std::dynamic_pointer_cast<DockNull>(dock_root))
    {
        dock_root = OMake<DockZone>(std::vector<PanelRef>{}, 0); // Empty screen
    }
}

DockNodeRef DockKeepAround::clean()
{
    DockZone::clean();
    return nullptr;
}

DockNodeRef DockZone::clean()
{
    for (auto it = panels.begin(); it != panels.end();)
    {
        if (*it == nullptr)
        {
            it = panels.erase(it);
            continue;
        }
        ++it;
    }
    if (panels.empty()) return OMake<DockNull>();

    active_panel = std::min(active_panel, (int)panels.size() - 1);
    return nullptr;
}

DockNodeRef DockHSplit::clean()
{
    if (left)
    {
        auto new_node = left->clean();
        if (new_node) left = new_node;
        if (std::dynamic_pointer_cast<DockNull>(left)) left = nullptr;
    }

    if (right)
    {
        auto new_node = right->clean();
        if (new_node) right = new_node;
        if (std::dynamic_pointer_cast<DockNull>(right)) right = nullptr;
    }

    if (!left && !right) return OMake<DockNull>();
    if (!left) return right;
    if (!right) return left;

    return nullptr;
}

DockNodeRef DockVSplit::clean()
{
    if (top)
    {
        auto new_node = top->clean();
        if (new_node) top = new_node;
        if (std::dynamic_pointer_cast<DockNull>(top)) top = nullptr;
    }

    if (bottom)
    {
        auto new_node = bottom->clean();
        if (new_node) bottom = new_node;
        if (std::dynamic_pointer_cast<DockNull>(bottom)) bottom = nullptr;
    }

    if (!top && !bottom) return OMake<DockNull>();
    if (!top) return bottom;
    if (!bottom) return top;

    return nullptr;
}

void PanelsManager::render(GUIContext* ctx)
{
    // Draw UIs
    dragging_panel  = nullptr;
    dragging_split  = nullptr;
    dropped_panel   = false;
    closed_panel    = false;

    ctx->begin();
    dock_root->render(ctx);

    // Draw/Do docking
    if (dragging_panel)
    {
        DockContext dock_context;
        dock_context.target         = nullptr;
        dock_context.left_most      = true;
        dock_context.right_most     = true;
        dock_context.top_most       = true;
        dock_context.bottom_most    = true;

        dock_root->dock(ctx, &dock_context);

        if (dock_context.target && dropped_panel)
        {
            undockPanel(dragging_panel);
            dockPanel(dragging_panel, dock_context);
            cleanDock();
        }
    }

    // Draw/Do split dragging
    if (dragging_split)
    {
        auto hsplit = ODynamicCast<DockHSplit>(dragging_split);
        auto vsplit = ODynamicCast<DockVSplit>(dragging_split);

        if (hsplit)
        {
            float splitPos = (float)(int)hsplit->amount;
            if (hsplit->magnet == eDockMagnet::Right) splitPos = (float)(int)(ctx->rect.z - hsplit->amount);
            else if (hsplit->magnet == eDockMagnet::Middle) splitPos = (float)(int)(ctx->rect.z * hsplit->amount);

            splitPos = (ctx->mouse.x - dragging_split_rect.x);
            splitPos = std::max(splitPos, ctx->theme->min_h_size);
            splitPos = std::min(splitPos, dragging_split_rect.z - ctx->theme->min_h_size);

            ctx->drawRect(nullptr, Rect(
                dragging_split_rect.x + splitPos - ctx->theme->panel_padding * 0.5f,
                dragging_split_rect.y,
                ctx->theme->panel_padding,
                dragging_split_rect.w),
                ctx->theme->dock_color);

            if (dropped_split)
            {
                if (hsplit->magnet == eDockMagnet::Left) hsplit->amount = splitPos;
                else if (hsplit->magnet == eDockMagnet::Right) hsplit->amount = dragging_split_rect.z - splitPos;
                else if (hsplit->magnet == eDockMagnet::Middle) hsplit->amount = splitPos / dragging_split_rect.z;
            }
        }
        else if (vsplit) // Implied
        {
            float splitPos = (float)(int)vsplit->amount;
            if (vsplit->magnet == eDockMagnet::Bottom) splitPos = (float)(int)(ctx->rect.w - vsplit->amount);
            else if (vsplit->magnet == eDockMagnet::Middle) splitPos = (float)(int)(ctx->rect.w * vsplit->amount);

            splitPos = (ctx->mouse.y - dragging_split_rect.y);
            splitPos = std::max(splitPos, ctx->theme->min_v_size);
            splitPos = std::min(splitPos, dragging_split_rect.w - ctx->theme->min_v_size);

            ctx->drawRect(nullptr, Rect(
                dragging_split_rect.x,
                dragging_split_rect.y + splitPos - ctx->theme->panel_padding * 0.5f,
                dragging_split_rect.z,
                ctx->theme->panel_padding),
                ctx->theme->dock_color);

            if (dropped_split)
            {
                if (vsplit->magnet == eDockMagnet::Top) vsplit->amount = splitPos;
                else if (vsplit->magnet == eDockMagnet::Bottom) vsplit->amount = dragging_split_rect.w - splitPos;
                else if (vsplit->magnet == eDockMagnet::Middle) vsplit->amount = splitPos / dragging_split_rect.w;
            }
        }
    }

    if (closed_panel)
    {
        cleanDock();
    }

    // Submit draw
    ctx->end();
}
