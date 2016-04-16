// Onut
#include <onut/ContentManager.h>
#include <onut/Crypto.h>
#include <onut/UIButton.h>
#include <onut/UICheckBox.h>
#include <onut/UIComponents.h>
#include <onut/UIContext.h>
#include <onut/UIControl.h>
#include <onut/UIImage.h>
#include <onut/UILabel.h>
#include <onut/UIPanel.h>
#include <onut/UITextBox.h>
#include <onut/UITreeView.h>

// Private
#include "UIJson.h"

OUIControlRef oUI;

namespace onut
{
    // Strings to Enums used for loading/saving
    static std::unordered_map<std::string, onut::Align> alignMap = {
        {"TOP_LEFT", onut::Align::TopLeft},
        {"TOP", onut::Align::Top},
        {"TOP_RIGHT", onut::Align::TopRight},
        {"LEFT", onut::Align::Left},
        {"CENTER", onut::Align::Center},
        {"RIGHT", onut::Align::Right},
        {"BOTTOM_LEFT", onut::Align::BottomLeft},
        {"BOTTOM", onut::Align::Bottom},
        {"BOTTOM_RIGHT", onut::Align::BottomRight}
    };
    static std::unordered_map<std::string, UIControl::DimType> dimTypeMap = {
        {"ABSOLUTE", UIControl::DimType::Absolute},
        {"RELATIVE", UIControl::DimType::Relative},
        {"PERCENTAGE", UIControl::DimType::Percentage}
    };
    static std::unordered_map<std::string, UIControl::PosType> posTypeMap = {
        {"RELATIVE", UIControl::PosType::Relative},
        {"PERCENTAGE", UIControl::PosType::Percentage}
    };
    static std::unordered_map<std::string, UIControl::AnchorType> anchorTypeMap = {
        {"PIXEL", UIControl::AnchorType::Pixel},
        {"PERCENTAGE", UIControl::AnchorType::Percentage}
    };
    static std::unordered_map<std::string, UIControl::Type> typeMap = {
        {"UIControl", UIControl::Type::Control},
        {"UIButton", UIControl::Type::Button},
        {"UIPanel", UIControl::Type::Panel},
        {"UILabel", UIControl::Type::Label},
        {"UIImage", UIControl::Type::Image},
        {"UICheckBox", UIControl::Type::CheckBox},
        {"UITreeView", UIControl::Type::TreeView},
        {"UITextBox", UIControl::Type::TextBox},
    };

    UIControl::Property::Property()
    {
        m_szString = nullptr;
        m_bool = false;
        m_int = 0;
        m_float = 0.f;
        m_type = Type::Int;
    }

    UIControl::Property::Property(const rapidjson::Value& jsonNode)
    {
        m_szString = nullptr;
        m_bool = false;
        m_int = 0;
        m_float = 0.f;
        if (jsonNode.IsNumber())
        {
            if (jsonNode.IsDouble())
            {
                m_type = Type::Float;
                m_float = static_cast<float>(jsonNode.GetDouble());
            }
            else if (jsonNode.IsInt())
            {
                m_type = Type::Int;
                m_int = jsonNode.GetInt();
            }
            else
            {
                assert(false);
            }
        }
        else if (jsonNode.IsBool())
        {
            m_type = Type::Bool;
            m_bool = jsonNode.GetBool();
        }
        else if (jsonNode.IsString())
        {
            m_type = Type::String;
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

    UIControl::Property::Property(const Property& other)
    {
        m_szString = nullptr;
        m_bool = false;
        m_int = 0;
        m_float = 0.f;
        m_type = other.m_type;
        switch (m_type)
        {
            case Type::Bool:
                m_bool = other.m_bool;
                break;
            case Type::Int:
                m_int = other.m_int;
                break;
            case Type::Float:
                m_float = other.m_float;
                break;
            case Type::String:
            {
                auto len = strlen(other.m_szString);
                m_szString = new char[len + 1];
                memcpy(m_szString, other.m_szString, len + 1);
                break;
            }
        }
    }

    UIControl::Property& UIControl::Property::operator=(const UIControl::Property& other)
    {
        if (m_type == Type::String)
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
            case Type::Bool:
                m_bool = other.m_bool;
                break;
            case Type::Int:
                m_int = other.m_int;
                break;
            case Type::Float:
                m_float = other.m_float;
                break;
            case Type::String:
            {
                auto len = strlen(other.m_szString);
                m_szString = new char[len + 1];
                memcpy(m_szString, other.m_szString, len + 1);
                break;
            }
        }
        return *this;
    }

    UIControl::Property::~Property()
    {
        if (m_type == Type::String)
        {
            delete[] m_szString;
        }
    }

    const char* UIControl::Property::getString() const
    {
        assert(m_type == Type::String);
        return m_szString;
    }

    int UIControl::Property::getInt() const
    {
        assert(m_type == Type::Int);
        return m_int;
    }

    float UIControl::Property::getFloat() const
    {
        assert(m_type == Type::Float || m_type == Type::Int);
        if (m_type == Type::Float)
        {
            return m_float;
        }
        else
        {
            return static_cast<float>(m_int);
        }
    }

    bool UIControl::Property::getBool() const
    {
        assert(m_type == Type::Bool || m_type == Type::Int);
        if (m_type == Type::Bool)
        {
            return m_bool;
        }
        else
        {
            return m_int ? true : false;
        }
    }

    OUIControlRef UIControl::create()
    {
        return std::shared_ptr<UIControl>(new UIControl());
    }

    OUIControlRef UIControl::createFromFile(const std::string& in_filename, OContentManagerRef pContentManager)
    {
        auto pControl = std::shared_ptr<UIControl>(new UIControl());

        if (!pContentManager) pContentManager = oContentManager;
        auto filename = oContentManager->findResourceFile(in_filename);
        if (filename.empty()) filename = in_filename;

        // Open json file
        FILE* pFile = nullptr;
        auto fopenRet = fopen_s(&pFile, filename.c_str(), "r");
        assert(!fopenRet);
        rapidjson::FileStream is(pFile);
        rapidjson::Document doc;
        doc.ParseStream<0>(is);

        pControl->load(doc);

        // Close the file
        fclose(pFile);

        return pControl;
    }

    UIControl::UIControl()
    {
    }

    bool UIControl::visit(const std::function<bool(const OUIControlRef&, const Rect&)>& callback, const Rect& parentRect)
    {
        auto worldRect = getWorldRect(parentRect);
        if (!callback(OThis, worldRect))
        {
            return false;
        }
        for (auto& pChild : m_children)
        {
            if (!pChild->visit(callback, worldRect))
            {
                return false;
            }
        }
        return true;
    }

    bool UIControl::visitChildrenFirst(const std::function<bool(const OUIControlRef&, const Rect&)>& callback, const Rect& parentRect)
    {
        auto worldRect = getWorldRect(parentRect);
        for (auto& pChild : m_children)
        {
            if (!pChild->visitChildrenFirst(callback, worldRect))
            {
                return false;
            }
        }
        if (!callback(OThis, worldRect))
        {
            return false;
        }
        return true;
    }

    bool UIControl::visitEnabled(const std::function<bool(const OUIControlRef&, const Rect&)>& callback, const Rect& parentRect)
    {
        if (!isEnabled) return true;
        if (!isVisible) return true;
        auto worldRect = getWorldRect(parentRect);
        if (!callback(OThis, worldRect))
        {
            return false;
        }
        for (auto& pChild : m_children)
        {
            if (!pChild->visitEnabled(callback, worldRect))
            {
                return false;
            }
        }
        return true;
    }

    bool UIControl::visitChildrenFirstEnabled(const std::function<bool(const OUIControlRef&, const Rect&)>& callback, const Rect& parentRect)
    {
        if (!isEnabled) return true;
        if (!isVisible) return true;
        auto worldRect = getWorldRect(parentRect);
        for (auto& pChild : m_children)
        {
            if (!pChild->visitChildrenFirstEnabled(callback, worldRect))
            {
                return false;
            }
        }
        if (!callback(OThis, worldRect))
        {
            return false;
        }
        return true;
    }

    bool UIControl::visitVisible(const std::function<bool(const OUIControlRef&, const Rect&)>& callback, const Rect& parentRect)
    {
        if (!isVisible) return true;
        auto worldRect = getWorldRect(parentRect);
        if (!callback(OThis, worldRect))
        {
            return false;
        }
        for (auto& pChild : m_children)
        {
            if (!pChild->visitVisible(callback, worldRect))
            {
                return false;
            }
        }
        return true;
    }

    bool UIControl::visitChildrenFirstVisible(const std::function<bool(const OUIControlRef&, const Rect&)>& callback, const Rect& parentRect)
    {
        if (!isVisible) return true;
        auto worldRect = getWorldRect(parentRect);
        for (auto& pChild : m_children)
        {
            if (!pChild->visitChildrenFirstVisible(callback, worldRect))
            {
                return false;
            }
        }
        if (!callback(OThis, worldRect))
        {
            return false;
        }
        return true;
    }

    void UIControl::load(const rapidjson::Value& jsonNode)
    {
        rect.x = getJsonFloat(jsonNode["x"]);
        rect.y = getJsonFloat(jsonNode["y"]);
        rect.z = getJsonFloat(jsonNode["width"], 0.f);
        rect.w = getJsonFloat(jsonNode["height"], 0.f);
        anchor.x = getJsonFloat(jsonNode["xAnchor"]);
        anchor.y = getJsonFloat(jsonNode["yAnchor"]);

        align = getJsonEnum(alignMap, jsonNode["align"], onut::Align::TopLeft);
        xType = getJsonEnum(posTypeMap, jsonNode["xType"], UIControl::PosType::Relative);
        yType = getJsonEnum(posTypeMap, jsonNode["yType"], UIControl::PosType::Relative);
        widthType = getJsonEnum(dimTypeMap, jsonNode["widthType"], UIControl::DimType::Absolute);
        heightType = getJsonEnum(dimTypeMap, jsonNode["heightType"], UIControl::DimType::Absolute);
        xAnchorType = getJsonEnum(anchorTypeMap, jsonNode["anchorType"], UIControl::AnchorType::Percentage);
        yAnchorType = getJsonEnum(anchorTypeMap, jsonNode["anchorType"], UIControl::AnchorType::Percentage);

        name = getJsonString(jsonNode["name"]);
        m_styleName = getJsonString(jsonNode["style"]);
        m_style = OHash(m_styleName.c_str());

        isEnabled = getJsonBool(jsonNode["enabled"], true);
        isVisible = getJsonBool(jsonNode["visible"], true);
        isClickThrough = getJsonBool(jsonNode["clickThrough"], false);
        clipChildren = getJsonBool(jsonNode["clipChildren"], false);

        // Properties
        auto& properties = jsonNode["properties"];
        if (!properties.IsNull())
        {
            for (auto it = properties.MemberBegin(); it != properties.MemberEnd(); ++it)
            {
                m_properties[it->name.GetString()] = Property(it->value);
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
                OUIControlRef pChild;
                if (!strcmp(jsonChildType, "UIControl"))
                {
                    pChild = UIControl::create();
                }
                else if (!strcmp(jsonChildType, "UIButton"))
                {
                    pChild = UIButton::create();
                }
                else if (!strcmp(jsonChildType, "UIPanel"))
                {
                    pChild = UIPanel::create();
                }
                else if (!strcmp(jsonChildType, "UILabel"))
                {
                    pChild = UILabel::create();
                }
                else if (!strcmp(jsonChildType, "UIImage"))
                {
                    pChild = UIImage::create();
                }
                else if (!strcmp(jsonChildType, "UICheckBox"))
                {
                    pChild = UICheckBox::create();
                }
                else if (!strcmp(jsonChildType, "UITreeView"))
                {
                    pChild = UITreeView::create();
                }
                else if (!strcmp(jsonChildType, "UITextBox"))
                {
                    pChild = UITextBox::create();
                }
                if (!pChild) continue;
                add(pChild);
                pChild->load(jsonChild);
            }
        }
    }

    void UIControl::save(const std::string& filename) const
    {
        rapidjson::Document doc;
        doc.SetObject();
        auto& allocator = doc.GetAllocator();
        save(doc, allocator);

        // Open json file
        FILE* pFile = nullptr;
        auto fopenRet = fopen_s(&pFile, filename.c_str(), "wb");
        assert(!fopenRet);

        rapidjson::StringBuffer s;
        rapidjson::Writer<rapidjson::StringBuffer> writer(s);
        doc.Accept(writer);

        auto str = s.GetString();
        fwrite(str, 1, strlen(str), pFile);

        fclose(pFile);
    }

    void UIControl::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        setJsonString(jsonNode, "type", enumToString(typeMap, getType()), allocator);

        setJsonFloat(jsonNode, "x", rect.x, allocator);
        setJsonFloat(jsonNode, "y", rect.y, allocator);
        setJsonFloat(jsonNode, "width", rect.z, allocator);
        setJsonFloat(jsonNode, "height", rect.w, allocator);
        setJsonFloat(jsonNode, "xAnchor", anchor.x, allocator);
        setJsonFloat(jsonNode, "yAnchor", anchor.y, allocator);

        setJsonString(jsonNode, "align", enumToString(alignMap, align), allocator, "TOP_LEFT");
        setJsonString(jsonNode, "xType", enumToString(posTypeMap, xType), allocator, "RELATIVE");
        setJsonString(jsonNode, "yType", enumToString(posTypeMap, yType), allocator, "RELATIVE");
        setJsonString(jsonNode, "widthType", enumToString(dimTypeMap, widthType), allocator, "ABSOLUTE");
        setJsonString(jsonNode, "heightType", enumToString(dimTypeMap, heightType), allocator, "ABSOLUTE");
        setJsonString(jsonNode, "xAnchorType", enumToString(anchorTypeMap, xAnchorType), allocator, "PERCENTAGE");
        setJsonString(jsonNode, "yAnchorType", enumToString(anchorTypeMap, yAnchorType), allocator, "PERCENTAGE");

        setJsonString(jsonNode, "name", name.c_str(), allocator);
        setJsonString(jsonNode, "style", m_styleName.c_str(), allocator);

        setJsonBool(jsonNode, "enabled", isEnabled, allocator, true);
        setJsonBool(jsonNode, "visible", isVisible, allocator, true);
        setJsonBool(jsonNode, "clickThrough", isClickThrough, allocator, false);
        setJsonBool(jsonNode, "clipChildren", clipChildren, allocator, false);

        for (auto& kv : m_properties)
        {
            switch (kv.second.getType())
            {
                case Property::Type::Bool:
                    jsonNode[kv.first.c_str()] = kv.second.getBool();
                    break;
                case Property::Type::Float:
                    jsonNode[kv.first.c_str()] = kv.second.getFloat();
                    break;
                case Property::Type::Int:
                    jsonNode[kv.first.c_str()] = kv.second.getInt();
                    break;
                case Property::Type::String:
                    jsonNode[kv.first.c_str()] = kv.second.getString();
                    break;
            }
        }

        if (!m_children.empty())
        {
            rapidjson::Value jsonChildren(rapidjson::kArrayType);
            for (auto pChild : m_children)
            {
                rapidjson::Value jsonChild(rapidjson::kObjectType);
                pChild->save(jsonChild, allocator);
                jsonChildren.PushBack(jsonChild, allocator);
            }
            jsonNode.AddMember("children", jsonChildren, allocator);
        }
    }

    void UIControl::operator=(const UIControl& other)
    {
        isEnabled = other.isEnabled;
        isClickThrough = other.isClickThrough;
        isVisible = other.isVisible;
        clipChildren = other.clipChildren;
        rect = other.rect;
        align = other.align;
        xType = other.xType;
        yType = other.yType;
        widthType = other.widthType;
        heightType = other.heightType;
        xAnchorType = other.xAnchorType;
        yAnchorType = other.yAnchorType;
        anchor = other.anchor;
        name = other.name;
        pUserData = other.pUserData;
        pSharedUserData = other.pSharedUserData;
        m_properties = other.m_properties;
        m_style = other.m_style;
        m_styleName = other.m_styleName;
        m_children.clear();

        for (auto pChild : other.m_children)
        {
            add(pChild->copy());
        }
    }

    OUIControlRef UIControl::copy() const
    {
        OUIControlRef pRet;
        switch (getType())
        {
            case Type::Control:
                pRet = UIControl::create();
                break;
            case Type::Button:
                pRet = UIButton::create();
                break;
            case Type::Panel:
                pRet = UIPanel::create();
                break;
            case Type::Label:
                pRet = UILabel::create();
                break;
            case Type::Image:
                pRet = UIImage::create();
                break;
            case Type::CheckBox:
                pRet = UICheckBox::create();
                break;
            case Type::TreeView:
                pRet = UITreeView::create();
                break;
            case Type::TextBox:
                pRet = UITextBox::create();
                break;
            default:
                assert(false);
                break;
        }
        *pRet.get() = *this;
        return pRet;
    }

    void UIControl::add(OUIControlRef pChild)
    {
        if (auto pParent = pChild->m_pParent.lock())
        {
            pParent->remove(pChild);
        }

        pChild->m_pParent = OThis;
        m_children.push_back(pChild);
    }

    void UIControl::insert(OUIControlRef pChild, const OUIControlRef& pBefore)
    {
        if (auto pParent = pChild->m_pParent.lock())
        {
            pParent->remove(pChild);
        }

        size_t i = 0;
        for (auto& pC : m_children)
        {
            if (pC == pBefore) break;
            ++i;
        }

        pChild->m_pParent = OThis;
        m_children.insert(m_children.begin() + i, pChild);
    }

    void UIControl::insertAfter(OUIControlRef pChild, const OUIControlRef& pAfter)
    {
        if (auto pParent = pChild->m_pParent.lock())
        {
            pParent->remove(pChild);
        }

        size_t i = 0;
        for (auto& pC : m_children)
        {
            ++i;
            if (pC == pAfter) break;
        }

        pChild->m_pParent = OThis;
        m_children.insert(m_children.begin() + i, pChild);
    }

    bool UIControl::insertAt(OUIControlRef pChild, size_t index)
    {
        if (index > m_children.size()) return false;

        if (auto pParent = pChild->m_pParent.lock())
        {
            pParent->remove(pChild);
        }

        pChild->m_pParent = OThis;
        m_children.insert(m_children.begin() + index, pChild);
        return true;
    }

    void UIControl::remove()
    {
        if (auto pParent = m_pParent.lock())
        {
            pParent->remove(OThis);
        }
    }

    void UIControl::remove(OUIControlRef in_pChild)
    {
        if (in_pChild->m_pParent.lock().get() != this) return;

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

        in_pChild->m_pParent.reset();
    }

    void UIControl::removeAll()
    {
        auto size = m_children.size();
        for (decltype(size) i = 0; i < size; ++i)
        {
            auto pChild = m_children[i];
            pChild->m_pParent.reset();
        }
        m_children.clear();
    }

    OUIControlRef UIControl::getChild(const std::string& in_name, bool bSearchSubChildren)
    {
        for (auto pChild : m_children)
        {
            if (pChild->name == in_name)
            {
                return pChild;
            }
        }

        if (bSearchSubChildren)
        {
            for (auto pChild : m_children)
            {
                auto pRet = pChild->getChild(in_name, bSearchSubChildren);
                if (pRet)
                {
                    return pRet;
                }
            }
        }

        return nullptr;
    }

    void UIControl::getChild(const OUIContextRef& context,
                             const Vector2& mousePos,
                             bool bSearchSubChildren,
                             bool bIgnoreClickThrough,
                             const Rect& parentRect,
                             OUIControlRef& pHoverControl)
    {
        if (!isVisible || isClickThrough && bIgnoreClickThrough) return;

        Rect worldRect = getWorldRect(parentRect);
        auto itend = m_children.rend();
        for (auto it = m_children.rbegin(); it != itend; ++it)
        {
            auto pChild = *it;
            pChild->getChild(context, mousePos, bSearchSubChildren, bIgnoreClickThrough, worldRect, pHoverControl);
        }

        if (!pHoverControl)
        {
            if (mousePos.x >= worldRect.x &&
                mousePos.y >= worldRect.y &&
                mousePos.x <= worldRect.x + worldRect.z &&
                mousePos.y <= worldRect.y + worldRect.w)
            {
                pHoverControl = OThis;
            }
        }
    }

    OUIControlRef UIControl::getChild(const OUIContextRef& context,
                                      const Vector2& mousePos,
                                      bool bSearchSubChildren,
                                      bool bIgnoreClickThrough)
    {
        Rect parentWorldRect;
        if (getParent())
        {
            parentWorldRect = getParent()->getWorldRect(context);
        }
        else
        {
            parentWorldRect = {{0, 0}, context->getScreenSize()};
        }
        OUIControlRef pHoverControl;
        getChild(context, mousePos, bSearchSubChildren, bIgnoreClickThrough, parentWorldRect, pHoverControl);
        if (pHoverControl.get() == this)
        {
            pHoverControl = nullptr;
        }
        return pHoverControl;
    }

    void UIControl::setStyle(const char* szStyle)
    {
        m_styleName = szStyle;
        m_style = OHash(szStyle);
    }

    bool isObstructed(const OUIContextRef& context, const OUIControlRef& pRoot, const OUIControlRef& pControl, const Rect& worldRect)
    {
        Rect parentRect = {{0, 0}, context->getScreenSize()};
        bool passedUs = false;
        return !pRoot->visitVisible([&worldRect, &passedUs, pControl](const OUIControlRef& pOther, const Rect& rect)
        {
            if (pOther == pControl)
            {
                passedUs = true;
                return true;
            }
            if (!pOther->isClickThrough && passedUs)
            {
                if (rect.x + rect.z >= worldRect.x &&
                    rect.x <= worldRect.x + worldRect.z &&
                    rect.y + rect.w >= worldRect.y &&
                    rect.y <= worldRect.y + worldRect.w)
                {
                    return false;
                }
            }
            return true;
        }, parentRect);
    }

    bool isReallyVisible(const OUIControlRef& pControl)
    {
        if (!pControl) return true;
        return pControl->isVisible && isReallyVisible(pControl->getParent());
    }

    bool isReallyEnabled(const OUIControlRef& pControl)
    {
        if (!pControl) return true;
        return pControl->isEnabled && isReallyEnabled(pControl->getParent());
    }

    void UIControl::update(const OUIContextRef& context, const Vector2& mousePos, bool bMouse1Down, bool bMouse2Down, bool bMouse3Down, bool bNavL, bool bNavR, bool bNavU, bool bNavD, bool bControl, float scroll)
    {
        // Prepare our data
        Rect parentRect = {{0, 0}, context->getScreenSize()};
        context->m_mouseEvents[0].mousePos = mousePos;
        context->m_mouseEvents[0].isMouseDown = bMouse1Down;
        context->m_mouseEvents[0].pContext = context;
        context->m_mouseEvents[0].button = 1;
        context->m_mouseEvents[0].isCtrlDown = bControl;
        context->m_mouseEvents[0].scroll = scroll;
        context->m_mouseEvents[1].mousePos = mousePos;
        context->m_mouseEvents[1].isMouseDown = bMouse2Down;
        context->m_mouseEvents[1].pContext = context;
        context->m_mouseEvents[1].button = 2;
        context->m_mouseEvents[1].isCtrlDown = bControl;
        context->m_mouseEvents[1].scroll = scroll;
        context->m_mouseEvents[2].mousePos = mousePos;
        context->m_mouseEvents[2].isMouseDown = bMouse3Down;
        context->m_mouseEvents[2].pContext = context;
        context->m_mouseEvents[2].button = 3;
        context->m_mouseEvents[2].isCtrlDown = bControl;
        context->m_mouseEvents[2].scroll = scroll;
        context->m_pHoverControl = nullptr;

        // Update UIs
        if (context->useNavigation && context->m_pLastHoverControl)
        {
            if (!isReallyEnabled(context->m_pLastHoverControl) ||
                !isReallyVisible(context->m_pLastHoverControl) ||
                context->m_pLastHoverControl->isClickThrough ||
                isObstructed(context, OThis, context->m_pLastHoverControl, context->m_pLastHoverControl->getWorldRect(context)))
            {
                context->m_pLastHoverControl = nullptr;
            }
        }
        if (context->useNavigation)
        {
            if (!context->m_pLastHoverControl)
            {
                // Find the first unobstructed navigable
                visitChildrenFirstEnabled([this, &context](const OUIControlRef& pControl, const Rect& rect) -> bool
                {
                    if (!pControl->isNavigatable()) return true;
                    if (!isObstructed(context, OThis, pControl, rect))
                    {
                        context->m_pHoverControl = pControl;
                        return false;
                    }
                    return true;
                }, parentRect);
            }
        }
        else
        {
            updateInternal(context, parentRect);
        }
        if (context->useNavigation)
        {
            if (!context->m_pHoverControl)
            {
                context->m_pHoverControl = context->m_pLastHoverControl;
            }
            if (context->m_pHoverControl &&
                (bNavR || bNavL || bNavU || bNavD))
            {
                auto worldRect = context->m_pHoverControl->getWorldRect(context);
                auto pPreviousHover = context->m_pHoverControl;
                float closest = 10000.f;
                float closestH = 10000.f;

                // Navigation
                if (bNavR)
                {
                    // Find closest down navigable
                    visitChildrenFirstEnabled([this, pPreviousHover, &context, &worldRect, &closest](const OUIControlRef& pControl, const Rect& rect) -> bool
                    {
                        if (!pControl->isNavigatable()) return true;
                        if (pControl == pPreviousHover) return true;
                        if (rect.y > worldRect.y + worldRect.w) return true;
                        if (rect.y + rect.w < worldRect.y) return true;
                        float distance = rect.x - worldRect.x;
                        if (distance < closest && distance > 0.f)
                        {
                            if (!isObstructed(context, OThis, pControl, rect))
                            {
                                closest = distance;
                                context->m_pHoverControl = pControl;
                            }
                        }
                        return true;
                    }, parentRect);
                }
                else if (bNavL)
                {
                    // Find closest down navigable
                    visitChildrenFirstEnabled([this, pPreviousHover, &context, &worldRect, &closest](const OUIControlRef& pControl, const Rect& rect) -> bool
                    {
                        if (!pControl->isNavigatable()) return true;
                        if (pControl == pPreviousHover) return true;
                        if (rect.y > worldRect.y + worldRect.w) return true;
                        if (rect.y + rect.w < worldRect.y) return true;
                        float distance = worldRect.x - rect.x;
                        if (distance < closest && distance > 0.f)
                        {
                            if (!isObstructed(context, OThis, pControl, rect))
                            {
                                closest = distance;
                                context->m_pHoverControl = pControl;
                            }
                        }
                        return true;
                    }, parentRect);
                }
                else if (bNavD)
                {
                    // Find closest down navigable
                    visitChildrenFirstEnabled([this, pPreviousHover, &context, &worldRect, &closest, &closestH](const OUIControlRef& pControl, const Rect& rect) -> bool
                    {
                        if (!pControl->isNavigatable()) return true;
                        if (pControl == pPreviousHover) return true;
                        float distance = rect.y - (worldRect.y + worldRect.w);
                        float distanceH = std::abs(rect.x - worldRect.x);
                        if ((distance < closest - 16.f && distance > 0.f) ||
                            (distance < closest + 16.f && distance > 0.f && distanceH < closestH))
                        {
                            if (!isObstructed(context, OThis, pControl, rect))
                            {
                                closest = distance;
                                closestH = distanceH;
                                context->m_pHoverControl = pControl;
                            }
                        }
                        return true;
                    }, parentRect);
                }
                else if (bNavU)
                {
                    // Find closest down navigable
                    visitChildrenFirstEnabled([this, pPreviousHover, &context, &worldRect, &closest, &closestH](const OUIControlRef& pControl, const Rect& rect) -> bool
                    {
                        if (!pControl->isNavigatable()) return true;
                        if (pControl == pPreviousHover) return true;
                        float distance = worldRect.y - (rect.y + rect.w);
                        float distanceH = std::abs(rect.x - worldRect.x);
                        if ((distance < closest - 16.f && distance > 0.f) ||
                            (distance < closest + 16.f && distance > 0.f && distanceH < closestH))
                        {
                            if (!isObstructed(context, OThis, pControl, rect))
                            {
                                closest = distance;
                                closestH = distanceH;
                                context->m_pHoverControl = pControl;
                            }
                        }
                        return true;
                    }, parentRect);
                }
            }
        }

        // Resolve
        context->resolve();

        // Send events
        context->dispatchEvents();

        // Reset for next frame
        context->reset();
    }

    void UIControl::render(const OUIContextRef& context)
    {
        Rect parentRect = {{0, 0}, context->getScreenSize()};
        renderInternal(context, parentRect);
    }

    Rect UIControl::getWorldRect(const OUIContextRef& context) const
    {
        if (auto pParent = m_pParent.lock())
        {
            return std::move(getWorldRect(pParent->getWorldRect(context)));
        }
        else
        {
            Rect parentRect = {{0, 0}, context->getScreenSize()};
            return std::move(getWorldRect(parentRect));
        }
    }

    Rect UIControl::getWorldRect(const Rect& parentRect) const
    {
        Rect worldRect;

        // Generate position and width values first
        switch (xType)
        {
            case PosType::Relative:
                worldRect.x = rect.x;
                break;
            case PosType::Percentage:
                worldRect.x = parentRect.z * rect.x;
                break;
        }
        switch (yType)
        {
            case PosType::Relative:
                worldRect.y = rect.y;
                break;
            case PosType::Percentage:
                worldRect.y = parentRect.w * rect.y;
                break;
        }
        switch (widthType)
        {
            case DimType::Absolute:
                worldRect.z = rect.z;
                break;
            case DimType::Relative:
                worldRect.z = parentRect.z + rect.z;
                break;
            case DimType::Percentage:
                worldRect.z = parentRect.z * rect.z;
                break;
        }
        switch (heightType)
        {
            case DimType::Absolute:
                worldRect.w = rect.w;
                break;
            case DimType::Relative:
                worldRect.w = parentRect.w + rect.w;
                break;
            case DimType::Percentage:
                worldRect.w = parentRect.w * rect.w;
                break;
        }

        // Then do alignement
        switch (align)
        {
            case onut::Align::TopLeft:
                worldRect.x = parentRect.x + worldRect.x;
                worldRect.y = parentRect.y + worldRect.y;
                break;
            case onut::Align::Top:
                worldRect.x = parentRect.x + parentRect.z * .5f + worldRect.x;
                worldRect.y = parentRect.y + worldRect.y;
                break;
            case onut::Align::TopRight:
                worldRect.x = parentRect.x + parentRect.z + worldRect.x;
                worldRect.y = parentRect.y + worldRect.y;
                break;
            case onut::Align::Left:
                worldRect.x = parentRect.x + worldRect.x;
                worldRect.y = parentRect.y + parentRect.w * .5f + worldRect.y;
                break;
            case onut::Align::Center:
                worldRect.x = parentRect.x + parentRect.z * .5f + worldRect.x;
                worldRect.y = parentRect.y + parentRect.w * .5f + worldRect.y;
                break;
            case onut::Align::Right:
                worldRect.x = parentRect.x + parentRect.z + worldRect.x;
                worldRect.y = parentRect.y + parentRect.w * .5f + worldRect.y;
                break;
            case onut::Align::BottomLeft:
                worldRect.x = parentRect.x + worldRect.x;
                worldRect.y = parentRect.y + parentRect.w + worldRect.y;
                break;
            case onut::Align::Bottom:
                worldRect.x = parentRect.x + parentRect.z * .5f + worldRect.x;
                worldRect.y = parentRect.y + parentRect.w + worldRect.y;
                break;
            case onut::Align::BottomRight:
                worldRect.x = parentRect.x + parentRect.z + worldRect.x;
                worldRect.y = parentRect.y + parentRect.w + worldRect.y;
                break;
        }

        // Finally anchoring
        switch (xAnchorType)
        {
            case AnchorType::Pixel:
                worldRect.x -= anchor.x;
                break;
            case AnchorType::Percentage:
                worldRect.x -= worldRect.z * anchor.x;
                break;
        }
        switch (yAnchorType)
        {
            case AnchorType::Pixel:
                worldRect.y -= anchor.y;
                break;
            case AnchorType::Percentage:
                worldRect.y -= worldRect.w * anchor.y;
                break;
        }

        // Snap to pixels
        worldRect.x = std::roundf(worldRect.x);
        worldRect.y = std::roundf(worldRect.y);
        worldRect.w = std::roundf(worldRect.w);
        worldRect.z = std::roundf(worldRect.z);

        return std::move(worldRect);
    }

    void UIControl::updateInternal(const OUIContextRef& context, const Rect& parentRect)
    {
        if (!isEnabled || !isVisible) return;
        Rect worldRect = getWorldRect(parentRect);

        // Do children first, inverted
        if (context->useNavigation)
        {
            for (auto pChild : m_children)
            {
                pChild->updateInternal(context, worldRect);
            }
        }
        else
        {
            auto itend = m_children.rend();
            for (auto it = m_children.rbegin(); it != itend; ++it)
            {
                auto pChild = *it;
                pChild->updateInternal(context, worldRect);
            }
        }

        if (!context->m_pHoverControl && !isClickThrough)
        {
            auto &mouseEvt = context->m_mouseEvents[0];
            if (mouseEvt.mousePos.x >= worldRect.x &&
                mouseEvt.mousePos.y >= worldRect.y &&
                mouseEvt.mousePos.x <= worldRect.x + worldRect.z &&
                mouseEvt.mousePos.y <= worldRect.y + worldRect.w)
            {
                context->m_mouseEvents[0].localMousePos.x = mouseEvt.mousePos.x - worldRect.x;
                context->m_mouseEvents[0].localMousePos.y = mouseEvt.mousePos.y - worldRect.y;
                context->m_mouseEvents[1].localMousePos = context->m_mouseEvents[0].localMousePos;
                context->m_mouseEvents[2].localMousePos = context->m_mouseEvents[0].localMousePos;
                context->m_pHoverControl = OThis;
            }
        }
    }

    void UIControl::renderInternal(const OUIContextRef& context, const Rect& parentRect)
    {
        if (!isVisible) return;

        Rect worldRect = getWorldRect(parentRect);
        if (clipChildren)
        {
            context->pushClip(worldRect);
        }
        renderControl(context, worldRect);
        for (auto pChild : m_children)
        {
            pChild->renderInternal(context, worldRect);
        }
        if (clipChildren)
        {
            context->popClip();
        }
    }

    void UIControl::setWorldRect(const Rect& in_rect, const OUIContextRef& context)
    {
        auto pParent = getParent();
        if (pParent)
        {
            auto parentRect = pParent->getWorldRect(context);

            Rect localRect = in_rect;

            // Undo anchoring
            switch (xAnchorType)
            {
                case AnchorType::Pixel:
                    localRect.x += anchor.x;
                    break;
                case AnchorType::Percentage:
                    localRect.x += localRect.z * anchor.x;
                    break;
            }
            switch (yAnchorType)
            {
                case AnchorType::Pixel:
                    localRect.y += anchor.y;
                    break;
                case AnchorType::Percentage:
                    localRect.y += localRect.w * anchor.y;
                    break;
            }

            // Undo alignement
            switch (align)
            {
                case onut::Align::TopLeft:
                    localRect.x = localRect.x - parentRect.x;
                    localRect.y = localRect.y - parentRect.y;
                    break;
                case onut::Align::Top:
                    localRect.x = localRect.x - parentRect.x - parentRect.z * .5f;
                    localRect.y = localRect.y - parentRect.y;
                    break;
                case onut::Align::TopRight:
                    localRect.x = localRect.x - parentRect.x - parentRect.z;
                    localRect.y = localRect.y - parentRect.y;
                    break;
                case onut::Align::Left:
                    localRect.x = localRect.x - parentRect.x;
                    localRect.y = localRect.y - parentRect.y - parentRect.w * .5f;
                    break;
                case onut::Align::Center:
                    localRect.x = localRect.x - parentRect.x - parentRect.z * .5f;
                    localRect.y = localRect.y - parentRect.y - parentRect.w * .5f;
                    break;
                case onut::Align::Right:
                    localRect.x = localRect.x - parentRect.x - parentRect.z;
                    localRect.y = localRect.y - parentRect.y - parentRect.w * .5f;
                    break;
                case onut::Align::BottomLeft:
                    localRect.x = localRect.x - parentRect.x;
                    localRect.y = localRect.y - parentRect.y - parentRect.w;
                    break;
                case onut::Align::Bottom:
                    localRect.x = localRect.x - parentRect.x - parentRect.z * .5f;
                    localRect.y = localRect.y - parentRect.y - parentRect.w;
                    break;
                case onut::Align::BottomRight:
                    localRect.x = localRect.x - parentRect.x - parentRect.z;
                    localRect.y = localRect.y - parentRect.y - parentRect.w;
                    break;
            }

            // Undo position and width values
            switch (xType)
            {
                case PosType::Relative:
                    localRect.x = localRect.x;
                    break;
                case PosType::Percentage:
                    localRect.x = localRect.x / parentRect.z;
                    break;
            }
            switch (yType)
            {
                case PosType::Relative:
                    localRect.y = localRect.y;
                    break;
                case PosType::Percentage:
                    localRect.y = localRect.y / parentRect.w;
                    break;
            }
            switch (widthType)
            {
                case DimType::Absolute:
                    localRect.z = localRect.z;
                    break;
                case DimType::Relative:
                    localRect.z = localRect.z - parentRect.z;
                    break;
                case DimType::Percentage:
                    localRect.z = localRect.z / parentRect.z;
                    break;
            }
            switch (heightType)
            {
                case DimType::Absolute:
                    localRect.w = localRect.w;
                    break;
                case DimType::Relative:
                    localRect.w = localRect.w - parentRect.w;
                    break;
                case DimType::Percentage:
                    localRect.w = localRect.w / parentRect.w;
                    break;
            }

            rect = std::move(localRect);
        }
        else
        {
            rect = in_rect;
        }
    }

    Vector2 UIControl::getAnchorInPixel() const
    {
        Vector2 ret = anchor;
        if (xAnchorType == AnchorType::Percentage)
        {
            ret.x = ret.x * rect.z;
        }
        if (yAnchorType == AnchorType::Percentage)
        {
            ret.y = ret.y * rect.w;
        }
        return std::move(ret);
    }

    Vector2 UIControl::getAnchorInPercentage() const
    {
        Vector2 ret = anchor;
        if (xAnchorType == AnchorType::Pixel)
        {
            ret.x = ret.x / rect.z;
        }
        if (yAnchorType == AnchorType::Pixel)
        {
            ret.y = ret.y / rect.w;
        }
        return std::move(ret);
    }

    void UIControl::setAnchorPercent(const Vector2& in_anchor)
    {
        if (xAnchorType == AnchorType::Pixel)
        {
            anchor.x = rect.z * in_anchor.x;
        }
        else
        {
            anchor.x = in_anchor.x;
        }
        if (yAnchorType == AnchorType::Pixel)
        {
            anchor.y = rect.w * in_anchor.y;
        }
        else
        {
            anchor.y = in_anchor.y;
        }
    }

    UIControl::State UIControl::getState(const OUIContextRef& context) const
    {
        if (isEnabled)
        {
            if (context->m_pHoverControl.get() == this)
            {
                if (context->m_pDownControls[0].get() == this)
                {
                    return State::Down;
                }
                else
                {
                    return State::Hover;
                }
            }
            else if (context->m_pDownControls[0].get() == this)
            {
                return State::Hover;
            }
            else
            {
                return State::Normal;
            }
        }
        else
        {
            return State::Disabled;
        }
    }

    bool UIControl::hasFocus(const OUIContextRef& context) const
    {
        return (context->m_pFocus.get() == this);
    }

    const UIControl::Property& UIControl::getProperty(const std::string& name) const
    {
        auto& it = m_properties.find(name);
        return it->second;
    }
};

OUIControlRef OFindUI(const std::string& name)
{
    return oUI->getChild(name);
}

OUIControlRef OLoadUI(const std::string& filename)
{
    return OUIControl::createFromFile(filename);
}
