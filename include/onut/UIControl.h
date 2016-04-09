#pragma once
// Onut
#include <onut/Maths.h>
#include <onut/UIEvents.h>

// STL
#include <unordered_map>
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ContentManager);
OForwardDeclare(UIContext);
OForwardDeclare(UIControl);
namespace rapidjson
{
    class CrtAllocator;
    template <typename BaseAllocator> class MemoryPoolAllocator;
    template <typename Encoding, typename Allocator> class GenericValue;
    template<typename CharType> struct UTF8;
    typedef GenericValue<UTF8<char>, MemoryPoolAllocator<CrtAllocator>> Value;
    typedef MemoryPoolAllocator<CrtAllocator> Allocator;
}

namespace onut
{
    class UIControl : public std::enable_shared_from_this<UIControl>
    {
    public:
        class Property
        {
        public:
            enum class Type
            {
                Int,
                Float,
                String,
                Bool
            };

            Property();
            Property(const rapidjson::Value& jsonNode);
            Property(const Property& other);
            Property& operator=(const Property& other);
            ~Property();

            Type getType() const { return m_type; };

            const char* getString() const;
            int getInt() const;
            float getFloat() const;
            bool getBool() const;

        private:
            Type m_type;
            union
            {
                char* m_szString;
                int m_int;
                float m_float;
                bool m_bool;
            };
        };

        enum class DimType
        {
            /** Dimension in pixel */
            Absolute,
            /** Relative to parent's dimensions. If 0, it will be parent's dimension. Negative will be less than parent by this value in pixel */
            Relative,
            /** Percentage of the parent's dimensions. 0-1 */
            Percentage
        };

        enum class PosType
        {
            /** Position is relative to parent's position. E.g.: fx = parent.x + x */
            Relative,
            /** Position is percentage of parent's dimensions. fx = parent.x + parent.width * x */
            Percentage
        };

        enum class AnchorType
        {
            /** Position is relative to parent's position. E.g.: fx = parent.x + x */
            Pixel,
            /** Position is percentage of parent's dimensions. fx = parent.x + parent.width * x */
            Percentage
        };

        enum class Type
        {
            Control,
            Button,
            Panel,
            Label,
            Image,
            CheckBox,
            TreeView,
            TextBox
        };

        enum class State
        {
            /** Normal state. */
            Normal,
            /** Control is disabled */
            Disabled,
            /** Mouse is hovering the control. Or the control has a DOWN focus but mouse is not hovering. */
            Hover,
            /** Mouse is down while hovering the control */
            Down
        };

        using Controls = std::vector<OUIControlRef>;

        static OUIControlRef create();
        static OUIControlRef createFromFile(const std::string& filename, OContentManagerRef pContentManager = nullptr);

        UIControl(const UIControl& other) = delete;
        virtual void operator=(const UIControl& other);
        OUIControlRef copy() const;

        // Public properties
        bool isEnabled = true; /*! Disabled control and their children won't receive mouse events */
        bool isClickThrough = false; /*! Same as isEnabled, but will traverse children */
        bool isVisible = true; /*! Visible or not. Invisible controls don't receive mouse events */
        bool clipChildren = false; /*! Will trigger a scissor on the children. Usefull for lists */
        Rect rect; /*! Local rectangle. Greatly influenced by align, anchor, pos and dim types. \see getWorldRect */
        onut::Align align = onut::Align::TopLeft; /*! Alignement inside parent control */
        PosType xType = PosType::Relative; /*! x position type */
        PosType yType = PosType::Relative; /*! y position type */
        DimType widthType = DimType::Absolute; /*! width type */
        DimType heightType = DimType::Absolute; /*! height type */
        AnchorType xAnchorType = AnchorType::Percentage; /*! x anchor type */
        AnchorType yAnchorType = AnchorType::Percentage; /*! y anchor type */
        Vector2 anchor; /*! Anchor position */
        std::string name; /*! This control's name. Can be used to search for it */
        void* pUserData = nullptr; /*! Set whatever data you want on this. it will never be accessed or freed by onut::UI */
        std::shared_ptr<void> pSharedUserData = nullptr; /*! Set whatever data you want on this. it will never be accessed or freed by onut::UI */

        void save(const std::string& filename) const;

        virtual Type getType() const { return Type::Control; }
        virtual bool isNavigatable() const { return false; }

        // Child methods
        void add(OUIControlRef pChild);
        void insert(OUIControlRef pChild, const OUIControlRef& pBefore);
        void insertAfter(OUIControlRef pChild, const OUIControlRef& pAfter);
        bool insertAt(OUIControlRef pChild, size_t index);
        void remove();
        void remove(OUIControlRef pChild);
        void removeAll();
        OUIControlRef getChild(const std::string& name, bool bSearchSubChildren = true);
        template<typename TuiType>
        std::shared_ptr<TuiType> getChild(const std::string& name, bool bSearchSubChildren = true)
        {
            return ODynamicCast<TuiType>(getChild(name, bSearchSubChildren));
        }
        OUIControlRef getChild(const OUIContextRef& context, const Vector2& mousePos, bool bSearchSubChildren = true, bool bIgnoreClickThrough = true);
        template<typename TuiType>
        std::shared_ptr<TuiType> getChild(const OUIContextRef& context, const Vector2& mousePos, bool bSearchSubChildren = true, bool bIgnoreClickThrough = true)
        {
            return ODynamicCast<TuiType>(getChild(context, mousePos, bSearchSubChildren, bIgnoreClickThrough));
        }

        void setStyle(const char* szStyle);
        int getStyle() const { return m_style; }
        const std::string& getStyleName() const { return m_styleName; }

        void update(const OUIContextRef& context, const Vector2& mousePos, bool bMouse1Down, bool bMouse2Down = false, bool bMouse3Down = false, bool bNavL = false, bool bNavR = false, bool bNavU = false, bool bNavD = false, bool bControl = false, float scroll = 0.f);
        void render(const OUIContextRef& context);

        Rect getWorldRect(const OUIContextRef& context) const;
        void setWorldRect(const Rect& rect, const OUIContextRef& context);

        Vector2 getAnchorInPixel() const;
        Vector2 getAnchorInPercentage() const;
        void setAnchorPercent(const Vector2& anchor);

        State getState(const OUIContextRef& context) const;
        bool hasFocus(const OUIContextRef& context) const;

        OUIControlRef getParent() const { return m_pParent.lock(); }

        const Property& getProperty(const std::string& name) const;

        const Controls& getChildren() const { return m_children; };

        // Event callbacks
        using TfnMouseEvent = std::function<void(const OUIControlRef&, const UIMouseEvent&)>;
        TfnMouseEvent onClick;
        TfnMouseEvent onDoubleClick;
        TfnMouseEvent onMouseMove;
        TfnMouseEvent onMouseDown;
        TfnMouseEvent onMouseUp;
        TfnMouseEvent onMouseEnter;
        TfnMouseEvent onMouseLeave;
        TfnMouseEvent onMouseScroll;

        TfnMouseEvent onRightClick;
        TfnMouseEvent onRightDoubleClick;
        TfnMouseEvent onRightMouseDown;
        TfnMouseEvent onRightMouseUp;

        TfnMouseEvent onMiddleClick;
        TfnMouseEvent onMiddleDoubleClick;
        TfnMouseEvent onMiddleMouseDown;
        TfnMouseEvent onMiddleMouseUp;

        using TfnFocusEvent = std::function<void(const OUIControlRef&)>;
        TfnFocusEvent onGainFocus;
        TfnFocusEvent onLoseFocus;

        using TfnKeyEvent = std::function<void(const OUIControlRef&, const UIKeyEvent&)>;
        TfnKeyEvent onKeyDown;

        bool visit(const std::function<bool(const OUIControlRef&, const Rect&)>& callback, const Rect& parentRect);
        bool visitChildrenFirst(const std::function<bool(const OUIControlRef&, const Rect&)>& callback, const Rect& parentRect);
        bool visitEnabled(const std::function<bool(const OUIControlRef&, const Rect&)>& callback, const Rect& parentRect);
        bool visitChildrenFirstEnabled(const std::function<bool(const OUIControlRef&, const Rect&)>& callback, const Rect& parentRect);
        bool visitVisible(const std::function<bool(const OUIControlRef&, const Rect&)>& callback, const Rect& parentRect);
        bool visitChildrenFirstVisible(const std::function<bool(const OUIControlRef&, const Rect&)>& callback, const Rect& parentRect);

    protected:
        UIControl();

        friend UIContext;

        virtual void load(const rapidjson::Value& jsonNode);
        virtual void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const;

        void updateInternal(const OUIContextRef& context, const Rect& parentRect);
        void renderInternal(const OUIContextRef& context, const Rect& parentRect);
        virtual void renderControl(const OUIContextRef& context, const Rect& rect) {}

        virtual void onClickInternal(const UIMouseEvent& evt) {}
        virtual void onMouseDownInternal(const UIMouseEvent& evt) {}
        virtual void onMouseMoveInternal(const UIMouseEvent& evt) {}
        virtual void onMouseScrollInternal(const UIMouseEvent& evt) {}
        virtual void onMouseUpInternal(const UIMouseEvent& evt) {}
        virtual void onGainFocusInternal(const UIFocusEvent& evt) {}
        virtual void onLoseFocusInternal(const UIFocusEvent& evt) {}
        virtual void onWriteInternal(char c, const OUIContextRef& context) {}
        virtual void onKeyDownInternal(const UIKeyEvent& evt) {}

    private:
        using PropertyMap = std::unordered_map<std::string, Property>;

        Rect getWorldRect(const Rect& parentRect) const;
        void getChild(const OUIContextRef& context,
                      const Vector2& mousePos,
                      bool bSearchSubChildren,
                      bool bIgnoreClickThrough,
                      const Rect& parentRect,
                      OUIControlRef& pHoverControl);

        Controls m_children;
        int m_style = 0;
        std::string m_styleName;
        OUIControlWeak m_pParent;
        PropertyMap m_properties;
    };
};

extern OUIControlRef oUI;

OUIControlRef OFindUI(const std::string& name);
template <typename Tui>
inline std::shared_ptr<Tui> OFindUI(const std::string& name)
{
    return ODynamicCast<Tui>(oUI->getChild(name));
}

OUIControlRef OLoadUI(const std::string& filename);
