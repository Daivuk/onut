#pragma once
#include <cinttypes>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

// Forward delcare needed rapidjson classes.
namespace rapidjson
{
    class CrtAllocator;
    template <typename BaseAllocator> class MemoryPoolAllocator;
    template <typename Encoding, typename Allocator> class GenericValue;
    template<typename CharType> struct UTF8;
    typedef GenericValue<UTF8<char>, MemoryPoolAllocator<CrtAllocator>> Value;
}

namespace onut
{
    enum class eUIAlign : uint8_t
    {
        TOP_LEFT,
        TOP,
        TOP_RIGHT,
        LEFT,
        CENTER,
        RIGHT,
        BOTTOM_LEFT,
        BOTTOM,
        BOTTOM_RIGHT
    };

    enum class eUIDimType : uint8_t
    {
        /** Dimension in pixel */
        DIM_ABSOLUTE,
        /** Relative to parent's dimensions. If 0, it will be parent's dimension. Negative will be less than parent by this value in pixel */
        DIM_RELATIVE,
        /** Percentage of the parent's dimensions. 0-1 */
        DIM_PERCENTAGE
    };

    enum class eUIPosType : uint8_t
    {
        /** Position is relative to parent's position. E.g.: fx = parent.x + x */
        POS_RELATIVE,
        /** Position is percentage of parent's dimensions. fx = parent.x + parent.width * x */
        POS_PERCENTAGE
    };

    enum class eUIType : uint8_t
    {
        UI_CONTROL,
        UI_BUTTON,
        UI_PANEL
    };

    enum class eUIState : uint8_t
    {
        /** Normal state. */
        NORMAL,
        /** Control is disabled */
        DISABLED,
        /** Mouse is hovering the control. Or the control has a DOWN focus but mouse is not hovering. */
        HOVER,
        /** Mouse is down while hovering the control */
        DOWN
    };

    struct sUIVector2
    {
        float x;
        float y;
    };
    
    struct sUIRect
    {
        sUIVector2 position;
        sUIVector2 size;
    };

    class UIMouseEvent
    {
    public:
        sUIVector2  mousePos;
        sUIVector2  localMousePos;
        bool        isMouseDown;
    };

    class UIControl;

    class UIContext
    {
        friend UIControl;

    public:
        using TfnCallback = std::function<void(const UIControl*, const sUIRect&)>;

        UIContext(const sUIVector2& screenSize);

        const sUIVector2& getScreenSize() const { return m_screenSize; }
        const TfnCallback& getStyle(int styleId) const;

        void resize(const sUIVector2& screenSize);
        void addStyle(const char* szStyle, const TfnCallback& renderCallback);

    private:
        void resolve();
        void dispatchEvents();
        void reset();

        std::unordered_map<int, TfnCallback>    m_callbacks;
        UIMouseEvent                            m_mouseEvent;
        UIMouseEvent                            m_lastMouseEvent;
        sUIVector2                              m_hoverLocalMousePos;
        sUIVector2                              m_screenSize;
        UIControl*                              m_pHoverControl = nullptr;
        UIControl*                              m_pDownControl = nullptr;
        UIControl*                              m_pLastHoverControl = nullptr;
        UIControl*                              m_pLastDownControl = nullptr;
    };

    class UIControl
    {
    public:
        UIControl(const char* szFilename);
        UIControl(const UIControl& other);
        virtual ~UIControl();

        virtual eUIType getType() const { return eUIType::UI_CONTROL; }

        void add(UIControl* pChild);
        void remove(UIControl* pChild);
        UIControl* getChild(const std::string& name, bool bSearchSubChildren = true);

        void setStyle(const char* szStyle);
        int getStyle() const { return m_style; }

        void retain();
        void release();

        void update(UIContext& context, const sUIVector2& mousePos, bool bMouseDown);
        void render(const UIContext& context) const;

        const sUIRect& getRect() const { return m_rect; }
        void setRect(const sUIRect& rect);

        const std::string& getName() const { return m_name; }
        void setName(const std::string& name);

        bool isEnabled() const { return m_isEnabled && m_isVisible; }
        void setIsEnabled(bool bIsEnabled);

        bool isVisible() const { return m_isVisible; }
        void setIsVisible(bool bIsVisible);

        eUIState getState(const UIContext& context) const;

        void* getUserData() const { return m_pUserData; }
        void setUserData(void* pUserData);

        using TfnMouseEvent = std::function<void(const UIControl*, const UIMouseEvent&)>;
        TfnMouseEvent onClick;
        TfnMouseEvent onMouseMove;
        TfnMouseEvent onMouseDown;
        TfnMouseEvent onMouseUp;
        TfnMouseEvent onMouseEnter;
        TfnMouseEvent onMouseLeave;

    protected:
        UIControl() {}

        virtual void load(const rapidjson::Value& jsonNode);

        void updateInternal(UIContext& context, const sUIRect& parentRect);
        void renderInternal(const UIContext& context, const sUIRect& parentRect) const;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) const {}

    private:
        sUIRect getWorldRect(const sUIRect& parentRect) const;

        bool                    m_isEnabled;
        bool                    m_isVisible;
        sUIRect                 m_rect;
        std::vector<UIControl*> m_children;
        eUIAlign                m_align;
        eUIPosType              m_posType[2];
        eUIDimType              m_dimType[2];
        int                     m_style;
        std::string             m_name;
        int32_t                 m_refCount = 0;
        UIControl*              m_pParent = nullptr;
        void*                   m_pUserData = nullptr;
    };

    class UIButton : public UIControl
    {
        friend UIControl;

    public:
        UIButton(const UIButton& other);
        virtual ~UIButton();

        virtual eUIType getType() const override { return eUIType::UI_BUTTON; }

        const std::string& getCaption() const { return m_caption; }
        void setCaption(const std::string& caption);

    protected:
        UIButton() {}

        virtual void load(const rapidjson::Value& jsonNode) override;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) const override;

    private:
        std::string m_caption;
    };

    class UIPanel : public UIControl
    {
        friend UIControl;

    public:
        UIPanel(const UIPanel& other);

        virtual eUIType getType() const override { return eUIType::UI_PANEL; }

    protected:
        virtual void renderControl(const UIContext& context, const sUIRect& rect) const override;

        UIPanel() {}
    };
}
