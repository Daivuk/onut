#pragma once
#include <cinttypes>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <typeindex>

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
    class UIControl;    // This control is not rendered. Useful for layout
    class UIPanel;      // Custom rendered panel
    class UIButton;     // Button with text

    unsigned int uiHash(const char* s, unsigned int seed = 0);

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
        UI_PANEL,
        UI_LABEL,
        UI_IMAGE
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

    struct sUIColor
    {
        float r;
        float g;
        float b;
        float a;
        uint32_t packed;
    };

    class UIMouseEvent
    {
    public:
        sUIVector2  mousePos;
        sUIVector2  localMousePos;
        bool        isMouseDown;
    };

    class UIControl;

    class IUIRenderCallback
    {
    public:
        virtual void render(const UIControl* pControl, const sUIRect& rect) const = 0;
    };

    template<typename TobjType, typename TcallbackType>
    class UIRenderCallback : public IUIRenderCallback
    {
    public:
        UIRenderCallback(const TcallbackType& callback) : m_callback(callback) {}

        void render(const UIControl* pControl, const sUIRect& rect) const
        {
            m_callback(dynamic_cast<const TobjType*>(pControl), rect);
        }

    private:
        TcallbackType m_callback;
    };

    class UIContext
    {
        friend UIControl;

    public:
        UIContext(const sUIVector2& screenSize);
        virtual ~UIContext();

        const sUIVector2& getScreenSize() const { return m_screenSize; }

        void resize(const sUIVector2& screenSize);

        template<typename TobjType, typename TcallbackType>
        void addStyle(const char* szStyle, const TcallbackType& renderCallback)
        {
            auto styleId = uiHash(szStyle);
            auto& m = m_callbacks[std::type_index(typeid(TobjType))];
            auto& it = m.find(styleId);
            if (it != m.end())
            {
                delete it->second;
            }
            m[styleId] = new UIRenderCallback<TobjType, TcallbackType>(renderCallback);
        }

        template<typename TobjType>
        const IUIRenderCallback* getStyle(unsigned int styleId) const
        {
            auto& mIt = m_callbacks.find(std::type_index(typeid(TobjType)));
            if (mIt == m_callbacks.end())
            {
                return nullptr;
            }
            auto& it = mIt->second.find(styleId);
            if (it != mIt->second.end())
            {
                return it->second;
            }
            else if (styleId != 0)
            {
                it = mIt->second.find(0);
                if (it != mIt->second.end())
                {
                    return it->second;
                }
            }
            return nullptr;
        }

    private:
        void resolve();
        void dispatchEvents();
        void reset();

        std::unordered_map<std::type_index, std::unordered_map<unsigned int, IUIRenderCallback*>> m_callbacks;
        UIMouseEvent    m_mouseEvent;
        UIMouseEvent    m_lastMouseEvent;
        sUIVector2      m_hoverLocalMousePos;
        sUIVector2      m_screenSize;
        UIControl*      m_pHoverControl = nullptr;
        UIControl*      m_pDownControl = nullptr;
        UIControl*      m_pLastHoverControl = nullptr;
        UIControl*      m_pLastDownControl = nullptr;
    };

    class UIControl
    {
    public:
        UIControl();
        UIControl(const char* szFilename);
        UIControl(const UIControl& other);
        virtual ~UIControl();

        virtual eUIType getType() const { return eUIType::UI_CONTROL; }

        void add(UIControl* pChild);
        void remove(UIControl* pChild);
        UIControl* getChild(const std::string& name, bool bSearchSubChildren = true);
        template<typename TuiType>
        TuiType* getChild(const std::string& name, bool bSearchSubChildren = true)
        {
            return dynamic_cast<TuiType*>(getChild(name, bSearchSubChildren));
        }

        void setStyle(const char* szStyle);
        int getStyle() const { return m_style; }

        void retain();
        void release();

        void update(UIContext& context, const sUIVector2& mousePos, bool bMouseDown);
        void render(const UIContext& context) const;

        const sUIRect& getRect() const { return m_rect; }
        void setRect(const sUIRect& rect);

        sUIRect getWorldRect() const;

        const std::string& getName() const { return m_name; }
        void setName(const std::string& name);

        bool isEnabled() const { return m_isEnabled && m_isVisible; }
        void setIsEnabled(bool bIsEnabled);

        bool isVisible() const { return m_isVisible; }
        void setIsVisible(bool bIsVisible);

        bool isClickThrough() const { return m_isClickThrough; }
        void setIsClickThrough(bool bIsClickThrough);

        eUIState getState(const UIContext& context) const;

        void* getUserData() const { return m_pUserData; }
        void setUserData(void* pUserData);

        using TfnMouseEvent = std::function<void(UIControl*, const UIMouseEvent&)>;
        TfnMouseEvent onClick;
        TfnMouseEvent onMouseMove;
        TfnMouseEvent onMouseDown;
        TfnMouseEvent onMouseUp;
        TfnMouseEvent onMouseEnter;
        TfnMouseEvent onMouseLeave;

    protected:
        virtual void load(const rapidjson::Value& jsonNode);

        void updateInternal(UIContext& context, const sUIRect& parentRect, bool bIsParentHover = false);
        void renderInternal(const UIContext& context, const sUIRect& parentRect) const;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) const {}

    private:
        sUIRect getWorldRect(const sUIRect& parentRect) const;

        bool                    m_isEnabled = true;
        bool                    m_isClickThrough = false;
        bool                    m_isVisible = true;
        sUIRect                 m_rect = sUIRect{{0, 0}, {0, 0}};
        std::vector<UIControl*> m_children;
        eUIAlign                m_align = eUIAlign::TOP_LEFT;
        eUIPosType              m_posType[2];
        eUIDimType              m_dimType[2];
        int                     m_style = 0;
        std::string             m_name;
        int32_t                 m_refCount = 0;
        UIControl*              m_pParent = nullptr;
        void*                   m_pUserData = nullptr;
    };

    class UIButton : public UIControl
    {
        friend UIControl;

    public:
        UIButton() {}
        UIButton(const UIButton& other);

        virtual eUIType getType() const override { return eUIType::UI_BUTTON; }

        const std::string& getCaption() const { return m_caption; }
        void setCaption(const std::string& caption);

    protected:
        virtual void load(const rapidjson::Value& jsonNode) override;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) const override;

    private:
        std::string m_caption;
    };

    class UIPanel : public UIControl
    {
        friend UIControl;

    public:
        UIPanel() {}
        UIPanel(const UIPanel& other);

        virtual eUIType getType() const override { return eUIType::UI_PANEL; }

        const sUIColor& getColor() const { return m_color; }
        void setColor(const sUIColor& color);

    protected:
        virtual void load(const rapidjson::Value& jsonNode) override;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) const override;

    private:
        sUIColor    m_color;
    };

    class UILabel : public UIControl
    {
        friend UIControl;

    public:
        UILabel() {}
        UILabel(const UILabel& other);

        virtual eUIType getType() const override { return eUIType::UI_LABEL; }

        const std::string& getText() const { return m_text; }
        void setText(const std::string& text);

    protected:
        virtual void load(const rapidjson::Value& jsonNode) override;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) const override;

    private:
        std::string m_text;
    };

    class UIImage : public UIControl
    {
        friend UIControl;

    public:
        UIImage() {}
        UIImage(const UIImage& other);

        virtual eUIType getType() const override { return eUIType::UI_IMAGE; }

        const std::string& getImage() const { return m_image; }
        void setImage(const std::string& image);

    protected:
        virtual void load(const rapidjson::Value& jsonNode) override;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) const override;

    private:
        std::string m_image;
    };
}
