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
        m_style = uiHash(getJsonString(jsonNode["style"]));

        m_isEnabled = getJsonBool(jsonNode["enabled"], true);
        m_isVisible = getJsonBool(jsonNode["visible"], true);
        m_isClickThrough = getJsonBool(jsonNode["clickThrough"], false);

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
                case eUIType::UI_LABEL:
                    add(new UILabel(*(UILabel*)pChild));
                    break;
                case eUIType::UI_IMAGE:
                    add(new UIImage(*(UIImage*)pChild));
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

    sUIRect UIControl::getWorldRect() const
    {
        if (m_pParent)
        {
            return std::move(getWorldRect(m_pParent->getWorldRect()));
        }
        return m_rect;
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
            
        return std::move(worldRect);
    }

    void UIControl::updateInternal(UIContext& context, const sUIRect& parentRect, bool bIsParentHover)
    {
        if (!isEnabled() || isClickThrough()) return;

        sUIRect worldRect = getWorldRect(parentRect);
        if (!context.m_pHoverControl || bIsParentHover)
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
            pChild->updateInternal(context, worldRect, bIsParentHover || context.m_pHoverControl == this);
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
};
