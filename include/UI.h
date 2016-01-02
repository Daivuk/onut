#pragma once
#include <chrono>
#include <cinttypes>
#include <functional>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

#ifdef WIN32
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif

// Forward delcare needed rapidjson classes.
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

    enum class eUIAnchorType : uint8_t
    {
        /** Position is relative to parent's position. E.g.: fx = parent.x + x */
        ANCHOR_PIXEL,
        /** Position is percentage of parent's dimensions. fx = parent.x + parent.width * x */
        ANCHOR_PERCENTAGE
    };

    enum class eUIType : uint8_t
    {
        UI_CONTROL,
        UI_BUTTON,
        UI_PANEL,
        UI_LABEL,
        UI_IMAGE,
        UI_CHECKBOX,
        UI_TREEVIEW,
        UI_TEXTBOX
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

    enum class eUICheckBehavior : uint8_t
    {
        /** Normal checkbox. Can toggle it on and off regardless of sibblings */
        NORMAL,
        /** Behaves like a radio button. Only one sibblings can be checked. And one HAS to be checked */
        EXCLUSIVE,
        /** Like exclusive, but all sibblings can be toggled off */
        CHK_OPTIONAL
    };

    struct sUIVector2
    {
        float x;
        float y;
        sUIVector2() : x(0), y(0) {}
        sUIVector2(float in_x, float in_y) : x(in_x), y(in_y) {}
    };
    
    struct sUIRect
    {
        sUIVector2 position;
        sUIVector2 size;
    };

    struct sUIPadding
    {
        float left;
        float top;
        float right;
        float bottom;
        sUIPadding() : left(0), right(0), top(0), bottom(0) {}
        sUIPadding(float in_left, float in_right, float in_top, float in_bottom) : left(in_left), right(in_right), top(in_top), bottom(in_bottom) {}
    };

    struct sUIColor
    {
        float r;
        float g;
        float b;
        float a;
        uint32_t packed;
        void unpack();
        void pack();
        sUIColor() : r(1.f), g(1.f), b(1.f), a(1.f), packed(0xffffffff) {}
        sUIColor(float in_r, float in_g, float in_b, float in_a) : r(in_r), g(in_g), b(in_b), a(in_a) { pack(); }
    };

    // Components
    struct sUIFont
    {
        enum eFontFlag : uint8_t
        {
            AUTO_FIT_SIZE = 0x01,
            ELLIPSIS = 0x02,
            WORD_WRAP = 0x04
        };
        sUIColor            color;
        eUIAlign            align;
        sUIPadding          padding;
        std::string         typeFace;
        float               size;
        uint8_t             flags;
        float               minSize;
        sUIFont() : align(eUIAlign::TOP_LEFT), typeFace("Arial"), size(12.f), flags(0), minSize(12.f) {}
    };

    struct sUITextComponent
    {
        std::string         text;
        sUIFont             font;
    };

    struct sUIImageComponent
    {
        std::string         filename;
        sUIColor            color;
    };

    struct sUIScale9Component
    {
        sUIImageComponent   image;
        bool                isScaled9;
        bool                isRepeat;
        sUIPadding          padding;
        sUIScale9Component() : isScaled9(false), isRepeat(false) {}
    };

    struct sUIIconComponent
    {
        sUIScale9Component  background;
        eUIAlign            align;
        sUIRect             rect;
        sUIIconComponent() : align(eUIAlign::TOP_LEFT) {}
    };

    class UIContext;
    class UITreeViewItem;
    class UITextBox;

    class UIMouseEvent
    {
    public:
        sUIVector2  mousePos;
        sUIVector2  localMousePos;
        bool        isMouseDown = false;
        UIContext*  pContext;
        int         button = 1;
        bool        isCtrlDown = false;
        float       scroll = 0.f;
    };

    static const uintptr_t KEY_END = 0x23;
    static const uintptr_t KEY_HOME = 0x24;
    static const uintptr_t KEY_LEFT = 0x25;
    static const uintptr_t KEY_UP = 0x26;
    static const uintptr_t KEY_RIGHT = 0x27;
    static const uintptr_t KEY_DOWN = 0x28;
    static const uintptr_t KEY_DELETE = 0x2E;

    class UIKeyEvent
    {
    public:
        uintptr_t   key = 0;
        UIContext*  pContext = nullptr;
    };

    class UICheckEvent
    {
    public:
        bool        isChecked = false;
        UIContext*  pContext = nullptr;
    };

    class UIFocusEvent
    {
    public:
        UIContext*  pContext = nullptr;
    };

    class UITreeViewSelectEvent
    {
    public:
        std::vector<UITreeViewItem*>*   pSelectedItems = nullptr;
        UIContext*                      pContext = nullptr;
    };

    class UITreeViewMoveEvent
    {
    public:
        std::vector<UITreeViewItem*>*   pSelectedItems = nullptr;
        UITreeViewItem*                 pTarget = nullptr;
        UIContext*                      pContext = nullptr;
    };

    class UITextBoxEvent
    {
    public:
        UIContext*  pContext = nullptr;
    };

    class UIControl;

    class IUIRenderCallback
    {
    public:
        virtual void render(const void* pControl, const sUIRect& rect) const = 0;
    };

    template<typename TobjType, typename TcallbackType>
    class UIRenderCallback : public IUIRenderCallback
    {
    public:
        UIRenderCallback(const TcallbackType& callback) : m_callback(callback) {}

        void render(const void* pControl, const sUIRect& rect) const
        {
            m_callback(static_cast<const TobjType*>(pControl), rect);
        }

    private:
        TcallbackType m_callback;
    };

    class IUITextCaretCallback
    {
    public:
        virtual decltype(std::string().size()) getCaretPos(const void* pControl, const sUIVector2& localPos) const = 0;
    };

    template<typename TobjType, typename TcallbackType>
    class UITextCaretCallback : public IUITextCaretCallback
    {
    public:
        UITextCaretCallback(const TcallbackType& callback) : m_callback(callback) {}

        decltype(std::string().size()) getCaretPos(const void* pControl, const sUIVector2& localPos) const
        {
            return m_callback(static_cast<const TobjType*>(pControl), localPos);
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

        template<typename TobjType, typename TcallbackType>
        void addTextCaretSolver(const char* szStyle, const TcallbackType& callback)
        {
            auto styleId = uiHash(szStyle);
            auto& m = m_textCaretSolvers[std::type_index(typeid(TobjType))];
            auto& it = m.find(styleId);
            if (it != m.end())
            {
                delete it->second;
            }
            m[styleId] = new UITextCaretCallback<TobjType, TcallbackType>(callback);
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

        template<typename TobjType>
        const IUITextCaretCallback* getTextCaretSolver(unsigned int styleId) const
        {
            auto& mIt = m_textCaretSolvers.find(std::type_index(typeid(TobjType)));
            if (mIt == m_textCaretSolvers.end())
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

        void write(char c);
        void keyDown(uintptr_t key);
        void focus(UIControl* pFocus);
        void clearState();

        UIControl* getHoverControl() const { return m_pHoverControl; }
        UIControl* getDownControl(int mouseButton = 1) const { return m_pDownControls[mouseButton - 1]; }
        UIControl* getFocusControl() const { return m_pFocus; }

        std::function<sUIVector2(const onut::UITextBox* pTextBox, const std::string &text)> textSize = nullptr;
        std::function<void(const onut::sUIRect& rect)> drawInsert = nullptr;

        std::function<void(UIControl*, const sUIRect&, const sUIColor&)> drawRect = 
            [](UIControl*, const sUIRect&, const sUIColor&){};
        std::function<void(UIControl*, const sUIRect&, const sUIImageComponent&)> drawTexturedRect =
            [](UIControl*, const sUIRect&, const sUIImageComponent&){};
        std::function<void(UIControl*, const sUIRect&, const sUIScale9Component&)> drawScale9Rect =
            [](UIControl*, const sUIRect&, const sUIScale9Component&){};
        std::function<void(UIControl*, const sUIRect&, const sUITextComponent&)> drawText = 
            [](UIControl*, const sUIRect&, const sUITextComponent&){};

        std::function<void(bool, const sUIRect&)> onClipping =
            [](bool enableClipping, const sUIRect& rect){};

        std::chrono::steady_clock::duration doubleClickTime = std::chrono::milliseconds(500);

        bool useNavigation = false;

    private:
        void resolve();
        void dispatchEvents();
        void reset();
        void pushClip(const sUIRect& rect);
        void popClip();

        std::unordered_map<std::type_index, std::unordered_map<unsigned int, IUIRenderCallback*>>       m_callbacks;
        std::unordered_map<std::type_index, std::unordered_map<unsigned int, IUITextCaretCallback*>>    m_textCaretSolvers;

        UIMouseEvent    m_mouseEvents[3];
        UIMouseEvent    m_lastMouseEvents[3];
        sUIVector2      m_hoverLocalMousePos;
        sUIVector2      m_screenSize;

        UIControl*      m_pHoverControl = nullptr;
        UIControl*      m_pDownControls[3];
        UIControl*      m_pFocus = nullptr;

        UIControl*      m_pLastHoverControl = nullptr;
        UIControl*      m_pLastDownControls[3];
        UIControl*      m_pLastFocus = nullptr;

        std::vector<char>       m_writes;
        std::vector<uintptr_t>  m_keyDowns;
        std::vector<sUIRect>    m_clips;

        std::chrono::steady_clock::time_point m_clickTimes[3];
        sUIVector2              m_clicksPos[3];
    };

    enum class eUIPropertyType : uint8_t
    {
        P_INT,
        P_FLOAT,
        P_STRING,
        P_BOOL
    };

    class UIProperty
    {
    public:
        UIProperty();
        UIProperty(const rapidjson::Value& jsonNode);
        UIProperty(const UIProperty& other);
        UIProperty& operator=(const UIProperty& other);
        ~UIProperty();

        eUIPropertyType getType() const { return m_type; };

        const char* getString() const;
        int         getInt() const;
        float       getFloat() const;
        bool        getBool() const;

    private:
        eUIPropertyType m_type;
        union
        {
            char*   m_szString;
            int     m_int;
            float   m_float;
            bool    m_bool;
        };
    };

    struct sVoidEvent
    {
    };

    class UIControl
    {
    public:
        UIControl();
        UIControl(const char* szFilename);
        UIControl(const UIControl& other);
        virtual ~UIControl();
        UIControl* copy() const;

        // Public properties
        bool            isEnabled = true; /*! Disabled control and their children won't receive mouse events */
        bool            isClickThrough = false; /*! Same as isEnabled, but will traverse children */
        bool            isVisible = true; /*! Visible or not. Invisible controls don't receive mouse events */
        bool            clipChildren = false; /*! Will trigger a scissor on the children. Usefull for lists */
        sUIRect         rect; /*! Local rectangle. Greatly influenced by align, anchor, pos and dim types. \see getWorldRect */
        eUIAlign        align = eUIAlign::TOP_LEFT; /*! Alignement inside parent control */
        eUIPosType      xType = eUIPosType::POS_RELATIVE; /*! x position type */
        eUIPosType      yType = eUIPosType::POS_RELATIVE; /*! y position type */
        eUIDimType      widthType = eUIDimType::DIM_ABSOLUTE; /*! width type */
        eUIDimType      heightType = eUIDimType::DIM_ABSOLUTE; /*! height type */
        eUIAnchorType   xAnchorType = eUIAnchorType::ANCHOR_PERCENTAGE; /*! x anchor type */
        eUIAnchorType   yAnchorType = eUIAnchorType::ANCHOR_PERCENTAGE; /*! y anchor type */
        sUIVector2      anchor; /*! Anchor position */
        std::string     name; /*! This control's name. Can be used to search for it */
        void*           pUserData = nullptr; /*! Set whatever data you want on this. it will never be accessed or freed by onut::UI */

        void save(const std::string& filename) const;

        virtual eUIType getType() const { return eUIType::UI_CONTROL; }
        virtual bool isNavigatable() const { return false; }

        // Child methods
        void add(UIControl* pChild);
        void insert(UIControl* pChild, UIControl* pBefore);
        void insertAfter(UIControl* pChild, UIControl* pAfter);
        bool insertAt(UIControl* pChild, decltype(std::vector<UIControl*>().size()) index);
        void remove();
        void remove(UIControl* pChild);
        void removeAll();
        UIControl* getChild(const std::string& name, bool bSearchSubChildren = true) const;
        template<typename TuiType>
        TuiType* getChild(const std::string& name, bool bSearchSubChildren = true) const
        {
            return dynamic_cast<TuiType*>(getChild(name, bSearchSubChildren));
        }
        UIControl* getChild(const UIContext& context, const sUIVector2& mousePos, bool bSearchSubChildren = true, bool bIgnoreClickThrough = true) const;
        template<typename TuiType>
        TuiType* getChild(const UIContext& context, const sUIVector2& mousePos, bool bSearchSubChildren = true, bool bIgnoreClickThrough = true) const
        {
            return dynamic_cast<TuiType*>(getChild(context, mousePos, bSearchSubChildren, bIgnoreClickThrough));
        }

        void setStyle(const char* szStyle);
        int getStyle() const { return m_style; }
        const std::string& getStyleName() const { return m_styleName; }

        void retain();
        void release();
        int32_t getRefCount() const { return m_refCount; }

        void update(UIContext& context, const sUIVector2& mousePos, bool bMouse1Down, bool bMouse2Down = false, bool bMouse3Down = false, bool bNavL = false, bool bNavR = false, bool bNavU = false, bool bNavD = false, bool bControl = false, float scroll = 0.f);
        void render(UIContext& context);

        sUIRect getWorldRect(const UIContext& context) const;
        void setWorldRect(const sUIRect& rect, const UIContext& context);

        sUIVector2 getAnchorInPixel() const;
        sUIVector2 getAnchorInPercentage() const;
        void setAnchorPercent(const sUIVector2& anchor);

        eUIState getState(const UIContext& context) const;
        bool hasFocus(const UIContext& context) const;

        UIControl* getParent() const { return m_pParent; }

        const UIProperty&   getProperty(const std::string& name) const;

        const std::vector<UIControl*>& getChildren() const { return m_children; };

        // Event callbacks
        using TfnMouseEvent = std::function<void(UIControl*, const UIMouseEvent&)>;
        TfnMouseEvent onClick;
        TfnMouseEvent onDoubleClick;
        TfnMouseEvent onMouseMove;
        TfnMouseEvent onMouseDown;
        TfnMouseEvent onMouseUp;
        TfnMouseEvent onMouseEnter;
        TfnMouseEvent onMouseLeave;

        TfnMouseEvent onRightClick;
        TfnMouseEvent onRightDoubleClick;
        TfnMouseEvent onRightMouseDown;
        TfnMouseEvent onRightMouseUp;

        TfnMouseEvent onMiddleClick;
        TfnMouseEvent onMiddleDoubleClick;
        TfnMouseEvent onMiddleMouseDown;
        TfnMouseEvent onMiddleMouseUp;

        using TfnFocusEvent = std::function<void(UIControl*)>;
        TfnFocusEvent onGainFocus;
        TfnFocusEvent onLoseFocus;

        using TfnKeyEvent = std::function<void(UIControl*, const UIKeyEvent&)>;
        TfnKeyEvent onKeyDown;

        bool visit(const std::function<bool(UIControl*, const sUIRect&)>& callback, const sUIRect& parentRect);
        bool visitChildrenFirst(const std::function<bool(UIControl*, const sUIRect&)>& callback, const sUIRect& parentRect);
        bool visitEnabled(const std::function<bool(UIControl*, const sUIRect&)>& callback, const sUIRect& parentRect);
        bool visitChildrenFirstEnabled(const std::function<bool(UIControl*, const sUIRect&)>& callback, const sUIRect& parentRect);
        bool visitVisible(const std::function<bool(UIControl*, const sUIRect&)>& callback, const sUIRect& parentRect);
        bool visitChildrenFirstVisible(const std::function<bool(UIControl*, const sUIRect&)>& callback, const sUIRect& parentRect);

    protected:
        friend UIContext;

        virtual void load(const rapidjson::Value& jsonNode);
        virtual void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const;

        void updateInternal(UIContext& context, const sUIRect& parentRect);
        void renderInternal(UIContext& context, const sUIRect& parentRect);
        virtual void renderControl(const UIContext& context, const sUIRect& rect) {}

        virtual void onClickInternal(const UIMouseEvent& evt) {}
        virtual void onMouseDownInternal(const UIMouseEvent& evt) {}
        virtual void onMouseMoveInternal(const UIMouseEvent& evt) {}
        virtual void onMouseScrollInternal(const UIMouseEvent& evt) {}
        virtual void onMouseUpInternal(const UIMouseEvent& evt) {}
        virtual void onGainFocusInternal(const UIFocusEvent& evt) {}
        virtual void onLoseFocusInternal(const UIFocusEvent& evt) {}
        virtual void onWriteInternal(char c, UIContext& context) {}
        virtual void onKeyDownInternal(const UIKeyEvent& evt) {}

    private:
        sUIRect getWorldRect(const sUIRect& parentRect) const;
        void getChild(const UIContext& context, 
                      const sUIVector2& mousePos, 
                      bool bSearchSubChildren, 
                      bool bIgnoreClickThrough,
                      const sUIRect& parentRect, 
                      const UIControl** ppHoverControl) const;

        std::vector<UIControl*>                     m_children;
        int                                         m_style = 0;
        std::string                                 m_styleName;
        int32_t                                     m_refCount = 0;
        UIControl*                                  m_pParent = nullptr;
        std::unordered_map<std::string, UIProperty> m_properties;
    };

    class UIButton : public UIControl
    {
        friend UIControl;

    public:
        UIButton();
        UIButton(const UIButton& other);

        virtual eUIType getType() const override { return eUIType::UI_BUTTON; }
        bool isNavigatable() const override { return true; }

        sUIScale9Component  scale9Component;
        sUITextComponent    textComponent;

    protected:
        virtual void load(const rapidjson::Value& jsonNode) override;
        virtual void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) override;
    };

    class UIPanel : public UIControl
    {
        friend UIControl;

    public:
        UIPanel() {}
        UIPanel(const UIPanel& other);

        virtual eUIType getType() const override { return eUIType::UI_PANEL; }

        sUIColor color;

    protected:
        virtual void load(const rapidjson::Value& jsonNode) override;
        virtual void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) override;
    };

    class UILabel : public UIControl
    {
        friend UIControl;

    public:
        UILabel();
        UILabel(const UILabel& other);

        virtual eUIType getType() const override { return eUIType::UI_LABEL; }

        sUITextComponent textComponent;

    protected:
        virtual void load(const rapidjson::Value& jsonNode) override;
        virtual void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) override;
    };

    class UIImage : public UIControl
    {
        friend UIControl;

    public:
        UIImage() {}
        UIImage(const UIImage& other);

        virtual eUIType getType() const override { return eUIType::UI_IMAGE; }

        sUIScale9Component scale9Component;

    protected:
        virtual void load(const rapidjson::Value& jsonNode) override;
        virtual void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) override;
    };

    class UICheckBox : public UIControl
    {
        friend UIControl;

    public:
        UICheckBox() {}
        UICheckBox(const UICheckBox& other);

        virtual eUIType getType() const override { return eUIType::UI_CHECKBOX; }

        sUIIconComponent iconComponent;
        sUITextComponent textComponent;
        eUICheckBehavior behavior = eUICheckBehavior::NORMAL;

        bool getIsChecked() const { return m_isChecked; }
        void setIsChecked(bool in_isChecked);
        bool isNavigatable() const override { return true; }

        std::function<void(UICheckBox*, const UICheckEvent&)> onCheckChanged;

    protected:
        virtual void load(const rapidjson::Value& jsonNode) override;
        virtual void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) override;
        virtual void onClickInternal(const UIMouseEvent& evt) override;

    private:
        bool                m_isChecked = false;
    };

    class UITreeViewItem;

    class UITreeView : public UIControl
    {
        friend UIControl;

    public:
        UITreeView() { }
        UITreeView(const UITreeView& other);
        virtual ~UITreeView();

        virtual eUIType getType() const override { return eUIType::UI_TREEVIEW; }

        sUIFont font;
        float expandedXOffset = 16.f;
        float expandClickWidth = 18.f;
        float itemHeight = 18.f;
        bool allowReorder = false;

        void addItem(UITreeViewItem* pItem);
        void addItemBefore(UITreeViewItem* pItem, UITreeViewItem* pBefore);
        void addItemAfter(UITreeViewItem* pItem, UITreeViewItem* pAfter);
        void removeItem(UITreeViewItem* pItem);
        void clear();

        void unselectAll();
        void unselectItem(UITreeViewItem* pItem);
        void addSelectedItem(UITreeViewItem* pItem);

        void expandTo(UITreeViewItem* pItem);

        std::function<void(UITreeView*, const UITreeViewSelectEvent&)> onSelectionChanged;
        std::function<void(UITreeView*, const UITreeViewMoveEvent&)> onMoveItemInto;
        std::function<void(UITreeView*, const UITreeViewMoveEvent&)> onMoveItemBefore;
        std::function<void(UITreeView*, const UITreeViewMoveEvent&)> onMoveItemAfter;

    protected:
        virtual void load(const rapidjson::Value& jsonNode) override;
        virtual void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) override;
        virtual void onMouseDownInternal(const UIMouseEvent& evt) override;
        virtual void onMouseMoveInternal(const UIMouseEvent& evt) override;
        virtual void onMouseUpInternal(const UIMouseEvent& evt) override;
        virtual void onMouseScrollInternal(const UIMouseEvent& evt) override;

    private:
        UITreeViewItem* getItemAtPosition(const sUIVector2& pos, const sUIRect& rect, bool* pPickedExpandButton = nullptr, sUIRect* pItemRect = nullptr) const;
        UITreeViewItem* getItemAtPosition(UITreeViewItem* pItem, const sUIVector2& pos, sUIRect& rect, bool* pPickedExpandButton = nullptr, sUIRect* pItemRect = nullptr) const;
        float getTotalHeight(UITreeViewItem* pItem = nullptr) const;

        std::vector<UITreeViewItem*>    m_items;
        std::vector<UITreeViewItem*>    m_selectedItems;
        float                           m_scroll = 0.f;
        bool                            m_isDragging = false;
        sUIVector2                      m_mousePosOnDragStart;
        sUIVector2                      m_dragMousePos;
        UITreeViewItem*                 m_dragHoverItem = nullptr;
        UITreeViewItem*                 m_dragBeforeItem = nullptr;
        UITreeViewItem*                 m_dragAfterItem = nullptr;
        sUIRect                         m_dragInBetweenRect;
    };

    class UITreeViewItem
    {
        friend UITreeView;

    public:
        UITreeViewItem() {}
        UITreeViewItem(const std::string& text);
        UITreeViewItem(const UITreeViewItem& other);

        bool        isExpanded = false;
        std::string text;
        void*       pUserData = nullptr;
        std::shared_ptr<void> pSharedUserData = nullptr;
        float       opacity = 1.f;

        UITreeViewItem* getParent() const { return m_pParent; }

        void addItem(UITreeViewItem* pItem);
        void addItemBefore(UITreeViewItem* pItem, UITreeViewItem* pBefore);
        void addItemAfter(UITreeViewItem* pItem, UITreeViewItem* pAfter);
        void removeItem(UITreeViewItem* pItem);

        UITreeView* getTreeView() const { return m_pTreeView; }
        void setTreeView(UITreeView* pTreeView);

        const std::vector<UITreeViewItem*>& getItems() const { return m_items; }

        bool getIsSelected() const { return m_isSelected; }

        void retain();
        void release();
        int32_t getRefCount() const { return m_refCount; }

    private:
        template<typename TfnCallback>
        void render(const TfnCallback& itemCallback, const UITreeView* pTreeView, const sUIRect& treeViewRect, sUIRect& rect) const
        {
            itemCallback->render(this, rect);
            rect.position.y += pTreeView->itemHeight;
            if (isExpanded)
            {
                if (!m_items.empty())
                {
                    auto xOffset = pTreeView->expandedXOffset;
                    rect.position.x += xOffset;
                    rect.size.x -= xOffset;
                    for (auto pItem : m_items)
                    {
                        pItem->render(itemCallback, pTreeView, treeViewRect, rect);
                    }
                    rect.size.x += xOffset;
                    rect.position.x -= xOffset;
                }
            }
        }

        template<typename TfnCallback>
        void renderDrag(const TfnCallback& itemCallback, const UITreeView* pTreeView, const sUIRect& treeViewRect, sUIRect& rect)
        {
            auto isSelected = m_isSelected;
            m_isSelected = false;
            opacity = .5f;
            itemCallback->render(this, rect);
            opacity = 1.f;
            m_isSelected = isSelected;
        }

        std::vector<UITreeViewItem*>    m_items;
        UITreeView*                     m_pTreeView = nullptr;
        bool                            m_isSelected = false;
        UITreeViewItem*                 m_pParent = nullptr;
        int32_t                         m_refCount = 0;
    };

    class UITextBox : public UIControl
    {
        friend UIControl;

    public:
        UITextBox() {}
        UITextBox(const UITextBox& other);

        virtual eUIType getType() const override { return eUIType::UI_TEXTBOX; }

        sUITextComponent textComponent;
        sUIScale9Component scale9Component;
        float min = -std::numeric_limits<float>::max();
        float max = std::numeric_limits<float>::max();
        float step = 1.f;

        const std::string::size_type* getSelectedTextRegion() const { return m_selectedTextRegion; }
        std::string::size_type getCursorPos() const { return m_cursorPos; }
        bool isCursorVisible() const;
        bool isNavigatable() const override { return true; }

        std::function<void(UITextBox*, const UITextBoxEvent&)> onTextChanged;
        std::function<void(UITextBox*, const UITextBoxEvent&)> onNumberSpinStart;
        std::function<void(UITextBox*, const UITextBoxEvent&)> onNumberSpinEnd;

        void selectAll();

        bool getIsNumerical() const { return m_isNumerical; }
        void setIsNumerical(bool isNumerical);
        int getDecimalPrecision() const { return m_decimalPrecision; }
        void setIsDecimalPrecision(int decimalPrecision);

        float getFloat() const;
        int getInt() const;
        void setFloat(float f);
        void setInt(int i);

    protected:
        virtual void load(const rapidjson::Value& jsonNode) override;
        virtual void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) override;

        virtual void onGainFocusInternal(const UIFocusEvent& evt) override;
        virtual void onLoseFocusInternal(const UIFocusEvent& evt) override;
        virtual void onMouseDownInternal(const UIMouseEvent& evt) override;
        virtual void onMouseMoveInternal(const UIMouseEvent& evt) override;
        virtual void onMouseUpInternal(const UIMouseEvent& evt) override;
        virtual void onWriteInternal(char c, UIContext& context) override;
        virtual void onKeyDownInternal(const UIKeyEvent& evt) override;

    private:
        void numerifyText();

        std::string::size_type                  m_selectedTextRegion[2];
        bool                                    m_isSelecting = false;
        std::string::size_type                  m_cursorPos = 0;
        std::chrono::steady_clock::time_point   m_cursorTime;
        bool                                    m_isTextChanged = false;
        bool                                    m_isNumerical = false;
        int                                     m_decimalPrecision = 0;
        float                                   m_mousePosOnDown;
        float                                   m_valueOnDown;
        bool                                    m_isSpinning = false;
    };
}
