#pragma once
#include <chrono>
#include <cinttypes>
#include <functional>
#include <string>
#include <typeindex>
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
        OPTIONAL
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
        float right;
        float top;
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
        sUIPadding          padding;
        sUIScale9Component() : isScaled9(false) {}
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

    class UIMouseEvent
    {
    public:
        sUIVector2  mousePos;
        sUIVector2  localMousePos;
        bool        isMouseDown = false;
        UIContext*  pContext;
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

        UIControl* getHoverControl() const { return m_pHoverControl; }
        UIControl* getDownControl() const { return m_pDownControl; }
        UIControl* getFocusControl() const { return m_pFocus; }

        std::function<void(UIControl*, const sUIRect&, const sUIColor&)> drawRect;
        std::function<void(UIControl*, const sUIRect&, const sUIImageComponent&)> drawTexturedRect;
        std::function<void(UIControl*, const sUIRect&, const sUIScale9Component&)> drawScale9Rect;
        std::function<void(UIControl*, const sUIRect&, const sUITextComponent&)> drawText;

    private:
        void resolve();
        void dispatchEvents();
        void reset();

        std::unordered_map<std::type_index, std::unordered_map<unsigned int, IUIRenderCallback*>>       m_callbacks;
        std::unordered_map<std::type_index, std::unordered_map<unsigned int, IUITextCaretCallback*>>    m_textCaretSolvers;

        UIMouseEvent    m_mouseEvent;
        UIMouseEvent    m_lastMouseEvent;
        sUIVector2      m_hoverLocalMousePos;
        sUIVector2      m_screenSize;

        UIControl*      m_pHoverControl = nullptr;
        UIControl*      m_pDownControl = nullptr;
        UIControl*      m_pFocus = nullptr;

        UIControl*      m_pLastHoverControl = nullptr;
        UIControl*      m_pLastDownControl = nullptr;
        UIControl*      m_pLastFocus = nullptr;

        std::vector<char>       m_writes;
        std::vector<uintptr_t>  m_keyDowns;
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

    class UIControl
    {
    public:
        UIControl();
        UIControl(const char* szFilename);
        UIControl(const UIControl& other);
        virtual ~UIControl();

        void* pUserData = nullptr;

        void save(const std::string& filename) const;

        virtual eUIType getType() const { return eUIType::UI_CONTROL; }

        void add(UIControl* pChild);
        void remove(UIControl* pChild);
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

        void update(UIContext& context, const sUIVector2& mousePos, bool bMouseDown);
        void render(const UIContext& context) const;

        const sUIRect& getRect() const { return m_rect; }
        void setRect(const sUIRect& rect);
        sUIRect getWorldRect(const UIContext& context) const;
        void setWorldRect(const sUIRect& rect, const UIContext& context);

        const sUIVector2& getAnchor() const { return m_anchor; }
        sUIVector2 getAnchorInPixel() const;
        sUIVector2 getAnchorInPercentage() const;
        void setAnchor(const sUIVector2& anchor);
        void setAnchorPercent(const sUIVector2& anchor);

        const std::string& getName() const { return m_name; }
        void setName(const std::string& name);

        bool isEnabled() const { return m_isEnabled && m_isVisible; }
        void setIsEnabled(bool bIsEnabled);

        bool isVisible() const { return m_isVisible; }
        void setIsVisible(bool bIsVisible);

        bool isClickThrough() const { return m_isClickThrough; }
        void setIsClickThrough(bool bIsClickThrough);

        eUIState getState(const UIContext& context) const;
        bool hasFocus(const UIContext& context) const;

        UIControl* getParent() const { return m_pParent; }

        using TfnMouseEvent = std::function < void(UIControl*, const UIMouseEvent&) > ;
        TfnMouseEvent onClick;
        TfnMouseEvent onMouseMove;
        TfnMouseEvent onMouseDown;
        TfnMouseEvent onMouseUp;
        TfnMouseEvent onMouseEnter;
        TfnMouseEvent onMouseLeave;

        using TfnFocusEvent = std::function < void(UIControl*) > ;
        TfnFocusEvent onGainFocus;
        TfnFocusEvent onLoseFocus;

        using TfnKeyEvent = std::function < void(UIControl*, const UIKeyEvent&) >;
        TfnKeyEvent onKeyDown;

        const UIProperty&   getProperty(const std::string& name) const;

        eUIAlign getAlign() const { return m_align; }
        void setAlign(eUIAlign align);

        eUIDimType getWidthType() const { return m_dimType[0]; }
        eUIDimType getHeightType() const { return m_dimType[1]; }
        void setWidthType(eUIDimType widthType);
        void setHeightType(eUIDimType heightType);

        eUIPosType getXType() const { return m_posType[0]; }
        eUIPosType getYType() const { return m_posType[1]; }
        void setXType(eUIPosType xType);
        void setYType(eUIPosType yType);

        eUIAnchorType getXAnchorType() const { return m_anchorType[0]; }
        eUIAnchorType getYAnchorType() const { return m_anchorType[1]; }
        void setXAnchorType(eUIAnchorType xAnchorType);
        void setYAnchorType(eUIAnchorType yAnchorType);

        const std::vector<UIControl*>& getChildren() const { return m_children; };

    protected:
        friend UIContext;

        virtual void load(const rapidjson::Value& jsonNode);
        virtual void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const;

        void updateInternal(UIContext& context, const sUIRect& parentRect);
        void renderInternal(const UIContext& context, const sUIRect& parentRect) const;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) const {}

        virtual void onClickInternal(const UIMouseEvent& evt) {}
        virtual void onMouseDownInternal(const UIMouseEvent& evt) {}
        virtual void onMouseMoveInternal(const UIMouseEvent& evt) {}
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

        bool                    m_isEnabled = true;
        bool                    m_isClickThrough = false;
        bool                    m_isVisible = true;
        sUIRect                 m_rect;
        sUIVector2              m_anchor;
        std::vector<UIControl*> m_children;
        eUIAlign                m_align = eUIAlign::TOP_LEFT;
        eUIPosType              m_posType[2];
        eUIDimType              m_dimType[2];
        eUIAnchorType           m_anchorType[2];
        int                     m_style = 0;
        std::string             m_name;
        std::string             m_styleName;
        int32_t                 m_refCount = 0;
        UIControl*              m_pParent = nullptr;
        std::unordered_map<std::string, UIProperty> m_properties;
    };

    class UIButton : public UIControl
    {
        friend UIControl;

    public:
        virtual eUIType getType() const override { return eUIType::UI_BUTTON; }

        sUIScale9Component  scale9Component;
        sUITextComponent    textComponent;

    protected:
        virtual void load(const rapidjson::Value& jsonNode) override;
        virtual void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) const override;
    };

    class UIPanel : public UIControl
    {
        friend UIControl;

    public:
        virtual eUIType getType() const override { return eUIType::UI_PANEL; }

        sUIColor color;

    protected:
        virtual void load(const rapidjson::Value& jsonNode) override;
        virtual void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) const override;
    };

    class UILabel : public UIControl
    {
        friend UIControl;

    public:
        virtual eUIType getType() const override { return eUIType::UI_LABEL; }

        sUITextComponent textComponent;

    protected:
        virtual void load(const rapidjson::Value& jsonNode) override;
        virtual void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) const override;
    };

    class UIImage : public UIControl
    {
        friend UIControl;

    public:
        virtual eUIType getType() const override { return eUIType::UI_IMAGE; }

        sUIScale9Component scale9Component;

    protected:
        virtual void load(const rapidjson::Value& jsonNode) override;
        virtual void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) const override;
    };

    class UICheckBox : public UIControl
    {
        friend UIControl;

    public:
        virtual eUIType getType() const override { return eUIType::UI_CHECKBOX; }

        sUIIconComponent iconComponent;
        sUITextComponent textComponent;
        eUICheckBehavior behavior = eUICheckBehavior::NORMAL;

        bool getIsChecked() const { return m_isChecked; }
        void setIsChecked(bool in_isChecked);

        std::function<void(UICheckBox*, const UICheckEvent&)> onCheckChanged;

    protected:
        virtual void load(const rapidjson::Value& jsonNode) override;
        virtual void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) const override;
        virtual void onClickInternal(const UIMouseEvent& evt) override;

    private:
        bool                m_isChecked = false;
    };

    class UITreeViewItem;

    class UITreeView : public UIControl
    {
        friend UIControl;

    public:
        UITreeView() {}
        UITreeView(const UITreeView& other);
        virtual ~UITreeView();

        virtual eUIType getType() const override { return eUIType::UI_TREEVIEW; }

        sUIFont font;
        float expandedXOffset = 16.f;
        float expandClickWidth = 18.f;
        float itemHeight = 18.f;

        void addItem(UITreeViewItem* pItem);
        void removeItem(UITreeViewItem* pItem);
        void clear();

        void unselectAll();
        void unselectItem(UITreeViewItem* pItem);
        void addSelectedItem(UITreeViewItem* pItem);

        void expandTo(UITreeViewItem* pItem);

        std::function<void(UITreeView*, const UITreeViewSelectEvent&)> onSelectionChanged;

    protected:
        virtual void load(const rapidjson::Value& jsonNode) override;
        virtual void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const;
        virtual void renderControl(const UIContext& context, const sUIRect& rect) const override;
        virtual void onMouseDownInternal(const UIMouseEvent& evt) override;

    private:
        UITreeViewItem* getItemAtPosition(const sUIVector2& pos, const sUIRect& rect, bool* pPickedExpandButton = nullptr) const;
        UITreeViewItem* getItemAtPosition(UITreeViewItem* pItem, const sUIVector2& pos, sUIRect& rect, bool* pPickedExpandButton = nullptr) const;

        std::vector<UITreeViewItem*>    m_items;
        std::vector<UITreeViewItem*>    m_selectedItems;
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

        UITreeViewItem* getParent() const { return m_pParent; }

        void addItem(UITreeViewItem* pItem);
        void removeItem(UITreeViewItem* pItem);

        UITreeView* getTreeView() const { return m_pTreeView; }

        const std::vector<UITreeViewItem*>& getItems() const { return m_items; }

        bool getIsSelected() const { return m_isSelected; }

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

        std::vector<UITreeViewItem*>    m_items;
        UITreeView*                     m_pTreeView = nullptr;
        bool                            m_isSelected = false;
        UITreeViewItem*                 m_pParent = nullptr;
    };

    class UITextBox : public UIControl
    {
        friend UIControl;

    public:
        virtual eUIType getType() const override { return eUIType::UI_TEXTBOX; }

        sUITextComponent textComponent;

        const std::string::size_type* getSelectedTextRegion() const { return m_selectedTextRegion; }
        std::string::size_type getCursorPos() const { return m_cursorPos; }
        bool isCursorVisible() const;

        std::function<void(UITextBox*, const UITextBoxEvent&)> onTextChanged;

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
        virtual void renderControl(const UIContext& context, const sUIRect& rect) const override;

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
    };
}
