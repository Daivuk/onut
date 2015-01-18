#include "UI.h"
#include "rapidjson/document.h"
#include "rapidjson/filestream.h"

namespace onut
{
    int uiHash(const char* pStr)
    {
        int hash = 0;
        while (*pStr)
        {
            hash = (hash << 1) | *pStr;
            ++pStr;
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

    UIContext::UIContext(const sUIVector2& screenSize) :
        m_screenSize(screenSize)
    {
    }

    void UIContext::resize(const sUIVector2& screenSize)
    {
        m_screenSize = screenSize;
    }

    void UIContext::addStyle(const char* szStyle, const TfnCallback& renderCallback)
    {
        m_callbacks[uiHash(szStyle)] = renderCallback;
    }

    const UIContext::TfnCallback& UIContext::getStyle(int styleId) const
    {
        auto& it = m_callbacks.find(styleId);
        if (it != m_callbacks.end())
        {
            return it->second;
        }
        static UIContext::TfnCallback nullFn = nullptr;
        return nullFn;
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
        m_style = uiHash(getJsonString(jsonNode["style"]));

        m_isEnabled = getJsonBool(jsonNode["enabled"], true);
        m_isVisible = getJsonBool(jsonNode["visible"], true);

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
                if (!pChild) continue;
                add(pChild);
                pChild->load(jsonChild);
            }
        }
    }

    UIControl::UIControl(const UIControl& other)
    {
        m_rect = other.m_rect;
        m_align = other.m_align;
        m_posType[0] = other.m_posType[0];
        m_posType[1] = other.m_posType[1];
        m_dimType[0] = other.m_dimType[0];
        m_dimType[1] = other.m_dimType[1];
        m_name = other.m_name;
        m_pParent = nullptr;

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

    UIControl* UIControl::getChild(const std::string& name, bool bSearchSubChildren)
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

    void UIControl::setStyle(const char* szStyle)
    {
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
                }
            }
            if (m_pDownControl)
            {
                if (m_pDownControl->onMouseDown)
                {
                    m_pDownControl->onMouseDown(m_pDownControl, m_mouseEvent);
                }
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

    sUIRect UIControl::getWorldRect(const sUIRect& parentRect) const
    {
        sUIRect worldRect;

        // Generate position and width values first
        switch (m_posType[0])
        {
            case eUIPosType::POS_RELATIVE:
                worldRect.position.x = parentRect.position.x + m_rect.position.x;
                break;
            case eUIPosType::POS_PERCENTAGE:
                worldRect.position.x = parentRect.position.x + parentRect.size.x * m_rect.position.x;
                break;
        }
        switch (m_posType[1])
        {
            case eUIPosType::POS_RELATIVE:
                worldRect.position.y = parentRect.position.y + m_rect.position.y;
                break;
            case eUIPosType::POS_PERCENTAGE:
                worldRect.position.y = parentRect.position.y + parentRect.size.y * m_rect.position.y;
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
                break;
            case eUIAlign::TOP:
                worldRect.position.x = worldRect.position.x - worldRect.size.x * .5f;
                break;
            case eUIAlign::TOP_RIGHT:
                worldRect.position.x = worldRect.position.x - worldRect.size.x;
                break;
            case eUIAlign::LEFT:
                worldRect.position.y = worldRect.position.y - worldRect.size.y * .5f;
                break;
            case eUIAlign::CENTER:
                worldRect.position.x = worldRect.position.x - worldRect.size.x * .5f;
                worldRect.position.y = worldRect.position.y - worldRect.size.y * .5f;
                break;
            case eUIAlign::RIGHT:
                worldRect.position.x = worldRect.position.x - worldRect.size.x;
                worldRect.position.y = worldRect.position.y - worldRect.size.y * .5f;
                break;
            case eUIAlign::BOTTOM_LEFT:
                worldRect.position.y = worldRect.position.y - worldRect.size.y;
                break;
            case eUIAlign::BOTTOM:
                worldRect.position.x = worldRect.position.x - worldRect.size.x * .5f;
                worldRect.position.y = worldRect.position.y - worldRect.size.y;
                break;
            case eUIAlign::BOTTOM_RIGHT:
                worldRect.position.x = worldRect.position.x - worldRect.size.x;
                worldRect.position.y = worldRect.position.y - worldRect.size.y;
                break;
        }
            
        return std::move(worldRect);
    }

    void UIControl::updateInternal(UIContext& context, const sUIRect& parentRect)
    {
        if (!isEnabled()) return;

        sUIRect worldRect = getWorldRect(parentRect);
        if (!context.m_pHoverControl || context.m_pHoverControl == m_pParent)
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
        for (auto pChild : m_children)
        {
            pChild->updateInternal(context, worldRect);
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

    void UIControl::setName(const std::string& name)
    {
        m_name = name;
    }

    inline void UIControl::setIsEnabled(bool bIsEnabled)
    {
        m_isEnabled = bIsEnabled;
    }

    inline void UIControl::setIsVisible(bool bIsVisible)
    {
        m_isVisible = bIsVisible;
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

    void UIButton::load(const rapidjson::Value& jsonNode)
    {
        m_caption = getJsonString(jsonNode["caption"], "");
        UIControl::load(jsonNode);
    }

    UIButton::UIButton(const UIButton& other) :
        UIControl(other)
    {
        m_caption = other.m_caption;
    }

    UIButton::~UIButton()
    {
    }

    void UIButton::setCaption(const std::string& caption)
    {
        m_caption = caption;
    }

    void UIButton::renderControl(const UIContext& context, const sUIRect& rect) const
    {
        const auto& callback = context.getStyle(getStyle());
        if (callback)
        {
            callback(this, rect);
        }
    }

    UIPanel::UIPanel(const UIPanel& other) :
        UIControl(other)
    {
    }

    void UIPanel::renderControl(const UIContext& context, const sUIRect& rect) const
    {
        const auto& callback = context.getStyle(getStyle());
        if (callback)
        {
            callback(this, rect);
        }
    }
};
