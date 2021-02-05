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
    Clicked
};

enum class eUIDrawCall
{
    Outline,
    Rect,
    Text,
    Sprite
};

struct UIDrawCall
{
    eUIDrawCall type;
    Rect rect;
    Vector2 pos;
    Vector2 origin;
    float thickness;
    Color color;
    OFontRef font;
    std::string text;
    OTextureRef texture;
};

enum class eUIDrag
{
    None,
    Tab
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
    OSpriteBatch*       sb;
    Theme*              theme;
    ORenderer*          r;
    onut::RenderStates* rs;
    size_t              saved_draw_point;
    UIDrag              drag;

    // Mouse/Input states
    Vector2             mouse;
    Vector2             down_pos;
    bool                ctrl, shift, alt;
    bool                down;
    bool                clicked;

    std::vector<UIDrawCall> draw_calls;

    void saveDrawPoint();
    void rewindDrawCalls();
    UIDrawCall* getNextDrawCall();
    void drawDrawCall(const UIDrawCall& draw_call);

    void drawOutline(const Rect& rect, float thickness, const Color& color);
    void drawRect(const OTextureRef& texture, const Rect& rect, const Color& color);
    void drawText(const OFontRef& font, const std::string& text, const Vector2& pos, const Vector2& origin, const Color& color);
    void drawSprite(const OTextureRef& texture, const Vector2& pos, const Color& color, const Vector2& origin);

    eUIState getState(const Rect& rect);
    const Color& colorForState(eUIState state, Color** colors);

    void update();

    void begin();
    void end();

    void beginDraw();
    void endDraw();

    void pushRect();
    Rect popRect();

    void drawPanel();
    eUIState drawTab(const std::string& name, float& offset,
                 const Color& color, const Color& border_color,
                 Color** text_colors);
    eUIState drawActiveTab(const std::string& name, float& offset);
    eUIState drawInactiveTab(const std::string& name, float& offset);

    void drawArea();
    bool drawToolButton(const OTextureRef& icon, const Vector2& pos);
};
