#pragma once

#include <onut/Color.h>
#include <onut/ForwardDeclaration.h>
#include "Assets.h"

OForwardDeclare(Font);
OForwardDeclare(Texture);

class Theme final
{
public:
    OFontRef font;

    // Godot palette
    float panel_margin          = 8.0f;
    float panel_padding         = 5.0f;
    float control_height        = 26.0f;
    float header_height         = 20.0f;
    float list_item_height      = 20.0f;
    float border_size           = 1.0f;
    float tab_spacing           = 0.0f;
    float tab_padding           = 8.0f;
    float tool_button_size      = 16.0f;
    float tool_bar_height       = 20.0f;
    float tree_indent           = 16.0f;
    float min_h_size            = 200.0f;
    float min_v_size            = 100.0f;
    float control_margin        = 8.0f;
    float control_padding       = 4.0f;
    float control_spacing       = 2.0f;
    float numeric_control_width = 40.0f;
    float bool_control_width    = 40.0f;
    float text_control_width    = 150.0f;

    Color window_color                  = OColorHex(202531);
    Color dark_color                    = window_color * Color(0.5f, 0.5f, 0.5f, 1.0f);
    Color panel_color                   = OColorHex(333b4f);
    Color panel_border_color            = OColorHex(191d27);
    Color area_color                    = OColorHex(262c3b);
    Color area_border_color             = OColorHex(191d27);
    Color control_color                 = OColorHex(262c3b);
    Color control_border_color          = OColorHex(202531);
    Color active_color                  = OColorHex(699ce8);
    Color disabled_control_color        = OColorHex(2e3547);
    Color disabled_control_border_color = OColorHex(2e3547);
    Color separator_color               = OColorHex(333b4f);
    Color text_color                    = OColorHex(e0e0e0);
    Color text_over_color               = OColorHex(ffffff);
    Color disabled_text_color           = OColorHex(777a82);
    Color tool_button_color             = OColorHex(e0e0e0);
    Color tool_button_hover             = OColorHex(ffffff);
    Color tool_button_down              = OColorHex(699ce8);
    Color inactive_tab_color            = OColorHex(262c3b);
    Color dock_color                    = active_color * 0.5f;
    Color disabled_tint                 = Color(0.5f, 0.5f, 0.5f, 1.0f);
    Color header_color                  = OColorHex(404553);

    Color* tool_button_colors[3]        = {&tool_button_color, &tool_button_hover, &tool_button_down};
    Color* text_colors[3]               = {&text_color, &text_over_color, &active_color};
    Color* border_colors[3]             = {&control_border_color, &tool_button_hover, &tool_button_down};

    OTextureRef menu_icon;
    OTextureRef x_icon;
    OTextureRef asset_icons[(int)eAssetType::COUNT];

    OTextureRef scrollbar;

    Theme();
};
