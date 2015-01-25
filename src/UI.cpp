#include "UI.h"
#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include <functional>

namespace onut
{
    unsigned int uiHash(const char* s, unsigned int seed)
    {
        unsigned hash = seed;
        while (*s)
        {
            hash = hash * 101 + *s++;
        }
        return hash;
    }

    eUIAlign getAlignFromString(const char* szAlign)
    {
        if (!strcmp(szAlign, "TOP_LEFT"))
        {
            return eUIAlign::TOP_LEFT;
        }
        else if (!strcmp(szAlign, "TOP"))
        {
            return eUIAlign::TOP;
        }
        else if (!strcmp(szAlign, "TOP_RIGHT"))
        {
            return eUIAlign::TOP_RIGHT;
        }
        else if (!strcmp(szAlign, "LEFT"))
        {
            return eUIAlign::LEFT;
        }
        else if (!strcmp(szAlign, "CENTER"))
        {
            return eUIAlign::CENTER;
        }
        else if (!strcmp(szAlign, "RIGHT"))
        {
            return eUIAlign::RIGHT;
        }
        else if (!strcmp(szAlign, "BOTTOM_LEFT"))
        {
            return eUIAlign::BOTTOM_LEFT;
        }
        else if (!strcmp(szAlign, "BOTTOM"))
        {
            return eUIAlign::BOTTOM;
        }
        else if (!strcmp(szAlign, "BOTTOM_RIGHT"))
        {
            return eUIAlign::BOTTOM_RIGHT;
        }
    
        return eUIAlign::TOP_LEFT;
    }

    eUIDimType getDimTypeFromString(const char* szAlign)
    {
        if (!strcmp(szAlign, "ABSOLUTE"))
        {
            return eUIDimType::DIM_ABSOLUTE;
        }
        else if (!strcmp(szAlign, "RELATIVE"))
        {
            return eUIDimType::DIM_RELATIVE;
        }
        else if (!strcmp(szAlign, "PERCENTAGE"))
        {
            return eUIDimType::DIM_PERCENTAGE;
        }

        return eUIDimType::DIM_ABSOLUTE;
    }

    eUIPosType getPosTypeFromString(const char* szAlign)
    {
        if (!strcmp(szAlign, "RELATIVE"))
        {
            return eUIPosType::POS_RELATIVE;
        }
        else if (!strcmp(szAlign, "PERCENTAGE"))
        {
            return eUIPosType::POS_PERCENTAGE;
        }

        return eUIPosType::POS_RELATIVE;
    }

    eUICheckBehavior getJsonCheckBehavior(const char* szAlign)
    {
        if (!strcmp(szAlign, "OPTIONAL"))
        {
            return eUICheckBehavior::OPTIONAL;
        }
        else if (!strcmp(szAlign, "EXCLUSIVE"))
        {
            return eUICheckBehavior::EXCLUSIVE;
        }

        return eUICheckBehavior::NORMAL;
    }

    static const char* getJsonString(const rapidjson::Value& jsonNode, const char* szDefault = "")
    {
        if (jsonNode.IsString())
        {
            return jsonNode.GetString();
        }
        else
        {
            return szDefault;
        }
    }
    
    static float getJsonFloat(const rapidjson::Value& jsonNode, const float default = 0.f)
    {
        if (jsonNode.IsNumber())
        {
            return static_cast<float>(jsonNode.GetDouble());
        }
        else
        {
            return default;
        }
    }

    static bool getJsonBool(const rapidjson::Value& jsonNode, const bool default = false)
    {
        if (jsonNode.IsBool())
        {
            return jsonNode.GetBool();
        }
        else
        {
            return default;
        }
    }

    static sUIColor getJsonColor(const rapidjson::Value& jsonNode, const sUIColor& default = {1, 1, 1, 1, 0xffffffff})
    {
        if (jsonNode.IsString())
        {
            sUIColor ret;
            auto szStr = jsonNode.GetString();
            auto len = jsonNode.GetStringLength();
            if (len != 8)
            {
                return default;
            }
            ret.packed = static_cast<uint32_t>(strtoul(szStr, NULL, 16));
            ret.r = (float)((ret.packed >> 24) & 0xff) / 255.f;
            ret.g = (float)((ret.packed >> 16) & 0xff) / 255.f;
            ret.b = (float)((ret.packed >> 8) & 0xff) / 255.f;
            ret.a = (float)(ret.packed & 0xff) / 255.f;
            return std::move(ret);
        }
        else
        {
            return default;
        }
    }

    UIContext::UIContext(const sUIVector2& screenSize) :
        m_screenSize(screenSize)
    {
    }

    UIContext::~UIContext()
    {
        for (auto& kv : m_callbacks)
        {
            for (auto& kv2 : kv.second)
            {
                delete kv2.second;
            }
        }
    }

    void UIContext::resize(const sUIVector2& screenSize)
    {
        m_screenSize = screenSize;
    }

    UIProperty::UIProperty()
    {
        m_szString = nullptr;
        m_bool = false;
        m_int = 0;
        m_float = 0.f;
        m_type = eUIPropertyType::P_INT;
    }

    UIProperty::UIProperty(const rapidjson::Value& jsonNode)
    {
        m_szString = nullptr;
        m_bool = false;
        m_int = 0;
        m_float = 0.f;
        if (jsonNode.IsNumber())
        {
            if (jsonNode.IsDouble())
            {
                m_type = eUIPropertyType::P_FLOAT;
                m_float = static_cast<float>(jsonNode.GetDouble());
            }
            else if (jsonNode.IsInt())
            {
                m_type = eUIPropertyType::P_INT;
                m_int = jsonNode.GetInt();
            }
            else
            {
                assert(false);
            }
        }
        else if (jsonNode.IsBool())
        {
            m_type = eUIPropertyType::P_BOOL;
            m_bool = jsonNode.GetBool();
        }
        else if (jsonNode.IsString())
        {
            m_type = eUIPropertyType::P_STRING;
            auto len = jsonNode.GetStringLength();
            auto szString = jsonNode.GetString();
            m_szString = new char[len + 1];
            memcpy(m_szString, szString, len + 1);
        }
        else
        {
            assert(false);
        }
    }

    UIProperty::UIProperty(const UIProperty& other)
    {
        m_szString = nullptr;
        m_bool = false;
        m_int = 0;
        m_float = 0.f;
        m_type = other.m_type;
        switch (m_type)
        {
            case eUIPropertyType::P_BOOL:
                m_bool = other.m_bool;
                break;
            case eUIPropertyType::P_INT:
                m_int = other.m_int;
                break;
            case eUIPropertyType::P_FLOAT:
                m_float = other.m_float;
                break;
            case eUIPropertyType::P_STRING:
            {
                auto len = strlen(other.m_szString);
                m_szString = new char[len + 1];
                memcpy(m_szString, other.m_szString, len + 1);
                break;
            }
        }
    }

    UIProperty& UIProperty::operator=(const UIProperty& other)
    {
        if (m_type == eUIPropertyType::P_STRING)
        {
            delete[] m_szString;
        }
        m_szString = nullptr;
        m_bool = false;
        m_int = 0;
        m_float = 0.f;
        m_type = other.m_type;
        switch (m_type)
        {
            case eUIPropertyType::P_BOOL:
                m_bool = other.m_bool;
                break;
            case eUIPropertyType::P_INT:
                m_int = other.m_int;
                break;
            case eUIPropertyType::P_FLOAT:
                m_float = other.m_float;
                break;
            case eUIPropertyType::P_STRING:
            {
                auto len = strlen(other.m_szString);
                m_szString = new char[len + 1];
                memcpy(m_szString, other.m_szString, len + 1);
                break;
            }
        }
        return *this;
    }

    UIProperty::~UIProperty()
    {
        if (m_type == eUIPropertyType::P_STRING)
        {
            delete[] m_szString;
        }
    }

    const char* UIProperty::getString() const
    {
        assert(m_type == eUIPropertyType::P_STRING);
        return m_szString;
    }

    int UIProperty::getInt() const
    {
        assert(m_type == eUIPropertyType::P_INT);
        return m_int;
    }

    float UIProperty::getFloat() const
    {
        assert(m_type == eUIPropertyType::P_FLOAT || m_type == eUIPropertyType::P_INT);
        if (m_type == eUIPropertyType::P_FLOAT)
        {
            return m_float;
        }
        else
        {
            return static_cast<float>(m_int);
        }
    }

    bool UIProperty::getBool() const
    {
        assert(m_type == eUIPropertyType::P_BOOL || m_type == eUIPropertyType::P_INT);
        if (m_type == eUIPropertyType::P_BOOL)
        {
            return m_bool;
        }
        else
        {
            return m_int ? true : false;
        }
    }

    UIControl::UIControl()
    {
        m_posType[0] = eUIPosType::POS_RELATIVE;
        m_posType[1] = eUIPosType::POS_RELATIVE;
        m_dimType[0] = eUIDimType::DIM_ABSOLUTE;
        m_dimType[1] = eUIDimType::DIM_ABSOLUTE;
    }

    UIControl::UIControl(const char* szFilename)
    {
        // Open json file
        FILE* pFile = nullptr;
        auto fopenRet = fopen_s(&pFile, szFilename, "r");
        assert(!fopenRet);
        rapidjson::FileStream is(pFile);
        rapidjson::Document doc;
        doc.ParseStream<0>(is);

        load(doc);

        // Close the file
        fclose(pFile);
    }

    void UIControl::load(const rapidjson::Value& jsonNode)
    {
        m_rect.position.x = getJsonFloat(jsonNode["x"]);
        m_rect.position.y = getJsonFloat(jsonNode["y"]);
        m_rect.size.x = getJsonFloat(jsonNode["width"], 0.f);
        m_rect.size.y = getJsonFloat(jsonNode["height"], 0.f);

        m_align = getAlignFromString(getJsonString(jsonNode["align"]));
        m_posType[0] = getPosTypeFromString(getJsonString(jsonNode["xType"]));
        m_posType[1] = getPosTypeFromString(getJsonString(jsonNode["yType"]));
        m_dimType[0] = getDimTypeFromString(getJsonString(jsonNode["widthType"]));
        m_dimType[1] = getDimTypeFromString(getJsonString(jsonNode["heightType"]));

        m_name = getJsonString(jsonNode["name"]);
        m_styleName = getJsonString(jsonNode["style"]);
        m_style = uiHash(m_styleName.c_str());

        m_isEnabled = getJsonBool(jsonNode["enabled"], true);
        m_isVisible = getJsonBool(jsonNode["visible"], true);
        m_isClickThrough = getJsonBool(jsonNode["clickThrough"], false);

        // Properties
        auto& properties = jsonNode["properties"];
        if (!properties.IsNull())
        {
            for (auto it = properties.MemberBegin(); it != properties.MemberEnd(); ++it)
            {
                m_properties[it->name.GetString()] = UIProperty(it->value);
            }
        }

        // Load children
        auto& jsonChildren = jsonNode["children"];
        if (jsonChildren.IsArray())
        {
            auto size = jsonChildren.Size();
            for (decltype(size) i = 0; i < size; ++i)
            {
                auto& jsonChild = jsonChildren[i];
                auto jsonChildType = getJsonString(jsonChild["type"], "UIControl");
                UIControl* pChild = nullptr;
                if (!strcmp(jsonChildType, "UIControl"))
                {
                    pChild = new UIControl();
                }
                else if (!strcmp(jsonChildType, "UIButton"))
                {
                    pChild = new UIButton();
                }
                else if (!strcmp(jsonChildType, "UIPanel"))
                {
                    pChild = new UIPanel();
                }
                else if (!strcmp(jsonChildType, "UILabel"))
                {
                    pChild = new UILabel();
                }
                else if (!strcmp(jsonChildType, "UIImage"))
                {
                    pChild = new UIImage();
                }
                else if (!strcmp(jsonChildType, "UICheckBox"))
                {
                    pChild = new UICheckBox();
                }
                else if (!strcmp(jsonChildType, "UITreeView"))
                {
                    pChild = new UITreeView();
                }
                if (!pChild) continue;
                add(pChild);
                pChild->load(jsonChild);
            }
        }
    }

    UIControl::UIControl(const UIControl& other)
    {
        m_isEnabled = other.m_isEnabled;
        m_isClickThrough = other.m_isClickThrough;
        m_isVisible = other.m_isVisible;
        m_rect = other.m_rect;
        m_align = other.m_align;
        m_posType[0] = other.m_posType[0];
        m_posType[1] = other.m_posType[1];
        m_dimType[0] = other.m_dimType[0];
        m_dimType[1] = other.m_dimType[1];
        m_name = other.m_name;
        m_pUserData = other.m_pUserData;
        m_properties = other.m_properties;

        for (auto pChild : m_children)
        {
            switch (pChild->getType())
            {
                case eUIType::UI_CONTROL:
                    add(new UIControl(*pChild));
                    break;
                case eUIType::UI_BUTTON:
                    add(new UIButton(*(UIButton*)pChild));
                    break;
                case eUIType::UI_PANEL:
                    add(new UIPanel(*(UIPanel*)pChild));
                    break;
                case eUIType::UI_LABEL:
                    add(new UILabel(*(UILabel*)pChild));
                    break;
                case eUIType::UI_IMAGE:
                    add(new UIImage(*(UIImage*)pChild));
                    break;
                case eUIType::UI_CHECKBOX:
                    add(new UICheckBox(*(UICheckBox*)pChild));
                    break;
                case eUIType::UI_TREEVIEW:
                    add(new UITreeView(*(UITreeView*)pChild));
                    break;
            }
        }
    }

    UIControl::~UIControl()
    {
        for (auto pChild : m_children)
        {
            pChild->release();
        }
    }

    void UIControl::add(UIControl* pChild)
    {
        pChild->retain();
        if (pChild->m_pParent)
        {
            pChild->m_pParent->remove(pChild);
        }

        pChild->m_pParent = this;
        m_children.push_back(pChild);
    }

    void UIControl::remove(UIControl* in_pChild)
    {
        if (in_pChild->m_pParent != this) return;

        auto size = m_children.size();
        for (decltype(size) i = 0; i < size; ++i)
        {
            auto pChild = m_children[i];
            if (pChild == in_pChild)
            {
                m_children.erase(m_children.begin() + i);
                break;
            }
        }

        in_pChild->m_pParent = nullptr;
        in_pChild->release();
    }

    UIControl* UIControl::getChild(const std::string& name, bool bSearchSubChildren) const
    {
        for (auto pChild : m_children)
        {
            if (pChild->m_name == name)
            {
                return pChild;
            }
        }

        if (bSearchSubChildren)
        {
            for (auto pChild : m_children)
            {
                auto pRet = pChild->getChild(name, bSearchSubChildren);
                if (pRet)
                {
                    return pRet;
                }
            }
        }

        return nullptr;
    }

    void UIControl::getChild(const UIContext& context, 
                             const sUIVector2& mousePos, 
                             bool bSearchSubChildren, 
                             const sUIRect& parentRect, 
                             const UIControl** ppHoverControl) const
    {
        if (!isVisible() || isClickThrough()) return;

        sUIRect worldRect = getWorldRect(parentRect);
        auto itend = m_children.rend();
        for (auto it = m_children.rbegin(); it != itend; ++it)
        {
            auto pChild = *it;
            pChild->getChild(context, mousePos, bSearchSubChildren, worldRect, ppHoverControl);
        }

        if (!*ppHoverControl)
        {
            if (mousePos.x >= worldRect.position.x &&
                mousePos.y >= worldRect.position.y &&
                mousePos.x <= worldRect.position.x + worldRect.size.x &&
                mousePos.y <= worldRect.position.y + worldRect.size.y)
            {
                *ppHoverControl = this;
            }
        }
    }

    UIControl* UIControl::getChild(const UIContext& context, const sUIVector2& mousePos, bool bSearchSubChildren) const
    {
        sUIRect parentWorldRect;
        if (getParent())
        {
            parentWorldRect = getParent()->getWorldRect(context);
        }
        else
        {
            parentWorldRect = {{0, 0}, context.getScreenSize()};
        }
        const UIControl* pHoverControl = nullptr;
        getChild(context, mousePos, bSearchSubChildren, parentWorldRect, &pHoverControl);
        if (pHoverControl == this)
        {
            pHoverControl = nullptr;
        }
        return (UIControl*)pHoverControl; // cast from const to not const. Chuuut don't tell anyone
    }

    void UIControl::setStyle(const char* szStyle)
    {
        m_styleName = szStyle;
        m_style = uiHash(szStyle);
    }

    void UIControl::retain()
    {
        ++m_refCount;
    }

    void UIControl::release()
    {
        --m_refCount;
        if (m_refCount <= 0)
        {
            delete this;
        }
    }

    void UIControl::update(UIContext& context, const sUIVector2& mousePos, bool bMouseDown)
    {
        // Prepare our data
        sUIRect parentRect = {{0, 0}, context.getScreenSize()};
        context.m_mouseEvent.mousePos = mousePos;
        context.m_mouseEvent.isMouseDown = bMouseDown;
        context.m_mouseEvent.pContext = &context;
        context.m_pHoverControl = nullptr;

        // Update UIs
        updateInternal(context, parentRect);

        // Resolve
        context.resolve();

        // Send events
        context.dispatchEvents();

        // Reset for next frame
        context.reset();
    }

    void UIContext::resolve()
    {
        // If it's the first mouse down since last frame, and there is an hover
        // control. Put him into down state
        if (!m_lastMouseEvent.isMouseDown && 
            m_mouseEvent.isMouseDown &&
            m_pHoverControl)
        {
            m_pDownControl = m_pHoverControl;
        }

        if (!m_mouseEvent.isMouseDown)
        {
            m_pDownControl = nullptr;
        }

        // Don't allow hovering of other controls if a control is in DOWN state
        if (m_pDownControl && 
            m_pHoverControl != m_pDownControl)
        {
            m_pHoverControl = nullptr;
        }
    }

    void UIContext::dispatchEvents()
    {
        if (m_pHoverControl != m_pLastHoverControl)
        {
            if (m_pLastHoverControl)
            {
                if (m_pLastHoverControl->onMouseLeave)
                {
                    m_pLastHoverControl->onMouseLeave(m_pLastHoverControl, m_mouseEvent);
                }
            }
            if (m_pHoverControl)
            {
                if (m_pHoverControl->onMouseEnter)
                {
                    m_pHoverControl->onMouseEnter(m_pHoverControl, m_mouseEvent);
                }
            }
        }
        if (m_pHoverControl)
        {
            if (m_lastMouseEvent.mousePos.x != m_mouseEvent.mousePos.x ||
                m_lastMouseEvent.mousePos.y != m_mouseEvent.mousePos.y)
            {
                if (m_pHoverControl->onMouseMove)
                {
                    m_pHoverControl->onMouseMove(m_pHoverControl, m_mouseEvent);
                }
            }
        }
        if (m_pDownControl != m_pLastDownControl)
        {
            if (m_pLastDownControl)
            {
                if (m_pLastDownControl->onMouseUp)
                {
                    m_pLastDownControl->onMouseUp(m_pLastDownControl, m_mouseEvent);
                }
                if (m_pHoverControl == m_pLastDownControl &&
                    !m_mouseEvent.isMouseDown)
                {
                    if (m_pLastDownControl->onClick)
                    {
                        m_pLastDownControl->onClick(m_pLastDownControl, m_mouseEvent);
                    }
                    m_pLastDownControl->onClickInternal(m_mouseEvent);
                }
            }
            if (m_pDownControl)
            {
                if (m_pDownControl->onMouseDown)
                {
                    m_pDownControl->onMouseDown(m_pDownControl, m_mouseEvent);
                }
                m_pDownControl->onMouseDownInternal(m_mouseEvent);
            }
        }
    }

    void UIContext::reset()
    {
        m_lastMouseEvent = m_mouseEvent;
        m_pLastHoverControl = m_pHoverControl;
        m_pLastDownControl = m_pDownControl;
    }

    void UIControl::render(const UIContext& context) const
    {
        sUIRect parentRect = {{0, 0}, context.getScreenSize() };
        renderInternal(context, parentRect);
    }

    sUIRect UIControl::getWorldRect(const UIContext& context) const
    {
        if (m_pParent)
        {
            return std::move(getWorldRect(m_pParent->getWorldRect(context)));
        }
        else
        {        
            sUIRect parentRect = {{0, 0}, context.getScreenSize()};
            return std::move(getWorldRect(parentRect));
        }
    }

    sUIRect UIControl::getWorldRect(const sUIRect& parentRect) const
    {
        sUIRect worldRect;

        // Generate position and width values first
        switch (m_posType[0])
        {
            case eUIPosType::POS_RELATIVE:
                worldRect.position.x = m_rect.position.x;
                break;
            case eUIPosType::POS_PERCENTAGE:
                worldRect.position.x = parentRect.size.x * m_rect.position.x;
                break;
        }
        switch (m_posType[1])
        {
            case eUIPosType::POS_RELATIVE:
                worldRect.position.y = m_rect.position.y;
                break;
            case eUIPosType::POS_PERCENTAGE:
                worldRect.position.y = parentRect.size.y * m_rect.position.y;
                break;
        }
        switch (m_dimType[0])
        {
            case eUIDimType::DIM_ABSOLUTE:
                worldRect.size.x = m_rect.size.x;
                break;
            case eUIDimType::DIM_RELATIVE:
                worldRect.size.x = parentRect.size.x + m_rect.size.x;
                break;
            case eUIDimType::DIM_PERCENTAGE:
                worldRect.size.x = parentRect.size.x * m_rect.size.x;
                break;
        }
        switch (m_dimType[1])
        {
            case eUIDimType::DIM_ABSOLUTE:
                worldRect.size.y = m_rect.size.y;
                break;
            case eUIDimType::DIM_RELATIVE:
                worldRect.size.y = parentRect.size.y + m_rect.size.y;
                break;
            case eUIDimType::DIM_PERCENTAGE:
                worldRect.size.y = parentRect.size.y * m_rect.size.y;
                break;
        }

        // Then do alignement
        switch (m_align)
        {
            case eUIAlign::TOP_LEFT:
                worldRect.position.x = parentRect.position.x + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + worldRect.position.y;
                break;
            case eUIAlign::TOP:
                worldRect.position.x = parentRect.position.x + parentRect.size.x * .5f - worldRect.size.x * .5f + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + worldRect.position.y;
                break;
            case eUIAlign::TOP_RIGHT:
                worldRect.position.x = parentRect.position.x + parentRect.size.x - worldRect.size.x - worldRect.position.x;
                worldRect.position.y = parentRect.position.y + worldRect.position.y;
                break;
            case eUIAlign::LEFT:
                worldRect.position.x = parentRect.position.x + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + parentRect.size.y * .5f - worldRect.size.y * .5f + worldRect.position.y;
                break;
            case eUIAlign::CENTER:
                worldRect.position.x = parentRect.position.x + parentRect.size.x * .5f - worldRect.size.x * .5f + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + parentRect.size.y * .5f - worldRect.size.y * .5f + worldRect.position.y;
                break;
            case eUIAlign::RIGHT:
                worldRect.position.x = parentRect.position.x + parentRect.size.x - worldRect.size.x - worldRect.position.x;
                worldRect.position.y = parentRect.position.y + parentRect.size.y * .5f - worldRect.size.y * .5f + worldRect.position.y;
                break;
            case eUIAlign::BOTTOM_LEFT:
                worldRect.position.x = parentRect.position.x + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + parentRect.size.y - worldRect.size.y - worldRect.position.y;
                break;
            case eUIAlign::BOTTOM:
                worldRect.position.x = parentRect.position.x + parentRect.size.x * .5f - worldRect.size.x * .5f + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + parentRect.size.y - worldRect.size.y - worldRect.position.y;
                break;
            case eUIAlign::BOTTOM_RIGHT:
                worldRect.position.x = parentRect.position.x + parentRect.size.x - worldRect.size.x - worldRect.position.x;
                worldRect.position.y = parentRect.position.y + parentRect.size.y - worldRect.size.y - worldRect.position.y;
                break;
        }

        // Snap to pixels
        worldRect.position.x = std::roundf(worldRect.position.x);
        worldRect.position.y = std::roundf(worldRect.position.y);
            
        return std::move(worldRect);
    }

    void UIControl::updateInternal(UIContext& context, const sUIRect& parentRect)
    {
        if (!isEnabled() || isClickThrough()) return;
        sUIRect worldRect = getWorldRect(parentRect);

        // Do children first, inverted
        auto itend = m_children.rend();
        for (auto it = m_children.rbegin(); it != itend; ++it)
        {
            auto pChild = *it;
            pChild->updateInternal(context, worldRect);
        }

        if (!context.m_pHoverControl)
        {
            if (context.m_mouseEvent.mousePos.x >= worldRect.position.x &&
                context.m_mouseEvent.mousePos.y >= worldRect.position.y &&
                context.m_mouseEvent.mousePos.x <= worldRect.position.x + worldRect.size.x &&
                context.m_mouseEvent.mousePos.y <= worldRect.position.y + worldRect.size.y)
            {
                context.m_mouseEvent.localMousePos.x = context.m_mouseEvent.mousePos.x - worldRect.position.x;
                context.m_mouseEvent.localMousePos.y = context.m_mouseEvent.mousePos.y - worldRect.position.y;
                context.m_pHoverControl = this;
            }
        }
    }

    void UIControl::renderInternal(const UIContext& context, const sUIRect& parentRect) const
    {
        if (!isVisible()) return;

        sUIRect worldRect = getWorldRect(parentRect);
        renderControl(context, worldRect);
        for (auto pChild : m_children)
        {
            pChild->renderInternal(context, worldRect);
        }
    }

    void UIControl::setRect(const sUIRect& rect)
    {
        m_rect = rect;
    }

    void UIControl::setWorldRect(const UIContext& context, const sUIRect& rect)
    {
        if (!m_pParent)
        {
            m_rect = rect;
        }
        else
        {
            auto worldRect = m_pParent->getWorldRect(context);
            m_rect.position.x = rect.position.x - worldRect.position.x;
            m_rect.position.y = rect.position.y - worldRect.position.y;
        }
        //m_rect.size.x = rect.size.x - worldRect.size.x;
        //m_rect.size.y = rect.size.y - worldRect.size.y;
    }

    void UIControl::setName(const std::string& name)
    {
        m_name = name;
    }

    void UIControl::setIsEnabled(bool bIsEnabled)
    {
        m_isEnabled = bIsEnabled;
    }

    void UIControl::setIsVisible(bool bIsVisible)
    {
        m_isVisible = bIsVisible;
    }

    void UIControl::setIsClickThrough(bool bIsClickThrough)
    {
        m_isClickThrough = bIsClickThrough;
    }

    eUIState UIControl::getState(const UIContext& context) const
    {
        if (isEnabled())
        {
            if (context.m_pHoverControl == this)
            {
                if (context.m_pDownControl == this)
                {
                    return eUIState::DOWN;
                }
                else
                {
                    return eUIState::HOVER;
                }
            }
            else if (context.m_pDownControl == this)
            {
                return eUIState::HOVER;
            }
            else
            {
                return eUIState::NORMAL;
            }
        }
        else
        {
            return eUIState::DISABLED;
        }
    }

    void UIControl::setUserData(void* pUserData)
    {
        m_pUserData = pUserData;
    }

    void UIControl::setAlign(eUIAlign align)
    {
        m_align = align;
    }

    void UIControl::setWidthType(eUIDimType widthType)
    {
        m_dimType[0] = widthType;
    }

    void UIControl::setHeightType(eUIDimType heightType)
    {
        m_dimType[1] = heightType;
    }

    void UIControl::setXType(eUIPosType xType)
    {
        m_posType[0] = xType;
    }

    void UIControl::setYType(eUIPosType yType)
    {
        m_posType[1] = yType;
    }

    const UIProperty& UIControl::getProperty(const std::string& name) const
    {
        auto& it = m_properties.find(name);
        return it->second;
    }

    void UITreeView::addItem(UITreeViewItem* pItem)
    {
        for (auto pMyItem : m_items)
        {
            if (pMyItem == pItem) return;
        }
        pItem->m_pTreeView = this;
        m_items.push_back(pItem);
    }

    void UITreeView::removeItem(UITreeViewItem* pItem)
    {
        auto size = m_items.size();
        for (decltype(size) i = 0; i < size; ++i)
        {
            if (m_items[i] == pItem)
            {
                m_items.erase(m_items.begin() + i);
                delete pItem;
                return;
            }
        }
    }

    UITreeViewItem* UITreeView::getItemAtPosition(const sUIVector2& pos, const sUIRect& rect, bool* pPickedExpandButton) const
    {
        // Render it's items
        sUIRect itemRect = {rect.position, {rect.size.x, m_itemHeight}};
        for (auto pItem : m_items)
        {
            auto pRet = getItemAtPosition(pItem, pos, itemRect, pPickedExpandButton);
            if (pRet)
            {
                return pRet;
            }
        }
        return nullptr;
    }

    UITreeViewItem* UITreeView::getItemAtPosition(UITreeViewItem* pItem, const sUIVector2& pos, sUIRect& rect, bool* pPickedExpandButton) const
    {
        if (pos.y >= rect.position.y &&
            pos.y <= rect.position.y + rect.size.y)
        {
            if (pos.x >= rect.position.x + getExpandClickWidth() ||
                pos.x <= rect.position.x)
            {
                return pItem;
            }
            else if (pPickedExpandButton)
            {
                *pPickedExpandButton = true;
                return pItem;
            }
        }
        rect.position.y += getItemHeight();
        if (pItem->m_isExpanded)
        {
            if (!pItem->m_items.empty())
            {
                auto xOffset = getExpandedXOffset();
                rect.position.x += xOffset;
                rect.size.x -= xOffset;
                for (auto pHisItem : pItem->m_items)
                {
                    auto pRet = getItemAtPosition(pHisItem, pos, rect, pPickedExpandButton);
                    if (pRet)
                    {
                        return pRet;
                    }
                }
                rect.size.x += xOffset;
                rect.position.x -= xOffset;
            }
        }
        return nullptr;
    }

    void UITreeView::unselectAll()
    {
        m_selectedItems.clear();
        for (auto pItem : m_items)
        {
            unselectItem(pItem);
        }
    }

    void UITreeView::unselectItem(UITreeViewItem* pItem)
    {
        pItem->m_isSelected = false;
        for (auto pHisItem : pItem->m_items)
        {
            unselectItem(pHisItem);
        }
    }

    void UITreeView::addSelectedItem(UITreeViewItem* pItem)
    {
        m_selectedItems.push_back(pItem);
        pItem->m_isSelected = true;
        expandTo(pItem);
    }

    void UITreeView::expandTo(UITreeViewItem* pItem)
    {
        if (pItem->m_pParent)
        {
            pItem->m_pParent->m_isExpanded = true;
            expandTo(pItem->m_pParent);
        }
    }

    void UITreeViewItem::addItem(UITreeViewItem* pItem)
    {
        for (auto pMyItem : m_items)
        {
            if (pMyItem == pItem) return;
        }
        pItem->m_pParent = this;
        pItem->m_pTreeView = m_pTreeView;
        m_items.push_back(pItem);
    }

    void UITreeViewItem::removeItem(UITreeViewItem* pItem)
    {
        auto size = m_items.size();
        for (decltype(size) i = 0; i < size; ++i)
        {
            if (m_items[i] == pItem)
            {
                m_items.erase(m_items.begin() + i);
                delete pItem;
                return;
            }
        }
    }

    //--- Copy
    UIButton::UIButton(const UIButton& other) :
        UIControl(other)
    {
        m_caption = other.m_caption;
    }

    UIPanel::UIPanel(const UIPanel& other) :
        UIControl(other)
    {
    }

    UILabel::UILabel(const UILabel& other) :
        UIControl(other)
    {
        m_text = other.m_text;
    }

    UIImage::UIImage(const UIImage& other) :
        UIControl(other)
    {
        m_image = other.m_image;
    }

    UICheckBox::UICheckBox(const UICheckBox& other) :
        UIControl(other)
    {
        m_caption = other.m_caption;
        m_isChecked = other.m_isChecked;
        m_behavior = other.m_behavior;
    }

    UITreeView::UITreeView(const UITreeView& other) :
        UIControl(other)
    {
        for (auto pOtherItem : other.m_items)
        {
            m_items.push_back(new UITreeViewItem(*pOtherItem));
        }
    }

    UITreeViewItem::UITreeViewItem(const std::string& text) :
        m_text(text)
    {
    }

    UITreeViewItem::UITreeViewItem(const UITreeViewItem& other)
    {
        for (auto pOtherItem : other.m_items)
        {
            m_items.push_back(new UITreeViewItem(*pOtherItem));
        }
        m_isExpanded = other.m_isExpanded;
        m_text = other.m_text;
    }

    //--- Properties
    void UIButton::setCaption(const std::string& caption)
    {
        m_caption = caption;
    }

    void UIPanel::setColor(const sUIColor& color)
    {
        m_color = color;
    }

    void UILabel::setText(const std::string& text)
    {
        m_text = text;
    }

    void UIImage::setImage(const std::string& image)
    {
        m_image = image;
    }

    void UICheckBox::setIsChecked(bool in_isChecked)
    {
        switch (m_behavior)
        {
            case eUICheckBehavior::NORMAL:
            {
                m_isChecked = in_isChecked;
                break;
            }
            case eUICheckBehavior::OPTIONAL:
            {
                if (!m_isChecked && in_isChecked)
                {
                    if (getParent())
                    {
                        auto& sibblings = getParent()->getChildren();
                        for (auto pSibbling : sibblings)
                        {
                            if (pSibbling == this) continue;
                            if (pSibbling->getType() == eUIType::UI_CHECKBOX)
                            {
                                auto pSibblingCheckBox = dynamic_cast<UICheckBox*>(pSibbling);
                                if (pSibblingCheckBox->getBehavior() == m_behavior)
                                {
                                    if (pSibblingCheckBox->m_isChecked)
                                    {
                                        pSibblingCheckBox->m_isChecked = false;
                                    }
                                }
                            }
                        }
                    }
                }

                m_isChecked = in_isChecked;
                break;
            }
            case eUICheckBehavior::EXCLUSIVE:
            {
                if (!m_isChecked && in_isChecked)
                {
                    if (getParent())
                    {
                        auto& sibblings = getParent()->getChildren();
                        for (auto pSibbling : sibblings)
                        {
                            if (pSibbling == this) continue;
                            if (pSibbling->getType() == eUIType::UI_CHECKBOX)
                            {
                                auto pSibblingCheckBox = dynamic_cast<UICheckBox*>(pSibbling);
                                if (pSibblingCheckBox->getBehavior() == m_behavior)
                                {
                                    if (pSibblingCheckBox->m_isChecked)
                                    {
                                        pSibblingCheckBox->m_isChecked = false;
                                    }
                                }
                            }
                        }
                    }

                    m_isChecked = in_isChecked;
                }
                break;
            }
        }
    }

    void UICheckBox::setBehavior(eUICheckBehavior behavior)
    {
        m_behavior = behavior;
    }

    void UITreeView::setExpandedXOffset(float expandedXOffset)
    {
        m_expandedXOffset = expandedXOffset;
    }

    void UITreeView::setExpandClickWidth(float expandClickWidth)
    {
        m_expandClickWidth = expandClickWidth;
    }

    void UITreeView::setItemHeight(float itemHeight)
    {
        m_itemHeight = itemHeight;
    }

    void UITreeViewItem::setIsExpanded(bool isExpanded)
    {
        m_isExpanded = isExpanded;
    }

    void UITreeViewItem::setText(const std::string& text)
    {
        m_text = text;
    }

    void UITreeViewItem::setUserData(void* pUserData)
    {
        m_pUserData = pUserData;
    }

    //--- Loads
    void UIButton::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        m_caption = getJsonString(jsonNode["caption"]);
    }

    void UIPanel::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        m_color = getJsonColor(jsonNode["color"]);
    }

    void UILabel::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        m_text = getJsonString(jsonNode["text"]);
    }

    void UIImage::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        m_image = getJsonString(jsonNode["image"]);
    }

    void UICheckBox::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        m_caption = getJsonString(jsonNode["caption"]);
        m_isChecked = getJsonBool(jsonNode["checked"], false);
        m_behavior = getJsonCheckBehavior(getJsonString(jsonNode["behavior"]));
    }

    void UITreeView::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        m_expandedXOffset = getJsonFloat(jsonNode["expandedXOffset"], 18.f);
        m_expandClickWidth = getJsonFloat(jsonNode["expandClickWidth"], 18.f);
        m_itemHeight = getJsonFloat(jsonNode["itemHeight"], 18.f);
    }

    //--- Renders
    void UIButton::renderControl(const UIContext& context, const sUIRect& rect) const
    {
        const auto& callback = context.getStyle<UIButton>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }
    }

    void UIPanel::renderControl(const UIContext& context, const sUIRect& rect) const
    {
        const auto& callback = context.getStyle<UIPanel>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }
    }

    void UILabel::renderControl(const UIContext& context, const sUIRect& rect) const
    {
        const auto& callback = context.getStyle<UILabel>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }
    }

    void UIImage::renderControl(const UIContext& context, const sUIRect& rect) const
    {
        const auto& callback = context.getStyle<UIImage>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }
    }

    void UICheckBox::renderControl(const UIContext& context, const sUIRect& rect) const
    {
        const auto& callback = context.getStyle<UICheckBox>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }
    }

    void UITreeView::renderControl(const UIContext& context, const sUIRect& rect) const
    {
        const auto& callback = context.getStyle<UITreeView>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }

        // Render it's items
        sUIRect itemRect = {rect.position, {rect.size.x, m_itemHeight}};
        const auto& itemCallback = context.getStyle<UITreeViewItem>(getStyle());
        if (itemCallback)
        {
            for (auto pItem : m_items)
            {
                pItem->render(itemCallback, this, rect, itemRect);
            }
        }
    }

    //--- Internal events
    void UICheckBox::onClickInternal(const UIMouseEvent& evt)
    {
        switch (m_behavior)
        {
            case eUICheckBehavior::NORMAL:
            {
                m_isChecked = !m_isChecked;
                if (onCheckChanged)
                {
                    UICheckEvent checkEvt;
                    checkEvt.isChecked = m_isChecked;
                    checkEvt.pContext = evt.pContext;
                    onCheckChanged(this, checkEvt);
                }
                break;
            }
            case eUICheckBehavior::OPTIONAL:
            {
                if (!m_isChecked)
                {
                    if (getParent())
                    {
                        auto& sibblings = getParent()->getChildren();
                        for (auto pSibbling : sibblings)
                        {
                            if (pSibbling == this) continue;
                            if (pSibbling->getType() == eUIType::UI_CHECKBOX)
                            {
                                auto pSibblingCheckBox = dynamic_cast<UICheckBox*>(pSibbling);
                                if (pSibblingCheckBox->getBehavior() == m_behavior)
                                {
                                    if (pSibblingCheckBox->m_isChecked)
                                    {
                                        pSibblingCheckBox->m_isChecked = false;
                                        if (pSibblingCheckBox->onCheckChanged)
                                        {
                                            UICheckEvent checkEvt;
                                            checkEvt.isChecked = pSibblingCheckBox->m_isChecked;
                                            checkEvt.pContext = evt.pContext;
                                            pSibblingCheckBox->onCheckChanged(this, checkEvt);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                m_isChecked = !m_isChecked;
                if (onCheckChanged)
                {
                    UICheckEvent checkEvt;
                    checkEvt.isChecked = m_isChecked;
                    checkEvt.pContext = evt.pContext;
                    onCheckChanged(this, checkEvt);
                }
                break;
            }
            case eUICheckBehavior::EXCLUSIVE:
            {
                if (!m_isChecked)
                {
                    if (getParent())
                    {
                        auto& sibblings = getParent()->getChildren();
                        for (auto pSibbling : sibblings)
                        {
                            if (pSibbling == this) continue;
                            if (pSibbling->getType() == eUIType::UI_CHECKBOX)
                            {
                                auto pSibblingCheckBox = dynamic_cast<UICheckBox*>(pSibbling);
                                if (pSibblingCheckBox->getBehavior() == m_behavior)
                                {
                                    if (pSibblingCheckBox->m_isChecked)
                                    {
                                        pSibblingCheckBox->m_isChecked = false;
                                        if (pSibblingCheckBox->onCheckChanged)
                                        {
                                            UICheckEvent checkEvt;
                                            checkEvt.isChecked = pSibblingCheckBox->m_isChecked;
                                            checkEvt.pContext = evt.pContext;
                                            pSibblingCheckBox->onCheckChanged(this, checkEvt);
                                        }
                                    }
                                }
                            }
                        }
                    }

                    m_isChecked = true;
                    if (onCheckChanged)
                    {
                        UICheckEvent checkEvt;
                        checkEvt.isChecked = m_isChecked;
                        checkEvt.pContext = evt.pContext;
                        onCheckChanged(this, checkEvt);
                    }
                }
                break;
            }
        }
    }

    void UITreeView::onMouseDownInternal(const UIMouseEvent& evt)
    {
        auto worldRect = getWorldRect(*evt.pContext);
        bool pickedExpandButton = false;
        auto pPicked = getItemAtPosition(evt.mousePos, worldRect, &pickedExpandButton);
        if (pPicked)
        {
            if (pickedExpandButton)
            {
                pPicked->m_isExpanded = !pPicked->m_isExpanded;
            }
            else
            {
                unselectAll();
                addSelectedItem(pPicked);
                if (onSelectionChanged)
                {
                    UITreeViewSelectEvent myEvt;
                    myEvt.pContext = evt.pContext;
                    myEvt.pSelectedItems = &m_selectedItems;
                    onSelectionChanged(this, myEvt);
                }
            }
        }
    }
};
