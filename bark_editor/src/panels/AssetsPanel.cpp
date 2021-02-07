#include <onut/Renderer.h>
#include "AssetsPanel.h"
#include "GUIContext.h"
#include "Assets.h"
#include "globals.h"
#include "Theme.h"

AssetsPanel::AssetsPanel()
{
    name = "Assets";
}

void AssetsPanel::render(GUIContext* ctx)
{
    asset_to_open = false;
    ctx->drawArea();
    ctx->rect = ctx->rect.Grow(-ctx->theme->border_size);

    GUI_START_V_SCROLLABLE(ctx, scroll);
    {
        ctx->rect.w = ctx->theme->list_item_height - 1;
        renderSubDir(ctx, &g_assets->root, 0);
    }
    GUI_END_V_SCROLLABLE(ctx, scroll);

    if (asset_to_open)
    {
        g_assets->open(asset_to_open);
    }
}

void AssetsPanel::renderSubDir(GUIContext* ctx, AssetDir* parent_dir, int indent)
{
    // Directories first
    for (auto& dir : parent_dir->sub_dirs)
    {
        auto draw_save = ctx->draw_calls.size();

        if (ctx->drawListItem(dir.name, 
                              ctx->theme->asset_icons[dir.expanded ? (int)eAssetType::ExpandedFolder : (int)eAssetType::Folder],
                              indent, true, dir.selected) == eUIState::Clicked)
        {
            if (!ctx->ctrl && !ctx->shift) dir.expanded = !dir.expanded;
            g_assets->addSelection(ctx, &dir);
        }

        ctx->rect.y += ctx->theme->list_item_height;

        if (dir.expanded)
        {
            renderSubDir(ctx, &dir, indent + 1);
        }
    }

    // Assets
    for (auto& asset : parent_dir->assets)
    {
        auto draw_save = ctx->draw_calls.size();

        auto ui_state = ctx->drawListItem(asset.name, 
                                          ctx->theme->asset_icons[(int)asset.type],
                                          indent, true, asset.selected);
        if (ui_state == eUIState::Clicked)
        {
            g_assets->addSelection(ctx, &asset);
        }
        else if (ui_state == eUIState::DoubleClicked)
        {
            asset_to_open = &asset;
        }

        ctx->rect.y += ctx->theme->list_item_height;
    }
}
