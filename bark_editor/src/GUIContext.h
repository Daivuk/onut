#pragma once

#include <onut/ForwardDeclaration.h>
#include <onut/Rect.h>
#include <onut/Vector2.h>
#include <onut/Color.h>
#include <vector>
#include <string>

OForwardDeclare(SpriteBatch);
OForwardDeclare(Font);
OForwardDeclare(Texture);
OForwardDeclare(Renderer);

#define GUI_START_V_SCROLLABLE(ctx, scroll) \
{ \
    ctx->v_scroll_view_size = ctx->rect.w; \
    ctx->pushRect(); \
    ctx->pushScissor(); \
    ctx->rect = ctx->rect.Grow(-ctx->theme->panel_padding); \
    ctx->rect.y -= scroll; \
    ctx->v_scroll_content_start = ctx->rect.y;

#define GUI_END_V_SCROLLABLE(ctx, scroll) \
    ctx->v_scroll_content_size = (ctx->rect.y + ctx->rect.w) - ctx->v_scroll_content_start; \
    ctx->popScissor(); \
    ctx->popRect(); \
}

namespace onut
{
    class RenderStates;
}

class Theme;

enum class eUIState
{
    None,
    Hover,
    Down,
    Drag,
    Drop,
    Clicked,
    DoubleClicked,
    Close
};

enum class eUIDrawCall
{
    Outline,
    Rect,
    Text,
    Sprite,
    Slice9,
    SetScissor,
    UnsetScissor,
};

struct UIDrawCall
{
    eUIDrawCall type;
    Rect rect;
    Vector2 pos;
    Vector2 origin;
    Vector4 padding;
    float thickness;
    Color color;
    OFontRef font;
    std::string text;
    OTextureRef texture;
};

enum class eUICursorType
{
    Arrow,
    SizeEW,
    SizeNS,
    SizeAll
};

enum class eUIDrag
{
    None,
    Tab,
    HSplit,
    VSplit
};

struct UIDrag
{
    eUIDrag type;
    std::vector<UIDrawCall> draw_calls;
};

class GUIContext final
{
public:
    Rect                rect;
    std::vector<Rect>   rect_stack;
    std::vector<Rect>   scissor_stack;
    OSpriteBatch*       sb;
    Theme*              theme;
    ORenderer*          r;
    onut::RenderStates* rs;
    size_t              saved_draw_point;
    UIDrag              drag;
    eUICursorType       cursor_type;

    // Mouse/Input states
    Vector2             mouse;
    Vector2             down_pos;
    bool                ctrl = false, shift = false, alt = false;
    bool                down = false;
    bool                clicked = false;
    bool                double_clicked = false;
    float               v_scroll_content_start = 0.0f;
    float               v_scroll_content_size = 0.0f;
    float               v_scroll_view_size = 0.0f;
    float               scroll_value = 0.0f;

    std::vector<UIDrawCall> draw_calls;

    void saveDrawPoint();
    void rewindDrawCalls();
    UIDrawCall* getNextDrawCall();
    void drawDrawCall(const UIDrawCall& draw_call);

    void drawOutline(const Rect& rect, float thickness, const Color& color);
    void drawRect(const OTextureRef& texture, const Rect& rect, const Color& color);
    void drawText(const OFontRef& font, const std::string& text, const Vector2& pos, const Vector2& origin, const Color& color);
    void drawSprite(const OTextureRef& texture, const Vector2& pos, const Color& color, const Vector2& origin);
    void drawSlice9(const OTextureRef& texture, const Rect& rect, const Vector4& padding, const Color& color);

    eUIState getState(const Rect& rect);
    const Color& colorForState(eUIState state, Color** colors);

    void update();

    void begin();
    void end();

    void pushRect();
    Rect popRect();

    void pushScissor();
    Rect popScissor();

    void drawPanel();
    eUIState drawTab(const std::string& name, float& offset,
                 const Color& color, const Color& border_color,
                 const Color& text_colors, bool close_btn = false);
    eUIState drawActiveTab(const std::string& name, float& offset, bool close_btn = false);
    eUIState drawInactiveTab(const std::string& name, float& offset, bool close_btn = false);

    void drawArea();
    bool drawToolButton(const OTextureRef& icon, const Vector2& pos);
    eUIState drawListItem(const std::string& text, const OTextureRef& icon = nullptr, int indent = 0, bool enabled = true, bool selected = false);
    bool vScroll(float* amount);

    eUIState drawHSplitHandle();
    eUIState drawVSplitHandle();
};
