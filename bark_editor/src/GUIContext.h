#pragma once

#include <onut/ForwardDeclaration.h>
#include <onut/Rect.h>
#include <onut/Vector2.h>
#include <onut/Color.h>
#include <onut/Point.h>
#include <vector>
#include <string>

OForwardDeclare(SpriteBatch);
OForwardDeclare(Font);
OForwardDeclare(Texture);
OForwardDeclare(Renderer);
OForwardDeclare(TiledMap);
OForwardDeclare(SpriteAnim);
OForwardDeclare(Sound);
OForwardDeclare(Shader);
OForwardDeclare(Model);
ForwardDeclare(Script);

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
    SetDisableStyle,
    SetEnableStyle
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
    enum class State
    {
        None,
        TextEditing,
        IntDragging,
        FloatDragging
    };

    struct TextEditing
    {
        bool        select_range_at_start   = true;
        bool        commited                = false;
        int         selected_from           = 0;
        int         selected_to             = 0;
        std::string text;
    };

    struct IntEditing
    {
        int start_value = 0;
        int value       = 0;
    };

    struct FloatEditing
    {
        float start_value = 0;
        float value       = 0;
    };

    struct Scrolling
    {
        float   v_content_start = 0.0f;
        float   v_content_size  = 0.0f;
        float   v_view_size     = 0.0f;
        float   amount          = 0.0f;
    };

    struct Keys
    {
        bool ctrl = false, shift = false, alt  = false;                 // Modifiers
        bool esc  = false, enter = false, del  = false;                 // Actions
        bool left = false, right = false, home = false, end = false;    // Navigation
    };

    struct Button
    {
        bool down            = false;
        bool just_down       = false;
        bool clicked         = false;
        bool double_clicked  = false;
    };

    Rect                rect;
    std::vector<Rect>   rect_stack;
    std::vector<Rect>   scissor_stack;
    std::vector<bool>   enable_stack;
    OSpriteBatch*       sb;
    Theme*              theme;
    ORenderer*          r;
    onut::RenderStates* rs;
    size_t              saved_draw_point;
    UIDrag              drag;
    eUICursorType       cursor_type;
    uint64_t            id = 0;
    bool                drawing_enable_state;

    // Mouse/Input states
    Vector2             mouse;
    Vector2             down_pos;
    uint64_t            down_id;

    uint64_t            editing_control;
    State               edit_state = State::None;
    Button              left_button;
    TextEditing         text_editing;
    IntEditing          int_editing;
    FloatEditing        float_editing;
    Scrolling           scrolling;
    Keys                keys;

    std::vector<UIDrawCall> draw_calls;

    GUIContext();
    ~GUIContext();

    bool drawEditingText(std::string* value, eUIState state);
    bool isEnabled();

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
    void reset();

    void begin();
    void end();

    void pushRect();
    Rect popRect();

    void pushScissor();
    Rect popScissor();

    void indent();
    void unindent();
    void pushEnableState(bool enabled);
    void popEnableState();

    void drawPanel();
    eUIState drawTab(const std::string& name, float& offset,
                     const Color& color, const Color& border_color,
                     const Color& text_colors, bool close_btn = false);
    eUIState drawActiveTab(const std::string& name, float& offset, bool close_btn = false);
    eUIState drawInactiveTab(const std::string& name, float& offset, bool close_btn = false);

    void drawArea();
    bool drawToolButton(const OTextureRef& icon, const Vector2& pos);
    eUIState drawListItem(const std::string& text, const OTextureRef& icon = nullptr, int indent = 0, bool enabled = true, bool selected = false);
    eUIState drawHSplitHandle();
    eUIState drawVSplitHandle();

    void beginVScrollArea(float scroll_amount);
    void endVScrollArea(float* scroll_amount);
    bool vScroll(float* amount);

    void drawControl();
    bool drawBoolControl(bool* value);
    bool drawIntControl(int* value);
    bool drawFloatControl(float* value, float step = 1.0f);
    bool drawTextControl(std::string* value);

    bool header(const std::string& text, bool* expanded = nullptr);

    bool bool_prop(bool* value, const std::string& label, const std::string& tooltip = "");
    bool int_prop(int* value, const std::string& label, const std::string& tooltip = "");
    bool float_prop(float* value, const std::string& label, const std::string& tooltip = "");
    bool std_string_prop(std::string* value, const std::string& label, const std::string& tooltip = "");
    bool Point_prop(Point* value, const std::string& label, const std::string& tooltip = "");
    bool Vector2_prop(Vector2* value, const std::string& label, const std::string& tooltip = "");
    bool Vector3_prop(Vector3* value, const std::string& label, const std::string& tooltip = "");
    bool Vector4_prop(Vector4* value, const std::string& label, const std::string& tooltip = "");
    bool Color_prop(Color* value, const std::string& label, const std::string& tooltip = "");
    bool OTextureRef_prop(OTextureRef* value, const std::string& label, const std::string& tooltip = "");
    bool OTiledMapRef_prop(OTiledMapRef* value, const std::string& label, const std::string& tooltip = "");
    bool OSoundRef_prop(OSoundRef* value, const std::string& label, const std::string& tooltip = "");
    bool OSpriteAnimRef_prop(OSpriteAnimRef* value, const std::string& label, const std::string& tooltip = "");
    bool OModelRef_prop(OModelRef* value, const std::string& label, const std::string& tooltip = "");
    bool OShaderRef_prop(OShaderRef* value, const std::string& label, const std::string& tooltip = "");
    bool ScriptRef_prop(ScriptRef* value, const std::string& label, const std::string& tooltip = "");
};
