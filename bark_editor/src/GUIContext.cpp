#include <sstream>
#include <iomanip>
#include <onut/ContentManager.h>
#include <onut/Input.h>
#include <onut/Log.h>
#include <onut/Font.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include <onut/Window.h>
#include <onut/Strings.h>
#include <onut/Texture.h>
#include <onut/TiledMap.h>
#include <onut/Sound.h>
#include <onut/SpriteAnim.h>
#include <onut/Model.h>
#include <onut/Shader.h>
#include "Script.h"
#include "GUIContext.h"
#include "globals.h"
#include "Theme.h"

static const float DRAG_DISTANCE            = 3.0f;
static const float INT_DRAG_SENSITIVITY     = 0.25f;
static const float FLOAT_DRAG_SENSITIVITY   = 0.5f;

GUIContext::GUIContext()
{
    oWindow->onWriteFunc = [this](onut::WriteFunc write_func)
    {
        if (edit_state != State::TextEditing) return;

        switch (write_func)
        {
            case onut::WriteFunc::Backspace:
                if (text_editing.selected_from == text_editing.selected_to) // Erase single character
                {
                    if (text_editing.selected_from > 0) // Only if we're not at the beggining
                    {
                        text_editing.text.erase(onut::utf8Pos(text_editing.text, text_editing.selected_from - 1), 1);
                        text_editing.selected_from--;
                        text_editing.selected_to = text_editing.selected_from;
                    }
                }
                else if (!text_editing.text.empty()) // Erase selected section
                {
                    text_editing.text = 
                        text_editing.text.substr(0, onut::utf8Pos(text_editing.text, text_editing.selected_from)) + 
                        text_editing.text.substr(onut::utf8Pos(text_editing.text, text_editing.selected_to), std::string::npos);
                    text_editing.selected_to = text_editing.selected_from;
                }
                break;

            case onut::WriteFunc::CariageReturn:
                text_editing.commited = true;
                break;

            case onut::WriteFunc::Escape:
                edit_state = State::None;
                break;

            default:
                break;
        }
    };

    oWindow->onWriteUTF8 = [this](const std::string& text)
    {
        if (edit_state != State::TextEditing) return;

        // Text insertion
        text_editing.text = 
            text_editing.text.substr(0, onut::utf8Pos(text_editing.text, text_editing.selected_from)) + 
            text +
            text_editing.text.substr(onut::utf8Pos(text_editing.text, text_editing.selected_to), std::string::npos);
        text_editing.selected_from++;
        text_editing.selected_to = text_editing.selected_from;
    };
}

GUIContext::~GUIContext()
{
}

void GUIContext::update()
{
    mouse                       = oInput->mousePosf;
    id                          = 0;

    left_button.down            = OInputPressed(OMouse1);
    left_button.just_down       = OInputJustPressed(OMouse1);
    left_button.clicked         = OInputJustReleased(OMouse1);
    left_button.double_clicked  = oInput->getDoubleClicked();

    if (left_button.double_clicked) left_button.down = false;

    keys.ctrl                   = OInputPressed(OKeyLeftControl);
    keys.shift                  = OInputPressed(OKeyLeftShift);
    keys.alt                    = OInputPressed(OKeyLeftAlt);
    keys.esc                    = OInputJustPressed(OKeyEscape);
    keys.enter                  = OInputJustPressed(OKeyEnter) || OInputJustPressed(OKeyNumPadEnter);
    keys.del                    = OInputJustPressed(OKeyDelete);
    keys.left                   = OInputJustPressed(OKeyLeft);
    keys.right                  = OInputJustPressed(OKeyRight);
    keys.home                   = OInputJustPressed(OKeyHome);
    keys.end                    = OInputJustPressed(OKeyEnd);

    scrolling.amount            = oInput->getStateValue(OMouseZ) * 0.5f;

    if (left_button.just_down) down_id = 0;

    if (OInputJustPressed(OMouse1)) down_pos = mouse;

    if (left_button.down || 
        left_button.clicked || 
        left_button.double_clicked || 
        scrolling.amount || 
        keys.esc ||
        keys.enter || 
        keys.del || 
        keys.left || 
        keys.right) invalidate(); // Make sure we'll render the frame after
}

void GUIContext::reset()
{
    left_button.clicked         = false;
    left_button.double_clicked  = false;
    left_button.just_down       = false;
    keys.esc                    = false;
    keys.enter                  = false;
    keys.del                    = false;
    keys.left                   = false;
    keys.right                  = false;
    scrolling.amount            = 0.0f;
    id                          = 0;
}

void GUIContext::begin()
{
    theme               = g_theme;
    sb                  = oSpriteBatch.get();
    rect                = ORectFullScreen.Grow(-theme->panel_margin);
    r                   = oRenderer.get();
    rs                  = &r->renderStates;
    drag.type           = eUIDrag::None;
    saved_draw_point    = 0;
    cursor_type         = eUICursorType::Arrow;

    draw_calls.clear();
    rect_stack.clear();
    scissor_stack.clear();
    enable_stack.clear();
}

void GUIContext::end()
{
    sb->begin();

    drawing_enable_state = true;
    for (const auto& draw_call : draw_calls)
    {
        drawDrawCall(draw_call);
    }

    if (drag.type != eUIDrag::None)
    {
        drawing_enable_state = true;
        auto diff = mouse - down_pos;
        if (drag.type == eUIDrag::HSplit) diff.y = 0.0f;
        if (drag.type == eUIDrag::VSplit) diff.x = 0.0f;
        for (auto& draw_call : drag.draw_calls)
        {
            draw_call.color *= 0.65f;
            draw_call.rect.x += diff.x;
            draw_call.rect.y += diff.y;
            draw_call.pos += diff;
            drawDrawCall(draw_call);
        }
    }

    sb->end();

    switch (cursor_type)
    {
        case eUICursorType::Arrow:      oInput->unsetMouseIcon();                           break;
        case eUICursorType::SizeEW:     oInput->setMouseIcon("cur_size_ew.png", {9, 9});    break;
        case eUICursorType::SizeNS:     oInput->setMouseIcon("cur_size_ns.png", {9, 9});    break;
        case eUICursorType::SizeNESW:   oInput->setMouseIcon("cur_size_ne_sw.png", {9, 9}); break;
        case eUICursorType::SizeNWSE:   oInput->setMouseIcon("cur_size_nw_se.png", {9, 9}); break;
        case eUICursorType::SizeAll:    oInput->setMouseIcon("cur_size_all.png", {9, 9});   break;
        case eUICursorType::RotN:       oInput->setMouseIcon("cur_rot_n.png", {9, 9});      break;
        case eUICursorType::RotNE:      oInput->setMouseIcon("cur_rot_ne.png", {9, 9});     break;
        case eUICursorType::RotE:       oInput->setMouseIcon("cur_rot_e.png", {9, 9});      break;
        case eUICursorType::RotSE:      oInput->setMouseIcon("cur_rot_se.png", {9, 9});     break;
        case eUICursorType::RotS:       oInput->setMouseIcon("cur_rot_s.png", {9, 9});      break;
        case eUICursorType::RotSW:      oInput->setMouseIcon("cur_rot_sw.png", {9, 9});     break;
        case eUICursorType::RotW:       oInput->setMouseIcon("cur_rot_w.png", {9, 9});      break;
        case eUICursorType::RotNW:      oInput->setMouseIcon("cur_rot_nw.png", {9, 9});     break;
    }
}

void GUIContext::drawDrawCall(const UIDrawCall& draw_call)
{
    Color color = draw_call.color;
    if (!drawing_enable_state) color = color.AdjustedSaturation(0.0f) * 0.5f;

    switch (draw_call.type)
    {
        case eUIDrawCall::Outline:
            sb->drawInnerOutlineRect(draw_call.rect, draw_call.thickness, color);
            break;
        case eUIDrawCall::Rect:
            sb->drawRect(draw_call.texture, draw_call.rect, color);
            break;
        case eUIDrawCall::Text:
            sb->drawText(draw_call.font, draw_call.text, draw_call.pos, draw_call.origin, color);
            break;
        case eUIDrawCall::Sprite:
            sb->drawSprite(draw_call.texture, draw_call.pos, color, draw_call.origin);
            break;
        case eUIDrawCall::Slice9:
            sb->drawRectScaled9(draw_call.texture, draw_call.rect, draw_call.padding, color);
            break;
        case eUIDrawCall::SetScissor:
            sb->flush();
            rs->scissorEnabled = true;
            rs->scissor = iRect{
                (int)draw_call.rect.x, 
                (int)draw_call.rect.y, 
                (int)(draw_call.rect.x + draw_call.rect.z),
                (int)(draw_call.rect.y + draw_call.rect.w)};
            break;
        case eUIDrawCall::UnsetScissor:
            sb->flush();
            rs->scissorEnabled = false;
            break;
        case eUIDrawCall::SetEnableStyle:
            drawing_enable_state = true;
            break;
        case eUIDrawCall::SetDisableStyle:
            drawing_enable_state = false;
            break;
    }
}

void GUIContext::pushRect()
{
    rect_stack.push_back(rect);
}

Rect GUIContext::popRect()
{
    if (rect_stack.empty()) return rect;

    rect = rect_stack.back();
    rect_stack.pop_back();

    return rect;
}

void GUIContext::pushScissor()
{
    scissor_stack.push_back(rect);

    auto draw_call = getNextDrawCall();
    draw_call->type = eUIDrawCall::SetScissor;
    draw_call->rect = rect;
}

Rect GUIContext::popScissor()
{
    if (scissor_stack.empty()) return rect;

    auto ret = scissor_stack.back();
    scissor_stack.pop_back();

    auto draw_call = getNextDrawCall();
    if (scissor_stack.empty())
    {
        draw_call->type = eUIDrawCall::UnsetScissor;
    }
    else
    {
        draw_call->type = eUIDrawCall::SetScissor;
        draw_call->rect = scissor_stack.back();
    }

    return ret;
}

void GUIContext::saveDrawPoint()
{
    saved_draw_point = draw_calls.size();
}

void GUIContext::rewindDrawCalls()
{
    draw_calls.resize(saved_draw_point);
}

UIDrawCall* GUIContext::getNextDrawCall()
{
    draw_calls.push_back({});
    return &draw_calls.back();
}

void GUIContext::drawOutline(const Rect& rect, float thickness, const Color& color)
{
    auto draw_call = getNextDrawCall();
    draw_call->type = eUIDrawCall::Outline;
    draw_call->thickness = thickness;
    draw_call->color = color;
    draw_call->rect = rect;
}

void GUIContext::drawRect(const OTextureRef& texture, const Rect& rect, const Color& color)
{
    auto draw_call = getNextDrawCall();
    draw_call->texture = texture;
    draw_call->type = eUIDrawCall::Rect;
    draw_call->rect = rect;
    draw_call->color = color;
}

void GUIContext::drawText(const OFontRef& font, const std::string& text, const Vector2& pos, const Vector2& origin, const Color& color)
{
    auto draw_call = getNextDrawCall();
    draw_call->type = eUIDrawCall::Text;
    draw_call->font = font;
    draw_call->text = text;
    draw_call->pos = pos;
    draw_call->origin = origin;
    draw_call->color = color;
}

void GUIContext::drawSprite(const OTextureRef& texture, const Vector2& pos, const Color& color, const Vector2& origin)
{
    auto draw_call = getNextDrawCall();
    draw_call->type = eUIDrawCall::Sprite;
    draw_call->texture = texture;
    draw_call->pos = pos;
    draw_call->color = color;
    draw_call->origin = origin;
}

void GUIContext::drawSlice9(const OTextureRef& texture, const Rect& rect, const Vector4& padding, const Color& color)
{
    auto draw_call = getNextDrawCall();
    draw_call->type = eUIDrawCall::Slice9;
    draw_call->texture = texture;
    draw_call->rect = rect;
    draw_call->padding = padding;
    draw_call->color = color;
}

void GUIContext::drawPanel()
{
    drawOutline(rect, theme->border_size, theme->panel_border_color);
    drawRect(nullptr, rect.Grow(-theme->border_size), theme->panel_color);
}

eUIState GUIContext::drawHSplitHandle()
{
    auto state = getState(rect);
    if (state != eUIState::None)
    {
        drawSlice9(theme->scrollbar, Rect(rect.x, rect.y + rect.w * 0.4f, rect.z, rect.w * 0.2f), Vector4(4), theme->disabled_text_color);
        cursor_type = eUICursorType::SizeEW;
    }

    if (state == eUIState::Drag)
    {
        drag.type = eUIDrag::HSplit;
        drag.draw_calls.clear();
    }

    return state;
}

eUIState GUIContext::drawVSplitHandle()
{
    auto state = getState(rect);
    if (state != eUIState::None)
    {
        drawSlice9(theme->scrollbar, Rect(rect.x + rect.z * 0.4f, rect.y, rect.z * 0.2f, rect.w), Vector4(4), theme->disabled_text_color);
        cursor_type = eUICursorType::SizeNS;
    }

    if (state == eUIState::Drag)
    {
        drag.type = eUIDrag::VSplit;
        drag.draw_calls.clear();
    }

    return state;
}

eUIState GUIContext::drawTab(const std::string& name, float& offset, 
                             const Color& color, const Color& border_color,
                             const Color& text_colors, bool close_btn)
{
    auto tab_draw_point = draw_calls.size();

    auto text_size = theme->font->measure(name);
    auto width = text_size.x + theme->tab_padding * 2.0f;
    auto full_width = width + (close_btn ? (16.0f + theme->tab_padding) : 0.0f);

    auto tab_rect = rect;
    tab_rect.x += offset;
    tab_rect.z = full_width;
    tab_rect.w++;
    drawRect(nullptr, tab_rect, border_color);

    tab_rect = tab_rect.Grow(-theme->border_size);
    tab_rect.w += theme->border_size;
    drawRect(nullptr, tab_rect, color);

    auto state = getState(tab_rect);
    auto text_rect = tab_rect;
    text_rect.z = width - theme->border_size * 2.0f;
    drawText(theme->font, name, text_rect.Center(), OCenter, text_colors);

    offset += full_width + theme->tab_spacing;

    if (close_btn)
    {
        if (drawToolButton(theme->x_icon, tab_rect.Right() - Vector2(16.0f + theme->tab_padding, 7.0f)))
        {
            left_button.clicked = false; // Necessary?
            return eUIState::Close;
        }
    }

    if (state == eUIState::Drag)
    {
        drag.type = eUIDrag::Tab;
        drag.draw_calls.assign(draw_calls.begin() + tab_draw_point, draw_calls.end());
    }

    return state;
}

eUIState GUIContext::drawActiveTab(const std::string& name, float& offset, bool close_btn)
{
    return drawTab(name, offset, theme->panel_color, theme->panel_border_color, theme->text_color, close_btn);
}

eUIState GUIContext::drawInactiveTab(const std::string& name, float& offset, bool close_btn)
{
    return drawTab(name, offset, theme->inactive_tab_color, theme->window_color, theme->disabled_text_color, close_btn);
}

void GUIContext::drawArea()
{
    drawOutline(rect, theme->border_size, theme->area_border_color);
    drawRect(nullptr, rect.Grow(-theme->border_size), theme->area_color);
}

eUIState GUIContext::getState(const Rect& in_rect)
{
    ++id;

    if (!isEnabled()) return eUIState::None;

    eUIState ret = eUIState::None;
    auto rect = in_rect;

    if (!scissor_stack.empty())
    {
        const auto& scissor_rect = scissor_stack.back();
        if (!scissor_rect.Overlaps(rect)) return ret; // No need to check further
        rect = scissor_rect.Difference(rect);
    }

    bool is_mouse_hover = rect.Contains(mouse);

    if (left_button.just_down && is_mouse_hover)
    {
        down_id = id;
        if (edit_state == State::TextEditing && id != editing_control) text_editing.commited = true;
    }

    if (left_button.double_clicked && id == down_id)
    {
        if (is_mouse_hover)
        {
            ret = eUIState::DoubleClicked;
        }
    }
    else if (left_button.clicked && id == down_id)
    {
        if (is_mouse_hover)
        {
            ret = eUIState::Clicked;
        }
        else
        {
            ret = eUIState::Drop;
        }
    }
    else if (left_button.down)
    {
        if (id == down_id)
        {
            if (is_mouse_hover)
            {
                ret = eUIState::Down;
            }
            else if (Vector2::Distance(down_pos, mouse) > DRAG_DISTANCE)
            {
                ret = eUIState::Drag;
            }
            else
            {
                ret = eUIState::Hover;
            }
        }
    }
    else if (is_mouse_hover)
    {
        ret = eUIState::Hover;
    }

    return ret;
}

const Color& GUIContext::colorForState(eUIState state, Color** colors)
{
    switch (state)
    {
        case eUIState::Hover: return *colors[1]; 
        case eUIState::Down:  return *colors[2]; 
        default:              return *colors[0];
    }
}

bool GUIContext::drawToolButton(const OTextureRef& icon, const Vector2& pos)
{
    Rect btn_rect(pos, theme->tool_button_size, theme->tool_button_size);
    auto state = getState(btn_rect);

    drawSprite(icon, pos, colorForState(state, theme->tool_button_colors), OTopLeft);

    return state == eUIState::Clicked;
}

eUIState GUIContext::drawListItem(const std::string& text, const OTextureRef& icon, int indent, bool enabled, bool selected)
{
    auto state = getState(rect);

    Vector2 left(rect.x + theme->tree_indent * (float)indent, rect.y + rect.w * 0.5f);

    if (selected)
    {
        drawRect(nullptr, rect, theme->dock_color);
    }
    else if (state == eUIState::Hover || state == eUIState::Down)
    {
        drawRect(nullptr, rect, theme->disabled_control_border_color);
    }

    if (icon)
    {
        drawSprite(icon, left, enabled ? Color::White : theme->disabled_tint, OLeft);
        drawText(theme->font, text, left + Vector2(18.0f, -1.0f), OLeft, enabled ? theme->text_color : theme->disabled_text_color);
    }
    else
    {
        drawText(theme->font, text, left + Vector2(0.0f, -1.0f), OLeft, enabled ? theme->text_color : theme->disabled_text_color);
    }

    return state;
}

void GUIContext::beginVScrollArea(float scroll_amount)
{
    scrolling.v_view_size = rect.w;
    pushRect();
    pushScissor();
    rect = rect.Grow(-theme->panel_padding);
    rect.y -= scroll_amount;
    scrolling.v_content_start = rect.y;
}

void GUIContext::endVScrollArea(float* scroll_amount)
{
    scrolling.v_content_size = (rect.y + rect.w) - scrolling.v_content_start;
    popScissor();
    popRect();
    vScroll(scroll_amount);
}

bool GUIContext::vScroll(float* amount)
{
    if (!rect.Contains(mouse)) return false;

    auto prev_value = *amount;
    auto new_value  = prev_value;
    auto max_scroll = std::max(0.0f, scrolling.v_content_size - scrolling.v_view_size);

    new_value -= scrolling.amount;
    scrolling.amount = 0.0f;

    new_value = std::min(max_scroll, new_value);
    new_value = std::max(0.0f, new_value);

    *amount = new_value;
    return prev_value != new_value;
}

bool GUIContext::isEnabled()
{
    if (enable_stack.empty()) return true;
    return enable_stack.back();
}

void GUIContext::pushEnableState(bool enabled)
{
    auto was_enabled = isEnabled();

    enable_stack.push_back(enabled);

    if (was_enabled && !enabled)
    {
        UIDrawCall draw_call;
        draw_call.type = eUIDrawCall::SetDisableStyle;
        draw_calls.push_back(draw_call);
    }
    else if (!was_enabled && enabled)
    {
        UIDrawCall draw_call;
        draw_call.type = eUIDrawCall::SetEnableStyle;
        draw_calls.push_back(draw_call);
    }
}

void GUIContext::popEnableState()
{
    auto was_enabled = isEnabled();
    if (!enable_stack.empty()) enable_stack.pop_back();
    auto enabled = isEnabled();

    if (was_enabled && !enabled)
    {
        UIDrawCall draw_call;
        draw_call.type = eUIDrawCall::SetDisableStyle;
        draw_calls.push_back(draw_call);
    }
    else if (!was_enabled && enabled)
    {
        UIDrawCall draw_call;
        draw_call.type = eUIDrawCall::SetEnableStyle;
        draw_calls.push_back(draw_call);
    }
}

static int getNextWordPos(const std::string& str, int cur_pos)
{
    auto len = (int)onut::utf8Length(str);
    cur_pos = std::min(cur_pos + 1, len);
    if (cur_pos == len) return cur_pos;

    //TODO
    return cur_pos;
}

static int getPrevWordPos(const std::string& str, int cur_pos)
{
    auto len = (int)onut::utf8Length(str);
    cur_pos = std::min(cur_pos + 1, len);
    if (cur_pos == len) return cur_pos;
    
    //TODO
    return cur_pos;
}

bool GUIContext::drawEditingText(std::string* value, eUIState state)
{
    bool ret = false;

    if (editing_control == id && edit_state == State::TextEditing)
    {
        if (state == eUIState::None && left_button.just_down)
        {
            text_editing.commited = true;
        }

        if (text_editing.commited)
        {
            edit_state = State::None;
            ret = *value != text_editing.text;
            *value = text_editing.text;
        }
        else if (keys.left)
        {
            if (keys.shift) // Range select
            {
                if (text_editing.select_range_at_start)
                {
                    if (text_editing.selected_from > 0)
                    {
                        if (keys.ctrl) // Jump words
                        {
                        }
                        else text_editing.selected_from--;
                    }
                }
                else
                {
                    if (text_editing.selected_to > text_editing.selected_from)
                    {
                        if (keys.ctrl) // Jump words
                        {
                        }
                        else text_editing.selected_to--;
                    }
                    else
                    {
                        text_editing.select_range_at_start = true;
                        if (text_editing.selected_from > 0)
                        {
                            if (keys.ctrl) // Jump words
                            {
                            }
                            else text_editing.selected_from--;
                        }
                    }
                }
            }
            else // Just move cursor
            {
                text_editing.select_range_at_start = true;
                if (text_editing.selected_to == text_editing.selected_from)
                {
                    if (text_editing.selected_from > 0)
                    {
                        if (keys.ctrl) // Jump words
                        {
                        }
                        else text_editing.selected_from--;
                        text_editing.selected_to = text_editing.selected_from;
                    }
                }
                else // Move cursor at start, remove range selection
                {
                    text_editing.selected_to = text_editing.selected_from;
                }
            }
        }
        else if (keys.right)
        {
            if (keys.shift) // Range select
            {
                if (text_editing.select_range_at_start)
                {
                    if (text_editing.selected_from < text_editing.selected_to)
                    {
                        if (keys.ctrl) // Jump words
                        {
                        }
                        else text_editing.selected_from++;
                    }
                    else
                    {
                        text_editing.select_range_at_start = false;
                        if (text_editing.selected_to < (int)onut::utf8Length(text_editing.text))
                        {
                            if (keys.ctrl) // Jump words
                            {
                            }
                            else text_editing.selected_to++;
                        }
                    }
                }
                else
                {
                    if (text_editing.selected_to < (int)onut::utf8Length(text_editing.text))
                    {
                        if (keys.ctrl) // Jump words
                        {
                        }
                        else text_editing.selected_to++;
                    }
                }
            }
            else // Just move cursor
            {
                text_editing.select_range_at_start = true;
                if (text_editing.selected_to == text_editing.selected_from)
                {
                    if (text_editing.selected_to < (int)onut::utf8Length(text_editing.text))
                    {
                        if (keys.ctrl) // Jump words
                        {
                        }
                        else text_editing.selected_to++;
                        text_editing.selected_from = text_editing.selected_to;
                    }
                }
                else // Move cursor at end, remove range selection
                {
                    text_editing.selected_from = text_editing.selected_to;
                }
            }
        }
        else if (keys.home)
        {
            text_editing.selected_from = 0;
            text_editing.select_range_at_start = true;
            if (!keys.shift) // Range select
            {
                text_editing.selected_to = text_editing.selected_from;
            }
        }
        else if (keys.end)
        {
            text_editing.selected_to = (int)onut::utf8Length(text_editing.text);
            text_editing.select_range_at_start = false;
            if (!keys.shift) // Range select
            {
                text_editing.selected_from = text_editing.selected_to;
            }
        }
        else if (keys.del)
        {
            if (text_editing.selected_from == text_editing.selected_to) // Erase single character
            {
                if (text_editing.selected_from < onut::utf8Length(text_editing.text)) // Only if we're not at the beggining
                {
                    text_editing.text.erase(onut::utf8Pos(text_editing.text, text_editing.selected_from), 1);
                }
            }
            else if (!text_editing.text.empty()) // Erase selected section
            {
                text_editing.text = 
                    text_editing.text.substr(0, onut::utf8Pos(text_editing.text, text_editing.selected_from)) + 
                    text_editing.text.substr(onut::utf8Pos(text_editing.text, text_editing.selected_to), std::string::npos);
                text_editing.selected_to = text_editing.selected_from;
            }
        }
    }

    if (edit_state == State::TextEditing && editing_control == id)
    {
        // Selection/Cursor
        auto text_start = theme->font->measure(text_editing.text.substr(0, onut::utf8Pos(text_editing.text, text_editing.selected_from)));
        auto text_end   = theme->font->measure(text_editing.text.substr(0, onut::utf8Pos(text_editing.text, text_editing.selected_to)));
        drawRect(nullptr, Rect(rect.x + text_start.x - 1.0f, rect.y, text_end.x - text_start.x + 2.0f, rect.w), theme->active_color);
        drawText(theme->font, text_editing.text, rect.Left(), OLeft, theme->text_color);
    }
    else
    {
        drawText(theme->font, *value, rect.Left(), OLeft, theme->text_color);
    }

    bool is_editing_this_field = (edit_state == State::TextEditing && editing_control == id);

    if (state == eUIState::Clicked && !is_editing_this_field)
    {
        edit_state                          = State::TextEditing;
        editing_control                     = id;

        text_editing.text                   = *value;
        text_editing.selected_from          = 0;
        text_editing.selected_to            = std::max(text_editing.selected_from, (int)onut::utf8Length(text_editing.text));
        text_editing.select_range_at_start  = false;
        text_editing.commited               = false;
    }
    else if (state == eUIState::DoubleClicked && is_editing_this_field)
    {
        text_editing.selected_from          = 0;
        text_editing.selected_to            = std::max(text_editing.selected_from, (int)onut::utf8Length(text_editing.text));
        text_editing.select_range_at_start  = false;
    }
    else if (state == eUIState::Down && is_editing_this_field)
    {
        if (left_button.just_down)
        {
            if (keys.shift)
            {
                auto pos = (int)theme->font->caretPos(text_editing.text, mouse.x - rect.x);
                if (text_editing.select_range_at_start)
                {
                    text_editing.selected_from = pos;
                    if (text_editing.selected_from > text_editing.selected_to)
                    {
                        text_editing.select_range_at_start = false;
                        std::swap(text_editing.selected_from, text_editing.selected_to);
                    }
                }
                else
                {
                    text_editing.selected_to = pos;
                    if (text_editing.selected_to < text_editing.selected_from)
                    {
                        text_editing.select_range_at_start = true;
                        std::swap(text_editing.selected_from, text_editing.selected_to);
                    }
                }
            }
            else
            {
                text_editing.selected_from = (int)theme->font->caretPos(text_editing.text, mouse.x - rect.x);
                text_editing.selected_to = text_editing.selected_from;
            }
        }
        else
        {
            auto pos = (int)theme->font->caretPos(text_editing.text, mouse.x - rect.x);
            if (text_editing.select_range_at_start)
            {
                text_editing.selected_from = pos;
                if (text_editing.selected_from > text_editing.selected_to)
                {
                    text_editing.select_range_at_start = false;
                    std::swap(text_editing.selected_from, text_editing.selected_to);
                }
            }
            else
            {
                text_editing.selected_to = pos;
                if (text_editing.selected_to < text_editing.selected_from)
                {
                    text_editing.select_range_at_start = true;
                    std::swap(text_editing.selected_from, text_editing.selected_to);
                }
            }
        }
    }
    
    return ret;
}

void GUIContext::indent()
{
    rect.x += theme->tree_indent;
    rect.z -= theme->tree_indent;
}

void GUIContext::unindent()
{
    rect.x -= theme->tree_indent;
    rect.z += theme->tree_indent;
}

void GUIContext::drawControl()
{
    drawRect(nullptr, rect, theme->control_border_color);
    drawRect(nullptr, rect.Grow(-theme->border_size), theme->dark_color);
}

bool GUIContext::drawBoolControl(bool* value)
{
    auto state = getState(rect);

    bool changed = false;
    if (state == eUIState::Clicked)
    {
        *value = !*value;
        changed = true;
    }

    //drawControl();
    drawRect(nullptr, rect, theme->control_border_color);
    drawRect(nullptr, rect.Grow(-theme->border_size), theme->dark_color);

    if (*value)
    {
        auto r = rect.Grow(-theme->border_size * 2.0f);
        r.x += r.z - r.w;
        r.z = r.w;
        drawRect(nullptr, r, theme->active_color);
    }
    else
    {
        auto r = rect.Grow(-theme->border_size * 2.0f);
        r.z = r.w;
        drawRect(nullptr, r, theme->area_color);
    }

    return changed;
}

bool GUIContext::drawIntControl(int* value)
{
    bool ret = false;

    auto state = getState(rect);

    if (state == eUIState::Down && left_button.just_down)
    {
        int_editing.start_value = *value;
    }
    if (state == eUIState::Drag)
    {
        edit_state = State::IntDragging;
        editing_control = id;
    }
    if (edit_state == State::IntDragging && editing_control == id)
    {
        auto diff = mouse.y - down_pos.y;
        if (diff < 0) diff = std::min(0.0f, diff + DRAG_DISTANCE);
        else if (diff > 0) diff = std::max(0.0f, diff - DRAG_DISTANCE);
        int_editing.value = int_editing.start_value - (int)(diff * INT_DRAG_SENSITIVITY);
        *value = int_editing.value;

        if (!left_button.down)
        {
            ret = int_editing.value != int_editing.start_value;
            edit_state = State::None;
        }
    }

    drawRect(nullptr, rect, theme->area_border_color);

    pushRect();
    rect = rect.Grow(-theme->border_size);
    pushScissor();

    drawRect(nullptr, rect, theme->control_border_color);

    rect.x += theme->control_padding;
    rect.z -= theme->control_padding * 2.0f;

    auto str_value = std::to_string(*value);
    if (edit_state == State::IntDragging && editing_control == id)
    {
        str_value = std::to_string(int_editing.value);
    }
    auto text_edit_changed = drawEditingText(&str_value, state);
    if (text_edit_changed)
    {
        try
        {
            *value = std::stoi(str_value);
            ret = true;
        }
        catch (...)
        {
            ret = false;
        }
    }

    popScissor();
    popRect();

    return ret;
}

bool GUIContext::drawFloatControl(float* value, float step)
{
    bool ret = false;

    auto state = getState(rect);

    if (state == eUIState::Down && left_button.just_down)
    {
        float_editing.start_value = *value;
    }
    if (state == eUIState::Drag)
    {
        edit_state = State::FloatDragging;
        editing_control = id;
    }
    if (edit_state == State::FloatDragging && editing_control == id)
    {
        auto diff = mouse.y - down_pos.y;
        if (diff < 0) diff = std::min(0.0f, diff + DRAG_DISTANCE);
        else if (diff > 0) diff = std::max(0.0f, diff - DRAG_DISTANCE);
        float_editing.value = float_editing.start_value - ((float)(int)(diff * FLOAT_DRAG_SENSITIVITY)) * step;
        *value = float_editing.value;

        if (!left_button.down)
        {
            ret = float_editing.value != float_editing.start_value;
            edit_state = State::None;
        }
    }

    drawRect(nullptr, rect, theme->area_border_color);

    pushRect();
    rect = rect.Grow(-theme->border_size);
    pushScissor();

    drawRect(nullptr, rect, theme->control_border_color);

    rect.x += theme->control_padding;
    rect.z -= theme->control_padding * 2.0f;

    auto val = *value;
    if (edit_state == State::FloatDragging && editing_control == id)
    {
        val = float_editing.value;
    }

    std::stringstream ss;
    ss << val;
    auto str_value = ss.str();
    //auto s = str_value.find_last_not_of("0");
    //str_value = str_value.substr(0, s + 1);

    auto text_edit_changed = drawEditingText(&str_value, state);
    if (text_edit_changed)
    {
        try
        {
            *value = std::stof(str_value);
            ret = true;
        }
        catch (...)
        {
            ret = false;
        }
    }

    popScissor();
    popRect();

    return ret;
}

bool GUIContext::drawTextControl(std::string* value)
{
    auto state = getState(rect);

    bool ret = false;

    drawRect(nullptr, rect, theme->area_border_color);

    pushRect();
    rect = rect.Grow(-theme->border_size);
    pushScissor();

    drawRect(nullptr, rect, theme->control_border_color);

    rect.x += theme->control_padding;
    rect.z -= theme->control_padding * 2.0f;
    ret |= drawEditingText(value, state);

    popScissor();
    popRect();

    return ret;
}

bool GUIContext::bool_prop(bool* value, const std::string& label, const std::string& tooltip)
{
    // Standard control height
    rect.w = theme->control_height - 1;
    pushRect();

    // Content
    drawText(theme->font, label, rect.Left(), OLeft, theme->text_color);

    auto h = theme->bool_control_width * 0.5f;
    rect.x = rect.x + rect.z - theme->bool_control_width;
    rect.z = theme->bool_control_width;
    rect.y += (rect.w - h) * 0.5f;
    rect.w -= (rect.w - h);
    auto changed = drawBoolControl(value);

    // Advance for next control
    popRect();
    rect.y += theme->control_height + theme->control_spacing;

    return changed;
}

bool GUIContext::int_prop(int* value, const std::string& label, const std::string& tooltip)
{
    bool changed = false;

    // Standard control height
    rect.w = theme->control_height - 1;
    pushRect();

    // Content
    drawText(theme->font, label, rect.Left(), OLeft, theme->text_color);

    rect.x = rect.x + rect.z - theme->numeric_control_width;
    rect.z = theme->numeric_control_width;
    changed |= drawIntControl(value);

    // Advance for next control
    popRect();
    rect.y += theme->control_height + theme->control_spacing;

    return changed;
}

bool GUIContext::float_prop(float* value, const std::string& label, const std::string& tooltip)
{
    bool changed = false;

    // Standard control height
    rect.w = theme->control_height - 1;
    pushRect();

    // Content
    drawText(theme->font, label, rect.Left(), OLeft, theme->text_color);

    rect.x = rect.x + rect.z - theme->numeric_control_width;
    rect.z = theme->numeric_control_width;
    changed |= drawFloatControl(value);

    // Advance for next control
    popRect();
    rect.y += theme->control_height + theme->control_spacing;

    return changed;
}

bool GUIContext::Point_prop(Point* value, const std::string& label, const std::string& tooltip)
{
    // Standard control height
    rect.w = theme->control_height - 1;
    pushRect();

    // Content
    drawText(theme->font, label, rect.Left(), OLeft, theme->text_color);

    bool changed = false;
    {
        rect.x = rect.x + rect.z - theme->numeric_control_width * 2.0f - theme->control_margin;
        rect.z = theme->numeric_control_width;
        changed |= drawIntControl(&value->x);
    }
    {
        rect.x += theme->numeric_control_width + theme->control_margin;
        changed |= drawIntControl(&value->y);
    }

    // Advance for next control
    popRect();
    rect.y += theme->control_height + theme->control_spacing;

    return changed;
}

bool GUIContext::Vector2_prop(Vector2* value, const std::string& label, const std::string& tooltip)
{
    // Standard control height
    rect.w = theme->control_height - 1;
    pushRect();

    // Content
    drawText(theme->font, label, rect.Left(), OLeft, theme->text_color);

    bool changed = false;
    {
        rect.x = rect.x + rect.z - theme->numeric_control_width * 2.0f - theme->control_margin;
        rect.z = theme->numeric_control_width;
        changed |= drawFloatControl(&value->x);
    }
    {
        rect.x += theme->numeric_control_width + theme->control_margin;
        changed |= drawFloatControl(&value->y);
    }

    // Advance for next control
    popRect();
    rect.y += theme->control_height + theme->control_spacing;

    return changed;
}

bool GUIContext::Vector3_prop(Vector3* value, const std::string& label, const std::string& tooltip)
{
    // Standard control height
    rect.w = theme->control_height - 1;
    pushRect();

    // Content
    drawText(theme->font, label, rect.Left(), OLeft, theme->text_color);

    bool changed = false;
    {
        rect.x = rect.x + rect.z - theme->numeric_control_width * 3.0f - theme->control_margin * 2.0f;
        rect.z = theme->numeric_control_width;
        changed |= drawFloatControl(&value->x);
    }
    {
        rect.x += theme->numeric_control_width + theme->control_margin;
        changed |= drawFloatControl(&value->y);
    }
    {
        rect.x += theme->numeric_control_width + theme->control_margin;
        changed |= drawFloatControl(&value->z);
    }

    // Advance for next control
    popRect();
    rect.y += theme->control_height + theme->control_spacing;

    return changed;
}

bool GUIContext::Vector4_prop(Vector4* value, const std::string& label, const std::string& tooltip)
{
    // Standard control height
    rect.w = theme->control_height - 1;
    pushRect();

    // Content
    drawText(theme->font, label, rect.Left(), OLeft, theme->text_color);

    bool changed = false;
    {
        rect.x = rect.x + rect.z - theme->numeric_control_width * 4.0f - theme->control_margin * 3.0f;
        rect.z = theme->numeric_control_width;
        changed |= drawFloatControl(&value->x);
    }
    {
        rect.x += theme->numeric_control_width + theme->control_margin;
        changed |= drawFloatControl(&value->y);
    }
    {
        rect.x += theme->numeric_control_width + theme->control_margin;
        changed |= drawFloatControl(&value->z);
    }
    {
        rect.x += theme->numeric_control_width + theme->control_margin;
        changed |= drawFloatControl(&value->w);
    }

    // Advance for next control
    popRect();
    rect.y += theme->control_height + theme->control_spacing;

    return changed;
}

bool GUIContext::Color_prop(Color* value, const std::string& label, const std::string& tooltip)
{
    // Standard control height
    rect.w = theme->control_height - 1;
    pushRect();

    // Content
    drawText(theme->font, label, rect.Left(), OLeft, theme->text_color);

    bool changed = false;
    {
        rect.x = rect.x + rect.z - theme->numeric_control_width * 4.0f - theme->control_margin * 3.0f;
        rect.z = theme->numeric_control_width;
        changed |= drawFloatControl(&value->r, 1.0f / 255.0f);
    }
    {
        rect.x += theme->numeric_control_width + theme->control_margin;
        changed |= drawFloatControl(&value->g, 1.0f / 255.0f);
    }
    {
        rect.x += theme->numeric_control_width + theme->control_margin;
        changed |= drawFloatControl(&value->b, 1.0f / 255.0f);
    }
    {
        rect.x += theme->numeric_control_width + theme->control_margin;
        changed |= drawFloatControl(&value->a, 1.0f / 255.0f);
    }

    // Advance for next control
    popRect();
    rect.y += theme->control_height + theme->control_spacing;

    return changed;
}

bool GUIContext::std_string_prop(std::string* value, const std::string& label, const std::string& tooltip)
{
    // Standard control height
    rect.w = theme->control_height - 1;
    pushRect();

    // Content
    drawText(theme->font, label, rect.Left(), OLeft, theme->text_color);

    rect.x = rect.x + rect.z - theme->text_control_width;
    rect.z = theme->text_control_width;
    auto changed = drawTextControl(value);

    // Advance for next control
    popRect();
    rect.y += theme->control_height + theme->control_spacing;

    return changed;
}

bool GUIContext::header(const std::string& text, bool* expanded)
{
    rect.y += (theme->control_height - theme->header_height) * 0.5f;
    auto adjusted_rect = rect;
    adjusted_rect.x -= theme->control_padding;
    adjusted_rect.z += theme->control_padding * 2.0f;
    adjusted_rect.w = theme->header_height;

    auto state = getState(adjusted_rect);

    if (state == eUIState::Clicked && expanded)
    {
        *expanded = !*expanded;
    }

    drawRect(nullptr, adjusted_rect, theme->header_color);
    drawText(theme->font, text, adjusted_rect.Center(), OCenter, colorForState(state, theme->text_colors));

    if (!expanded || *expanded)
    {
        drawSprite(theme->asset_icons[(int)eAssetType::ExpandedFolder], adjusted_rect.Left(), Color::White, OLeft);
    }
    else
    {
        drawSprite(theme->asset_icons[(int)eAssetType::Folder], adjusted_rect.Left(), Color::White, OLeft);
    }

    rect.y += theme->header_height + theme->control_spacing;

    return expanded ? *expanded : true;
}

#define RESOURCE_PROP(__type__) \
    std::string name = *value ? value->get()->getName() : "-- NULL --"; \
    if (std_string_prop(&name, label)) \
    { \
        *value = g_content_mgr->getResourceAs<__type__>(name); \
        changed |= true; \
    }

bool GUIContext::OTextureRef_prop(OTextureRef* value, const std::string& label, const std::string& tooltip)
{
    bool changed = false;
    RESOURCE_PROP(OTexture);
    return changed;
}

bool GUIContext::OTiledMapRef_prop(OTiledMapRef* value, const std::string& label, const std::string& tooltip)
{
    bool changed = false;
    RESOURCE_PROP(OTiledMap);
    return changed;
}

bool GUIContext::OSoundRef_prop(OSoundRef* value, const std::string& label, const std::string& tooltip)
{
    bool changed = false;
    RESOURCE_PROP(OSound);
    return changed;
}

bool GUIContext::OSpriteAnimRef_prop(OSpriteAnimRef* value, const std::string& label, const std::string& tooltip)
{
    bool changed = false;
    RESOURCE_PROP(OSpriteAnim);
    return changed;
}

bool GUIContext::OModelRef_prop(OModelRef* value, const std::string& label, const std::string& tooltip)
{
    bool changed = false;
    RESOURCE_PROP(OModel);
    return changed;
}

bool GUIContext::OShaderRef_prop(OShaderRef* value, const std::string& label, const std::string& tooltip)
{
    bool changed = false;
    RESOURCE_PROP(OShader);
    return changed;
}

bool GUIContext::ScriptRef_prop(ScriptRef* value, const std::string& label, const std::string& tooltip)
{
    bool changed = false;
    RESOURCE_PROP(Script);
    return changed;
}
