#include "UI.h"
#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <algorithm>
#include <functional>
#include <sstream>
#include <iomanip>

namespace onut
{
    // Strings to Enums used for loading/saving
    static std::unordered_map<std::string, sUIFont::eFontFlag> fontFlagMap = {
        {"AUTO_FIT_SIZE", sUIFont::AUTO_FIT_SIZE},
        {"ELLIPSIS", sUIFont::ELLIPSIS},
        {"WORD_WRAP", sUIFont::WORD_WRAP}
    };
    static std::unordered_map<std::string, eUIAlign> alignMap = {
        {"TOP_LEFT", eUIAlign::TOP_LEFT},
        {"TOP", eUIAlign::TOP},
        {"TOP_RIGHT", eUIAlign::TOP_RIGHT},
        {"LEFT", eUIAlign::LEFT},
        {"CENTER", eUIAlign::CENTER},
        {"RIGHT", eUIAlign::RIGHT},
        {"BOTTOM_LEFT", eUIAlign::BOTTOM_LEFT},
        {"BOTTOM", eUIAlign::BOTTOM},
        {"BOTTOM_RIGHT", eUIAlign::BOTTOM_RIGHT}
    };
    static std::unordered_map<std::string, eUIDimType> dimTypeMap = {
        {"ABSOLUTE", eUIDimType::DIM_ABSOLUTE},
        {"RELATIVE", eUIDimType::DIM_RELATIVE},
        {"PERCENTAGE", eUIDimType::DIM_PERCENTAGE}
    };
    static std::unordered_map<std::string, eUIPosType> posTypeMap = {
        {"RELATIVE", eUIPosType::POS_RELATIVE},
        {"PERCENTAGE", eUIPosType::POS_PERCENTAGE}
    };
    static std::unordered_map<std::string, eUIAnchorType> anchorTypeMap = {
        {"PIXEL", eUIAnchorType::ANCHOR_PIXEL},
        {"PERCENTAGE", eUIAnchorType::ANCHOR_PERCENTAGE}
    };
    static std::unordered_map<std::string, eUICheckBehavior> checkBehaviorMap = {
        {"NORMAL", eUICheckBehavior::NORMAL},
        {"OPTIONAL", eUICheckBehavior::CHK_OPTIONAL},
        {"EXCLUSIVE", eUICheckBehavior::EXCLUSIVE}
    };
    static std::unordered_map<std::string, eUIType> typeMap = {
        {"UIControl", eUIType::UI_CONTROL},
        {"UIButton", eUIType::UI_BUTTON},
        {"UIPanel", eUIType::UI_PANEL},
        {"UILabel", eUIType::UI_LABEL},
        {"UIImage", eUIType::UI_IMAGE},
        {"UICheckBox", eUIType::UI_CHECKBOX},
        {"UITreeView", eUIType::UI_TREEVIEW},
        {"UITextBox", eUIType::UI_TEXTBOX},
    };

    // Convert string to enum. Used for loading
    template<typename Tmap, typename Tenum>
    Tenum stringToEnum(const Tmap& map, const std::string& key, Tenum default)
    {
        auto it = map.find(key);
        if (it == map.end()) return default;
        return it->second;
    }

    // Convert enum to string. Used for saving. A bit slower
    template<typename Tmap, typename Tenum>
    const std::string& enumToString(const Tmap& map, Tenum value)
    {
        for (auto& kv : map)
        {
            if (kv.second == value)
            {
                return kv.first;
            }
        }
        static std::string empty;
        return empty;
    }

    void sUIColor::unpack()
    {
        r = (float)((packed >> 24) & 0xff) / 255.f;
        g = (float)((packed >> 16) & 0xff) / 255.f;
        b = (float)((packed >> 8) & 0xff) / 255.f;
        a = (float)(packed & 0xff) / 255.f;
    }

    void sUIColor::pack()
    {
        packed = 0;
        packed |= ((uint32_t)(r * 255.f) << 24) & 0xff000000;
        packed |= ((uint32_t)(g * 255.f) << 16) & 0x00ff0000;
        packed |= ((uint32_t)(b * 255.f) << 8) & 0x0000ff00;
        packed |= (uint32_t)(a * 255.f) & 0x000000ff;
    }

    unsigned int uiHash(const char* s, unsigned int seed)
    {
        unsigned hash = seed;
        while (*s) hash = hash * 101 + *s++;
        return hash;
    }

    static const char* getJsonString(const rapidjson::Value& jsonNode, const char* szDefault = "")
    {
        if (jsonNode.IsString()) return jsonNode.GetString();
        else return szDefault;
    }
    
    static float getJsonFloat(const rapidjson::Value& jsonNode, const float default = 0.f)
    {
        if (jsonNode.IsNumber()) return static_cast<float>(jsonNode.GetDouble());
        else return default;
    }
    
    static int getJsonInt(const rapidjson::Value& jsonNode, const int default = 0)
    {
        if (jsonNode.IsInt()) return jsonNode.GetInt();
        else return default;
    }

    static bool getJsonBool(const rapidjson::Value& jsonNode, const bool default = false)
    {
        if (jsonNode.IsBool()) return jsonNode.GetBool();
        else return default;
    }

    static void setJsonFloat(rapidjson::Value& jsonNode, const char* szName, float value, rapidjson::Allocator& allocator, float default = 0.f)
    {
        if (value == default) return;
        jsonNode.AddMember(szName, value, allocator);
    }

    static void setJsonInt(rapidjson::Value& jsonNode, const char* szName, int value, rapidjson::Allocator& allocator, int default = 0)
    {
        if (value == default) return;
        jsonNode.AddMember(szName, value, allocator);
    }

    static void setJsonString(rapidjson::Value& jsonNode, const char* szName, const char* szValue, rapidjson::Allocator& allocator, const char* default = "")
    {
        if (!strcmp(szValue, default)) return;
        jsonNode.AddMember(szName, szValue, allocator);
    }

    static void setJsonString(rapidjson::Value& jsonNode, const char* szName, const std::string& value, rapidjson::Allocator& allocator, const char* default = "")
    {
        if (!strcmp(value.c_str(), default)) return;
        rapidjson::Value jsonValue;
        jsonValue.SetString(value.c_str(), allocator);
        jsonNode.AddMember(szName, jsonValue, allocator);
    }

    static void setJsonBool(rapidjson::Value& jsonNode, const char* szName, const bool value, rapidjson::Allocator& allocator, bool default = false)
    {
        if (value == default) return;
        jsonNode.AddMember(szName, value, allocator);
    }

    static void setJsonColor(rapidjson::Value& jsonNode, 
                             const char* szName, 
                             const sUIColor& value, 
                             rapidjson::Allocator& allocator, 
                             const sUIColor& default = {1.f, 1.f, 1.f, 1.f})
    {
        if (value.packed == default.packed) return;

        rapidjson::Value jsonValue;
        char buffer[10];
        int len = sprintf_s(buffer, "%08x", value.packed); // dynamically created string.
        jsonValue.SetString(buffer, len, allocator);
        jsonNode.AddMember(szName, jsonValue, allocator);
    }

    static sUIColor getJsonColor(const rapidjson::Value& jsonNode, const sUIColor& default = {1.f, 1.f, 1.f, 1.f})
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

    static sUIPadding getJsonPadding(const rapidjson::Value& node)
    {
        sUIPadding ret;
        if (!node.IsNull())
        {
            ret.left = getJsonFloat(node["left"]);
            ret.right = getJsonFloat(node["right"]);
            ret.top = getJsonFloat(node["top"]);
            ret.bottom = getJsonFloat(node["bottom"]);
        }
        return std::move(ret);
    }

    static void setJsonPadding(rapidjson::Value& node, const sUIPadding& padding, rapidjson::Allocator& allocator)
    {
        rapidjson::Value paddingNode(rapidjson::kObjectType);
        setJsonFloat(paddingNode, "left", padding.left, allocator);
        setJsonFloat(paddingNode, "right", padding.right, allocator);
        setJsonFloat(paddingNode, "top", padding.top, allocator);
        setJsonFloat(paddingNode, "bottom", padding.bottom, allocator);
        node.AddMember("padding", paddingNode, allocator);
    }

    template <typename Tmap, typename Tenum>
    static Tenum getJsonEnum(const Tmap& enumMap, const rapidjson::Value& jsonNode, Tenum default)
    {
        Tenum ret = default;
        if (jsonNode.IsString())
        {
            ret = stringToEnum(enumMap, jsonNode.GetString(), default);
        }
        return ret;
    }

    template <typename Tmap, typename Tenum = Tmap::mapped_type>
    static Tenum getJsonBitmask(const Tmap& enumMap, const rapidjson::Value& jsonNode)
    {
        uint32_t ret = 0;
        if (jsonNode.IsArray())
        {
            for (decltype(jsonNode.Size()) i = 0; i < jsonNode.Size(); ++i)
            {
                auto& jsonEnum = jsonNode[i];
                if (jsonEnum.IsString())
                {
                    ret |= stringToEnum(enumMap, jsonEnum.GetString(), (Tenum)0);
                }
            }
        }
        else if (jsonNode.IsString())
        {
            ret = stringToEnum(enumMap, jsonNode.GetString(), (Tenum)0);
        }
        return (Tenum)ret;
    }

    template <typename Tmap, typename Tenum = Tmap::mapped_type>
    static void setJsonBitmask(rapidjson::Value& node, const char* szName, const Tmap& enumMap, Tenum value, rapidjson::Allocator& allocator)
    {
        if (value == 0) return;
        std::vector<std::string> ret;
        for (auto& kv : enumMap)
        {
            if (value & kv.second)
            {
                ret.push_back(kv.first);
            }
        }
        if (ret.size() == 1)
        {
            setJsonString(node, szName, ret.front(), allocator);
        }
        else if (!ret.empty())
        {
            rapidjson::Value arr(rapidjson::kArrayType);
            for (auto& str : ret)
            {
                rapidjson::Value strValue;
                strValue.SetString(str.c_str(), allocator);
                arr.PushBack(strValue, allocator);
            }
            node.AddMember(szName, arr, allocator);
        }
    }

    static sUIFont getJsonFont(const rapidjson::Value& node)
    {
        sUIFont ret;
        if (!node.IsNull())
        {
            ret.color = getJsonColor(node["color"]);
            ret.align = getJsonEnum(alignMap, node["align"], eUIAlign::TOP_LEFT);
            ret.padding = getJsonPadding(node["padding"]);
            ret.typeFace = getJsonString(node["typeFace"], "Arial");
            ret.size = getJsonFloat(node["size"], 12.f);
            ret.flags = getJsonBitmask(fontFlagMap, node["flags"]);
            ret.minSize = getJsonFloat(node["minSize"], 12.f);
        }
        return std::move(ret);
    }

    static void setJsonFont(rapidjson::Value& node, const sUIFont& font, rapidjson::Allocator& allocator)
    {
        rapidjson::Value fontNode(rapidjson::kObjectType);
        setJsonColor(fontNode, "color", font.color, allocator);
        setJsonString(fontNode, "align", enumToString(alignMap, font.align), allocator, "TOP_LEFT");
        setJsonPadding(fontNode, font.padding, allocator);
        setJsonString(fontNode, "typeFace", font.typeFace, allocator, "Arial");
        setJsonFloat(fontNode, "size", font.size, allocator, 12.f);
        setJsonBitmask(fontNode, "flags", fontFlagMap, font.flags, allocator);
        setJsonFloat(fontNode, "minSize", font.minSize, allocator, 12.f);
        node.AddMember("font", fontNode, allocator);
    }

    static sUITextComponent getJsonTextComponent(const rapidjson::Value& node)
    {
        sUITextComponent ret;
        if (!node.IsNull())
        {
            ret.text = getJsonString(node["text"]);
            ret.font = getJsonFont(node["font"]);
        }
        return std::move(ret);
    }

    static sUIImageComponent getJsonImageComponent(const rapidjson::Value& node)
    {
        sUIImageComponent ret;
        if (!node.IsNull())
        {
            ret.filename = getJsonString(node["filename"]);
            ret.color = getJsonColor(node["color"]);
        }
        return std::move(ret);
    }

    static sUIScale9Component getJsonScale9Component(const rapidjson::Value& node)
    {
        sUIScale9Component scale9Component;
        if (node.HasMember("scale9Component"))
        {
            auto& scale9Node = node["scale9Component"];
            scale9Component.isScaled9 = true;
            scale9Component.padding = getJsonPadding(scale9Node["padding"]);
            scale9Component.image = getJsonImageComponent(scale9Node["imageComponent"]);
            scale9Component.isRepeat = getJsonBool(scale9Node["repeat"]);
        }
        else if (node.HasMember("imageComponent"))
        {
            scale9Component.image = getJsonImageComponent(node["imageComponent"]);
        }
        else if (node.HasMember("image"))
        {
            scale9Component.image.filename = getJsonString(node["image"]);
        }
        return std::move(scale9Component);
    }

    static void setJsonImageComponent(rapidjson::Value& node, const sUIImageComponent& imageComponent, rapidjson::Allocator& allocator)
    {
        rapidjson::Value imageNode(rapidjson::kObjectType);
        setJsonString(imageNode, "filename", imageComponent.filename, allocator);
        setJsonColor(imageNode, "color", imageComponent.color, allocator);
        node.AddMember("imageComponent", imageNode, allocator);
    }

    static void setJsonScale9Component(rapidjson::Value& node, const sUIScale9Component& scale9Component, rapidjson::Allocator& allocator)
    {
        if (scale9Component.isScaled9)
        {
            rapidjson::Value scale9Node(rapidjson::kObjectType);
            setJsonPadding(scale9Node, scale9Component.padding, allocator);
            setJsonImageComponent(scale9Node, scale9Component.image, allocator);
            setJsonBool(scale9Node, "repeat", scale9Component.isRepeat, allocator);
            node.AddMember("scale9Component", scale9Node, allocator);
            return;
        }
        sUIColor white;
        if (scale9Component.image.color.r != white.r ||
            scale9Component.image.color.g != white.g ||
            scale9Component.image.color.b != white.b ||
            scale9Component.image.color.a != white.a)
        {
            setJsonImageComponent(node, scale9Component.image, allocator);
        }
        else
        {
            setJsonString(node, "image", scale9Component.image.filename, allocator);
        }
    }

    static void setJsonTextComponent(rapidjson::Value& node, const sUITextComponent& textComponent, rapidjson::Allocator& allocator)
    {
        rapidjson::Value textNode(rapidjson::kObjectType);
        setJsonString(textNode, "text", textComponent.text, allocator);
        setJsonFont(textNode, textComponent.font, allocator);
        node.AddMember("textComponent", textNode, allocator);
    }

    UIContext::UIContext(const sUIVector2& screenSize) :
        m_screenSize(screenSize)
    {
        m_pDownControls[0] = nullptr;
        m_pDownControls[1] = nullptr;
        m_pDownControls[2] = nullptr;
        m_pLastDownControls[0] = nullptr;
        m_pLastDownControls[1] = nullptr;
        m_pLastDownControls[2] = nullptr;
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

    void UIContext::pushClip(const sUIRect& rect)
    {
        m_clips.push_back(rect);
        onClipping(true, rect);
    }

    void UIContext::popClip()
    {
        m_clips.pop_back();
        if (m_clips.empty())
        {
            onClipping(false, {{0, 0}, m_screenSize});
        }
        else
        {
            onClipping(true, m_clips.back());
        }
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

    bool UIControl::visit(const std::function<bool(UIControl*, const sUIRect&)>& callback, const sUIRect& parentRect)
    {
        auto worldRect = getWorldRect(parentRect);
        if (!callback(this, worldRect))
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

    bool UIControl::visitChildrenFirst(const std::function<bool(UIControl*, const sUIRect&)>& callback, const sUIRect& parentRect)
    {
        auto worldRect = getWorldRect(parentRect);
        for (auto& pChild : m_children)
        {
            if (!pChild->visitChildrenFirst(callback, worldRect))
            {
                return false;
            }
        }
        if (!callback(this, worldRect))
        {
            return false;
        }
        return true;
    }

    bool UIControl::visitEnabled(const std::function<bool(UIControl*, const sUIRect&)>& callback, const sUIRect& parentRect)
    {
        if (!isEnabled) return true;
        if (!isVisible) return true;
        auto worldRect = getWorldRect(parentRect);
        if (!callback(this, worldRect))
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

    bool UIControl::visitChildrenFirstEnabled(const std::function<bool(UIControl*, const sUIRect&)>& callback, const sUIRect& parentRect)
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
        if (!callback(this, worldRect))
        {
            return false;
        }
        return true;
    }

    bool UIControl::visitVisible(const std::function<bool(UIControl*, const sUIRect&)>& callback, const sUIRect& parentRect)
    {
        if (!isVisible) return true;
        auto worldRect = getWorldRect(parentRect);
        if (!callback(this, worldRect))
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

    bool UIControl::visitChildrenFirstVisible(const std::function<bool(UIControl*, const sUIRect&)>& callback, const sUIRect& parentRect)
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
        if (!callback(this, worldRect))
        {
            return false;
        }
        return true;
    }

    void UIControl::load(const rapidjson::Value& jsonNode)
    {
        rect.position.x = getJsonFloat(jsonNode["x"]);
        rect.position.y = getJsonFloat(jsonNode["y"]);
        rect.size.x = getJsonFloat(jsonNode["width"], 0.f);
        rect.size.y = getJsonFloat(jsonNode["height"], 0.f);
        anchor.x = getJsonFloat(jsonNode["xAnchor"]);
        anchor.y = getJsonFloat(jsonNode["yAnchor"]);

        align = getJsonEnum(alignMap, jsonNode["align"], eUIAlign::TOP_LEFT);
        xType = getJsonEnum(posTypeMap, jsonNode["xType"], eUIPosType::POS_RELATIVE);
        yType = getJsonEnum(posTypeMap, jsonNode["yType"], eUIPosType::POS_RELATIVE);
        widthType = getJsonEnum(dimTypeMap, jsonNode["widthType"], eUIDimType::DIM_ABSOLUTE);
        heightType = getJsonEnum(dimTypeMap, jsonNode["heightType"], eUIDimType::DIM_ABSOLUTE);
        xAnchorType = getJsonEnum(anchorTypeMap, jsonNode["anchorType"], eUIAnchorType::ANCHOR_PERCENTAGE);
        yAnchorType = getJsonEnum(anchorTypeMap, jsonNode["anchorType"], eUIAnchorType::ANCHOR_PERCENTAGE);

        name = getJsonString(jsonNode["name"]);
        m_styleName = getJsonString(jsonNode["style"]);
        m_style = uiHash(m_styleName.c_str());

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
                else if (!strcmp(jsonChildType, "UITextBox"))
                {
                    pChild = new UITextBox();
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

        setJsonFloat(jsonNode, "x", rect.position.x, allocator);
        setJsonFloat(jsonNode, "y", rect.position.y, allocator);
        setJsonFloat(jsonNode, "width", rect.size.x, allocator);
        setJsonFloat(jsonNode, "height", rect.size.y, allocator);
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
                case eUIPropertyType::P_BOOL:
                    jsonNode[kv.first.c_str()] = kv.second.getBool();
                    break;
                case eUIPropertyType::P_FLOAT:
                    jsonNode[kv.first.c_str()] = kv.second.getFloat();
                    break;
                case eUIPropertyType::P_INT:
                    jsonNode[kv.first.c_str()] = kv.second.getInt();
                    break;
                case eUIPropertyType::P_STRING:
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

    UIControl::UIControl(const UIControl& other) :
        UIControl()
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
        m_properties = other.m_properties;
        m_style = other.m_style;
        m_styleName = other.m_styleName;

        for (auto pChild : other.m_children)
        {
            add(pChild->copy());
        }
    }

    UIControl* UIControl::copy() const
    {
        switch (getType())
        {
            case eUIType::UI_CONTROL:
                return (new UIControl(*this));
            case eUIType::UI_BUTTON:
                return (new UIButton(*(UIButton*)this));
            case eUIType::UI_PANEL:
                return (new UIPanel(*(UIPanel*)this));
            case eUIType::UI_LABEL:
                return (new UILabel(*(UILabel*)this));
            case eUIType::UI_IMAGE:
                return (new UIImage(*(UIImage*)this));
            case eUIType::UI_CHECKBOX:
                return (new UICheckBox(*(UICheckBox*)this));
            case eUIType::UI_TREEVIEW:
                return (new UITreeView(*(UITreeView*)this));
            case eUIType::UI_TEXTBOX:
                return (new UITextBox(*(UITextBox*)this));
        }
        return nullptr;
    }

    UIControl::~UIControl()
    {
        if (m_pParent)
        {
            auto size = m_pParent->m_children.size();
            for (decltype(size) i = 0; i < size; ++i)
            {
                auto pChild = m_pParent->m_children[i];
                if (pChild == this)
                {
                    m_pParent->m_children.erase(m_pParent->m_children.begin() + i);
                    break;
                }
            }
        }
        for (auto pChild : m_children)
        {
            pChild->m_pParent = nullptr;
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

    void UIControl::insert(UIControl* pChild, UIControl* pBefore)
    {
        pChild->retain();
        if (pChild->m_pParent)
        {
            pChild->m_pParent->remove(pChild);
        }

        size_t i = 0;
        for (auto& pC : m_children)
        {
            if (pC == pBefore) break;
            ++i;
        }

        pChild->m_pParent = this;
        m_children.insert(m_children.begin() + i, pChild);
    }

    void UIControl::insertAfter(UIControl* pChild, UIControl* pAfter)
    {
        pChild->retain();
        if (pChild->m_pParent)
        {
            pChild->m_pParent->remove(pChild);
        }

        size_t i = 0;
        for (auto& pC : m_children)
        {
            ++i;
            if (pC == pAfter) break;
        }

        pChild->m_pParent = this;
        m_children.insert(m_children.begin() + i, pChild);
    }

    bool UIControl::insertAt(UIControl* pChild, decltype(std::vector<UIControl*>().size()) index)
    {
        if (index > m_children.size()) return false;

        pChild->retain();
        if (pChild->m_pParent)
        {
            pChild->m_pParent->remove(pChild);
        }

        pChild->m_pParent = this;
        m_children.insert(m_children.begin() + index, pChild);
        return true;
    }

    void UIControl::remove()
    {
        if (m_pParent) m_pParent->remove(this);
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

    void UIControl::removeAll()
    {
        while (m_children.size())
        {
            remove(m_children.front());
        }
    }

    UIControl* UIControl::getChild(const std::string& in_name, bool bSearchSubChildren) const
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

    void UIControl::getChild(const UIContext& context, 
                             const sUIVector2& mousePos, 
                             bool bSearchSubChildren, 
                             bool bIgnoreClickThrough,
                             const sUIRect& parentRect, 
                             const UIControl** ppHoverControl) const
    {
        if (!isVisible || isClickThrough && bIgnoreClickThrough) return;

        sUIRect worldRect = getWorldRect(parentRect);
        auto itend = m_children.rend();
        for (auto it = m_children.rbegin(); it != itend; ++it)
        {
            auto pChild = *it;
            pChild->getChild(context, mousePos, bSearchSubChildren, bIgnoreClickThrough, worldRect, ppHoverControl);
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

    UIControl* UIControl::getChild(const UIContext& context, 
                                   const sUIVector2& mousePos, 
                                   bool bSearchSubChildren,
                                   bool bIgnoreClickThrough) const
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
        getChild(context, mousePos, bSearchSubChildren, bIgnoreClickThrough, parentWorldRect, &pHoverControl);
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

    bool isObstructed(UIContext& context, UIControl* pRoot, UIControl* pControl, const sUIRect& worldRect)
    {
        sUIRect parentRect = {{0, 0}, context.getScreenSize()};
        bool passedUs = false;
        return !pRoot->visitVisible([&worldRect, &passedUs, pControl](UIControl* pOther, const sUIRect& rect)
        {
            if (pOther == pControl)
            {
                passedUs = true;
                return true;
            }
            if (!pOther->isClickThrough && passedUs)
            {
                if (rect.position.x + rect.size.x >= worldRect.position.x &&
                    rect.position.x <= worldRect.position.x + worldRect.size.x &&
                    rect.position.y + rect.size.y >= worldRect.position.y &&
                    rect.position.y <= worldRect.position.y + worldRect.size.y)
                {
                    return false;
                }
            }
            return true;
        }, parentRect);
    }

    bool isReallyVisible(UIControl* pControl)
    {
        if (!pControl) return true;
        return pControl->isVisible && isReallyVisible(pControl->getParent());
    }

    bool isReallyEnabled(UIControl* pControl)
    {
        if (!pControl) return true;
        return pControl->isEnabled && isReallyEnabled(pControl->getParent());
    }

    void UIControl::update(UIContext& context, const sUIVector2& mousePos, bool bMouse1Down, bool bMouse2Down, bool bMouse3Down, bool bNavL, bool bNavR, bool bNavU, bool bNavD, bool bControl, float scroll)
    {
        retain();

        // Prepare our data
        sUIRect parentRect = {{0, 0}, context.getScreenSize()};
        context.m_mouseEvents[0].mousePos = mousePos;
        context.m_mouseEvents[0].isMouseDown = bMouse1Down;
        context.m_mouseEvents[0].pContext = &context;
        context.m_mouseEvents[0].button = 1;
        context.m_mouseEvents[0].isCtrlDown = bControl;
        context.m_mouseEvents[0].scroll = scroll;
        context.m_mouseEvents[1].mousePos = mousePos;
        context.m_mouseEvents[1].isMouseDown = bMouse2Down;
        context.m_mouseEvents[1].pContext = &context;
        context.m_mouseEvents[1].button = 2;
        context.m_mouseEvents[1].isCtrlDown = bControl;
        context.m_mouseEvents[1].scroll = scroll;
        context.m_mouseEvents[2].mousePos = mousePos;
        context.m_mouseEvents[2].isMouseDown = bMouse3Down;
        context.m_mouseEvents[2].pContext = &context;
        context.m_mouseEvents[2].button = 3;
        context.m_mouseEvents[2].isCtrlDown = bControl;
        context.m_mouseEvents[2].scroll = scroll;
        context.m_pHoverControl = nullptr;

        // Update UIs
        if (context.useNavigation && context.m_pLastHoverControl)
        {
            if (!isReallyEnabled(context.m_pLastHoverControl) ||
                !isReallyVisible(context.m_pLastHoverControl) ||
                context.m_pLastHoverControl->isClickThrough ||
                isObstructed(context, this, context.m_pLastHoverControl, context.m_pLastHoverControl->getWorldRect(context)))
            {
                context.m_pLastHoverControl->release();
                context.m_pLastHoverControl = nullptr;
            }
        }
        if (context.useNavigation)
        {
            if (!context.m_pLastHoverControl)
            {
                // Find the first unobstructed navigable
                visitChildrenFirstEnabled([this, &context](UIControl* pControl, const sUIRect& rect) -> bool
                {
                    if (!pControl->isNavigatable()) return true;
                    if (!isObstructed(context, this, pControl, rect))
                    {
                        context.m_pHoverControl = pControl;
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
        if (context.useNavigation)
        {
            if (!context.m_pHoverControl)
            {
                context.m_pHoverControl = context.m_pLastHoverControl;
            }
            if (context.m_pHoverControl && 
                (bNavR || bNavL || bNavU || bNavD))
            {
                auto worldRect = context.m_pHoverControl->getWorldRect(context);
                auto pPreviousHover = context.m_pHoverControl;
                float closest = 10000.f;
                float closestH = 10000.f;

                // Navigation
                if (bNavR)
                {
                    // Find closest down navigable
                    visitChildrenFirstEnabled([this, pPreviousHover, &context, &worldRect, &closest](UIControl* pControl, const sUIRect& rect) -> bool
                    {
                        if (!pControl->isNavigatable()) return true;
                        if (pControl == pPreviousHover) return true;
                        if (rect.position.y > worldRect.position.y + worldRect.size.y) return true;
                        if (rect.position.y + rect.size.y < worldRect.position.y) return true;
                        float distance = rect.position.x - worldRect.position.x;
                        if (distance < closest && distance > 0.f)
                        {
                            if (!isObstructed(context, this, pControl, rect))
                            {
                                closest = distance;
                                context.m_pHoverControl = pControl;
                            }
                        }
                        return true;
                    }, parentRect);
                }
                else if (bNavL)
                {
                    // Find closest down navigable
                    visitChildrenFirstEnabled([this, pPreviousHover, &context, &worldRect, &closest](UIControl* pControl, const sUIRect& rect) -> bool
                    {
                        if (!pControl->isNavigatable()) return true;
                        if (pControl == pPreviousHover) return true;
                        if (rect.position.y > worldRect.position.y + worldRect.size.y) return true;
                        if (rect.position.y + rect.size.y < worldRect.position.y) return true;
                        float distance = worldRect.position.x - rect.position.x;
                        if (distance < closest && distance > 0.f)
                        {
                            if (!isObstructed(context, this, pControl, rect))
                            {
                                closest = distance;
                                context.m_pHoverControl = pControl;
                            }
                        }
                        return true;
                    }, parentRect);
                }
                else if (bNavD)
                {
                    // Find closest down navigable
                    visitChildrenFirstEnabled([this, pPreviousHover, &context, &worldRect, &closest, &closestH](UIControl* pControl, const sUIRect& rect) -> bool
                    {
                        if (!pControl->isNavigatable()) return true;
                        if (pControl == pPreviousHover) return true;
                        float distance = rect.position.y - (worldRect.position.y + worldRect.size.y);
                        float distanceH = std::abs(rect.position.x - worldRect.position.x);
                        if ((distance < closest - 16.f && distance > 0.f) ||
                            (distance < closest + 16.f && distance > 0.f && distanceH < closestH))
                        {
                            if (!isObstructed(context, this, pControl, rect))
                            {
                                closest = distance;
                                closestH = distanceH;
                                context.m_pHoverControl = pControl;
                            }
                        }
                        return true;
                    }, parentRect);
                }
                else if (bNavU)
                {
                    // Find closest down navigable
                    visitChildrenFirstEnabled([this, pPreviousHover, &context, &worldRect, &closest, &closestH](UIControl* pControl, const sUIRect& rect) -> bool
                    {
                        if (!pControl->isNavigatable()) return true;
                        if (pControl == pPreviousHover) return true;
                        float distance = worldRect.position.y - (rect.position.y + rect.size.y);
                        float distanceH = std::abs(rect.position.x - worldRect.position.x);
                        if ((distance < closest - 16.f && distance > 0.f) ||
                            (distance < closest + 16.f && distance > 0.f && distanceH < closestH))
                        {
                            if (!isObstructed(context, this, pControl, rect))
                            {
                                closest = distance;
                                closestH = distanceH;
                                context.m_pHoverControl = pControl;
                            }
                        }
                        return true;
                    }, parentRect);
                }
            }
        }

        // Resolve
        context.resolve();

        // Send events
        context.dispatchEvents();

        // Reset for next frame
        context.reset();

        release();
    }

    void UIContext::resolve()
    {
        // If it's the first mouse down since last frame, and there is an hover
        // control. Put him into down state
        for (int i = 0; i < 3; ++i)
        {
            if (!m_lastMouseEvents[i].isMouseDown &&
                m_mouseEvents[i].isMouseDown &&
                m_pHoverControl)
            {
                m_pDownControls[i] = m_pHoverControl;
            }

            if (!m_mouseEvents[i].isMouseDown)
            {
                m_pDownControls[i] = nullptr;
            }

            // Don't allow hovering of other controls if a control is in DOWN state
            if (m_pDownControls[i] &&
                m_pHoverControl != m_pDownControls[i])
            {
                m_pHoverControl = nullptr;
            }
        }

        if (m_pDownControls[0])
        {
            m_pFocus = m_pDownControls[0];
        }
    }

    void UIContext::dispatchEvents()
    {
        if (m_pHoverControl) m_pHoverControl->retain();
        for (int i = 0; i < 3; ++i) if (m_pDownControls[i]) m_pDownControls[i]->retain();
        if (m_pFocus) m_pFocus->retain();

        if (m_pLastHoverControl) m_pLastHoverControl->retain();
        for (int i = 0; i < 3; ++i) if (m_pLastDownControls[i]) m_pLastDownControls[i]->retain();
        if (m_pLastFocus) m_pLastFocus->retain();

        // Do writes
        if (m_pFocus)
        {
            for (auto c : m_writes)
            {
                m_pFocus->onWriteInternal(c, *this);
            }
        }
        m_writes.clear();

        // Do key events
        if (m_pFocus)
        {
            UIKeyEvent evt;
            evt.pContext = this;
            for (auto key : m_keyDowns)
            {
                evt.key = key;
                m_pFocus->onKeyDownInternal(evt);
                if (m_pFocus->onKeyDown)
                {
                    m_pFocus->onKeyDown(m_pFocus, evt);
                }
            }
        }
        m_keyDowns.clear();

        if (m_pHoverControl != m_pLastHoverControl)
        {
            if (m_pLastHoverControl)
            {
                if (m_pLastHoverControl->onMouseLeave)
                {
                    m_pLastHoverControl->onMouseLeave(m_pLastHoverControl, m_mouseEvents[0]);
                }
            }
            if (m_pHoverControl)
            {
                if (m_pHoverControl->onMouseEnter)
                {
                    m_pHoverControl->onMouseEnter(m_pHoverControl, m_mouseEvents[0]);
                }
            }
        }
        if (m_pHoverControl)
        {
            if (!useNavigation)
            {
                if (m_mouseEvents[0].scroll != 0)
                {
                    m_pHoverControl->onMouseScrollInternal(m_mouseEvents[0]);
                }
            }
        }
        for (int i = 0; i < 3; ++i)
        {
            if (m_pDownControls[i] != m_pLastDownControls[i])
            {
                if (m_pLastDownControls[i])
                {
                    if (!i) m_pLastDownControls[i]->onMouseUpInternal(m_mouseEvents[i]);
                    if (i == 0)
                    {
                        if (m_pLastDownControls[i]->onMouseUp)
                        {
                            m_pLastDownControls[i]->onMouseUp(m_pLastDownControls[i], m_mouseEvents[i]);
                        }
                    }
                    else if (i == 1)
                    {
                        if (m_pLastDownControls[i]->onRightMouseUp)
                        {
                            m_pLastDownControls[i]->onRightMouseUp(m_pLastDownControls[i], m_mouseEvents[i]);
                        }
                    }
                    else if (i == 2)
                    {
                        if (m_pLastDownControls[i]->onMiddleMouseUp)
                        {
                            m_pLastDownControls[i]->onMiddleMouseUp(m_pLastDownControls[i], m_mouseEvents[i]);
                        }
                    }
                    if (m_pHoverControl == m_pLastDownControls[i] &&
                        !m_mouseEvents[i].isMouseDown)
                    {
                        if (!i) m_pLastDownControls[i]->onClickInternal(m_mouseEvents[i]);
                        if (i == 0)
                        {
                            if (m_pLastDownControls[i]->onClick)
                            {
                                m_pLastDownControls[i]->onClick(m_pLastDownControls[i], m_mouseEvents[i]);
                            }
                        }
                        else if (i == 1)
                        {
                            if (m_pLastDownControls[i]->onRightClick)
                            {
                                m_pLastDownControls[i]->onRightClick(m_pLastDownControls[i], m_mouseEvents[i]);
                            }
                        }
                        else if (i == 2)
                        {
                            if (m_pLastDownControls[i]->onMiddleClick)
                            {
                                m_pLastDownControls[i]->onMiddleClick(m_pLastDownControls[i], m_mouseEvents[i]);
                            }
                        }
                    }
                }
                if (m_pDownControls[i])
                {
                    if (!i) m_pDownControls[i]->onMouseDownInternal(m_mouseEvents[i]);
                    if (i == 0)
                    {
                        if (m_pDownControls[i]->onMouseDown)
                        {
                            m_pDownControls[i]->onMouseDown(m_pDownControls[i], m_mouseEvents[i]);
                        }
                    }
                    else if (i == 1)
                    {
                        if (m_pDownControls[i]->onRightMouseDown)
                        {
                            m_pDownControls[i]->onRightMouseDown(m_pDownControls[i], m_mouseEvents[i]);
                        }
                    }
                    else if (i == 2)
                    {
                        if (m_pDownControls[i]->onMiddleMouseDown)
                        {
                            m_pDownControls[i]->onMiddleMouseDown(m_pDownControls[i], m_mouseEvents[i]);
                        }
                    }

                    // Check for double click events
                    auto now = std::chrono::steady_clock::now();
                    if (now - m_clickTimes[i] <= doubleClickTime && !useNavigation &&
                        m_clicksPos[i].x > m_mouseEvents[i].mousePos.x - 3 &&
                        m_clicksPos[i].y > m_mouseEvents[i].mousePos.y - 3 &&
                        m_clicksPos[i].x < m_mouseEvents[i].mousePos.x + 3 &&
                        m_clicksPos[i].y < m_mouseEvents[i].mousePos.y + 3)
                    {
                        if (i == 0)
                        {
                            if (m_pDownControls[i]->onDoubleClick)
                            {
                                m_pDownControls[i]->onDoubleClick(m_pDownControls[i], m_mouseEvents[i]);
                            }
                        }
                        else if (i == 1)
                        {
                            if (m_pDownControls[i]->onRightDoubleClick)
                            {
                                m_pDownControls[i]->onRightDoubleClick(m_pDownControls[i], m_mouseEvents[i]);
                            }
                        }
                        else if (i == 2)
                        {
                            if (m_pDownControls[i]->onMiddleDoubleClick)
                            {
                                m_pDownControls[i]->onMiddleDoubleClick(m_pDownControls[i], m_mouseEvents[i]);
                            }
                        }
                    }
                    m_clickTimes[i] = now;
                    m_clicksPos[i] = m_mouseEvents[i].mousePos;
                }
            }
        }

        // Focus gain/lost
        if (m_pFocus != m_pLastFocus)
        {
            if (m_pLastFocus)
            {
                UIFocusEvent evt;
                evt.pContext = this;
                m_pLastFocus->onLoseFocusInternal(evt);
                if (m_pLastFocus->onLoseFocus)
                {
                    m_pLastFocus->onLoseFocus(m_pLastFocus);
                }
            }
            if (m_pFocus)
            {
                UIFocusEvent evt;
                evt.pContext = this;
                m_pFocus->onGainFocusInternal(evt);
                if (m_pFocus->onGainFocus)
                {
                    m_pFocus->onGainFocus(m_pFocus);
                }
            }
        }

        if (!useNavigation)
        {
            if (m_lastMouseEvents[0].mousePos.x != m_mouseEvents[0].mousePos.x ||
                m_lastMouseEvents[0].mousePos.y != m_mouseEvents[0].mousePos.y)
            {
                if (m_pDownControls[0])
                {
                    m_pDownControls[0]->onMouseMoveInternal(m_mouseEvents[0]);
                    if (m_pDownControls[0]->onMouseMove)
                    {
                        m_pDownControls[0]->onMouseMove(m_pDownControls[0], m_mouseEvents[0]);
                    }
                }
                else if (m_pHoverControl)
                {
                    m_pHoverControl->onMouseMoveInternal(m_mouseEvents[0]);
                    if (m_pHoverControl->onMouseMove)
                    {
                        m_pHoverControl->onMouseMove(m_pHoverControl, m_mouseEvents[0]);
                    }
                }
            }
        }

        if (m_pHoverControl) if (m_pHoverControl->getRefCount() <= 1) { m_pHoverControl->release(); m_pHoverControl = nullptr; }
        else { m_pHoverControl->release(); }
        for (int i = 0; i < 3; ++i) if (m_pDownControls[i]) if (m_pDownControls[i]->getRefCount() <= 1) { m_pDownControls[i]->release(); m_pDownControls[i] = nullptr; }
        else { m_pDownControls[i]->release(); }
        if (m_pFocus) if (m_pFocus->getRefCount() <= 1) { m_pFocus->release(); m_pFocus = nullptr; }
        else { m_pFocus->release(); }

        if (m_pLastHoverControl) if (m_pLastHoverControl->getRefCount() <= 1) { m_pLastHoverControl->release(); m_pLastHoverControl = nullptr; }
        else { m_pLastHoverControl->release(); }
        for (int i = 0; i < 3; ++i) if (m_pLastDownControls[i]) if (m_pLastDownControls[i]->getRefCount() <= 1) { m_pLastDownControls[i]->release(); m_pLastDownControls[i] = nullptr; }
        else { m_pLastDownControls[i]->release(); }
        if (m_pLastFocus) if (m_pLastFocus->getRefCount() <= 1) { m_pLastFocus->release(); m_pLastFocus = nullptr; }
        else { m_pLastFocus->release(); }
    }

    void UIContext::reset()
    {
        for (int i = 0; i < 3; ++i) m_lastMouseEvents[i] = m_mouseEvents[i];

        if (m_pLastHoverControl) m_pLastHoverControl->release();
        for (int i = 0; i < 3; ++i) if (m_pLastDownControls[i]) m_pLastDownControls[i]->release();
        if (m_pLastFocus) m_pLastFocus->release();

        m_pLastHoverControl = m_pHoverControl;
        for (int i = 0; i < 3; ++i) m_pLastDownControls[i] = m_pDownControls[i];
        m_pLastFocus = m_pFocus;

        if (m_pLastHoverControl) m_pLastHoverControl->retain();
        for (int i = 0; i < 3; ++i) if (m_pLastDownControls[i]) m_pLastDownControls[i]->retain();
        if (m_pLastFocus) m_pLastFocus->retain();

        m_clips.clear();
    }

    void UIContext::write(char c)
    {
        m_writes.push_back(c);
    }

    void UIContext::keyDown(uintptr_t key)
    {
        m_keyDowns.push_back(key);
    }

    void UIContext::focus(UIControl* pFocus)
    {
        m_pFocus = pFocus;
    }

    void UIContext::clearState()
    {
        if (m_pLastHoverControl) m_pLastHoverControl->release();
        for (int i = 0; i < 3; ++i) if (m_pLastDownControls[i]) m_pLastDownControls[i]->release();
        if (m_pLastFocus) m_pLastFocus->release();

        m_pLastHoverControl = nullptr;
        for (int i = 0; i < 3; ++i) m_pLastDownControls[i] = nullptr;
        m_pLastFocus = nullptr;

        m_pHoverControl = nullptr;
        for (int i = 0; i < 3; ++i) m_pDownControls[i] = nullptr;
        m_pFocus = nullptr;

        m_clips.clear();
        m_writes.clear();
        m_keyDowns.clear();

        for (int i = 0; i < 3; ++i)
        {
            m_clicksPos[i].x = -100;
            m_clicksPos[i].y = -100;
        }
    }

    void UIControl::render(UIContext& context)
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
        switch (xType)
        {
            case eUIPosType::POS_RELATIVE:
                worldRect.position.x = rect.position.x;
                break;
            case eUIPosType::POS_PERCENTAGE:
                worldRect.position.x = parentRect.size.x * rect.position.x;
                break;
        }
        switch (yType)
        {
            case eUIPosType::POS_RELATIVE:
                worldRect.position.y = rect.position.y;
                break;
            case eUIPosType::POS_PERCENTAGE:
                worldRect.position.y = parentRect.size.y * rect.position.y;
                break;
        }
        switch (widthType)
        {
            case eUIDimType::DIM_ABSOLUTE:
                worldRect.size.x = rect.size.x;
                break;
            case eUIDimType::DIM_RELATIVE:
                worldRect.size.x = parentRect.size.x + rect.size.x;
                break;
            case eUIDimType::DIM_PERCENTAGE:
                worldRect.size.x = parentRect.size.x * rect.size.x;
                break;
        }
        switch (heightType)
        {
            case eUIDimType::DIM_ABSOLUTE:
                worldRect.size.y = rect.size.y;
                break;
            case eUIDimType::DIM_RELATIVE:
                worldRect.size.y = parentRect.size.y + rect.size.y;
                break;
            case eUIDimType::DIM_PERCENTAGE:
                worldRect.size.y = parentRect.size.y * rect.size.y;
                break;
        }

        // Then do alignement
        switch (align)
        {
            case eUIAlign::TOP_LEFT:
                worldRect.position.x = parentRect.position.x + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + worldRect.position.y;
                break;
            case eUIAlign::TOP:
                worldRect.position.x = parentRect.position.x + parentRect.size.x * .5f + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + worldRect.position.y;
                break;
            case eUIAlign::TOP_RIGHT:
                worldRect.position.x = parentRect.position.x + parentRect.size.x + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + worldRect.position.y;
                break;
            case eUIAlign::LEFT:
                worldRect.position.x = parentRect.position.x + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + parentRect.size.y * .5f + worldRect.position.y;
                break;
            case eUIAlign::CENTER:
                worldRect.position.x = parentRect.position.x + parentRect.size.x * .5f + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + parentRect.size.y * .5f + worldRect.position.y;
                break;
            case eUIAlign::RIGHT:
                worldRect.position.x = parentRect.position.x + parentRect.size.x + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + parentRect.size.y * .5f + worldRect.position.y;
                break;
            case eUIAlign::BOTTOM_LEFT:
                worldRect.position.x = parentRect.position.x + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + parentRect.size.y + worldRect.position.y;
                break;
            case eUIAlign::BOTTOM:
                worldRect.position.x = parentRect.position.x + parentRect.size.x * .5f + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + parentRect.size.y + worldRect.position.y;
                break;
            case eUIAlign::BOTTOM_RIGHT:
                worldRect.position.x = parentRect.position.x + parentRect.size.x + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + parentRect.size.y + worldRect.position.y;
                break;
        }

        // Finally anchoring
        switch (xAnchorType)
        {
            case eUIAnchorType::ANCHOR_PIXEL:
                worldRect.position.x -= anchor.x;
                break;
            case eUIAnchorType::ANCHOR_PERCENTAGE:
                worldRect.position.x -= worldRect.size.x * anchor.x;
                break;
        }
        switch (yAnchorType)
        {
            case eUIAnchorType::ANCHOR_PIXEL:
                worldRect.position.y -= anchor.y;
                break;
            case eUIAnchorType::ANCHOR_PERCENTAGE:
                worldRect.position.y -= worldRect.size.y * anchor.y;
                break;
        }

        // Snap to pixels
        worldRect.position.x = std::roundf(worldRect.position.x);
        worldRect.position.y = std::roundf(worldRect.position.y);
            
        return std::move(worldRect);
    }

    void UIControl::updateInternal(UIContext& context, const sUIRect& parentRect)
    {
        if (!isEnabled || !isVisible) return;
        sUIRect worldRect = getWorldRect(parentRect);

        // Do children first, inverted
        if (context.useNavigation)
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

        if (!context.m_pHoverControl && !isClickThrough)
        {
            auto &mouseEvt = context.m_mouseEvents[0];
            if (mouseEvt.mousePos.x >= worldRect.position.x &&
                mouseEvt.mousePos.y >= worldRect.position.y &&
                mouseEvt.mousePos.x <= worldRect.position.x + worldRect.size.x &&
                mouseEvt.mousePos.y <= worldRect.position.y + worldRect.size.y)
            {
                context.m_mouseEvents[0].localMousePos.x = mouseEvt.mousePos.x - worldRect.position.x;
                context.m_mouseEvents[0].localMousePos.y = mouseEvt.mousePos.y - worldRect.position.y;
                context.m_mouseEvents[1].localMousePos = context.m_mouseEvents[0].localMousePos;
                context.m_mouseEvents[2].localMousePos = context.m_mouseEvents[0].localMousePos;
                context.m_pHoverControl = this;
            }
        }
    }

    void UIControl::renderInternal(UIContext& context, const sUIRect& parentRect)
    {
        if (!isVisible) return;

        sUIRect worldRect = getWorldRect(parentRect);
        if (clipChildren)
        {
            context.pushClip(worldRect);
        }
        renderControl(context, worldRect);
        for (auto pChild : m_children)
        {
            pChild->renderInternal(context, worldRect);
        }
        if (clipChildren)
        {
            context.popClip();
        }
    }

    void UIControl::setWorldRect(const sUIRect& in_rect, const UIContext& context)
    {
        if (getParent())
        {
            auto parentRect = getParent()->getWorldRect(context);

            sUIRect localRect = in_rect;

            // Undo anchoring
            switch (xAnchorType)
            {
                case eUIAnchorType::ANCHOR_PIXEL:
                    localRect.position.x += anchor.x;
                    break;
                case eUIAnchorType::ANCHOR_PERCENTAGE:
                    localRect.position.x += localRect.size.x * anchor.x;
                    break;
            }
            switch (yAnchorType)
            {
                case eUIAnchorType::ANCHOR_PIXEL:
                    localRect.position.y += anchor.y;
                    break;
                case eUIAnchorType::ANCHOR_PERCENTAGE:
                    localRect.position.y += localRect.size.y * anchor.y;
                    break;
            }

            // Undo alignement
            switch (align)
            {
                case eUIAlign::TOP_LEFT:
                    localRect.position.x = localRect.position.x - parentRect.position.x;
                    localRect.position.y = localRect.position.y - parentRect.position.y;
                    break;
                case eUIAlign::TOP:
                    localRect.position.x = localRect.position.x - parentRect.position.x - parentRect.size.x * .5f;
                    localRect.position.y = localRect.position.y - parentRect.position.y;
                    break;
                case eUIAlign::TOP_RIGHT:
                    localRect.position.x = localRect.position.x - parentRect.position.x - parentRect.size.x;
                    localRect.position.y = localRect.position.y - parentRect.position.y;
                    break;
                case eUIAlign::LEFT:
                    localRect.position.x = localRect.position.x - parentRect.position.x;
                    localRect.position.y = localRect.position.y - parentRect.position.y - parentRect.size.y * .5f;
                    break;
                case eUIAlign::CENTER:
                    localRect.position.x = localRect.position.x - parentRect.position.x - parentRect.size.x * .5f;
                    localRect.position.y = localRect.position.y - parentRect.position.y - parentRect.size.y * .5f;
                    break;
                case eUIAlign::RIGHT:
                    localRect.position.x = localRect.position.x - parentRect.position.x - parentRect.size.x;
                    localRect.position.y = localRect.position.y - parentRect.position.y - parentRect.size.y * .5f;
                    break;
                case eUIAlign::BOTTOM_LEFT:
                    localRect.position.x = localRect.position.x - parentRect.position.x;
                    localRect.position.y = localRect.position.y - parentRect.position.y - parentRect.size.y;
                    break;
                case eUIAlign::BOTTOM:
                    localRect.position.x = localRect.position.x - parentRect.position.x - parentRect.size.x * .5f;
                    localRect.position.y = localRect.position.y - parentRect.position.y - parentRect.size.y;
                    break;
                case eUIAlign::BOTTOM_RIGHT:
                    localRect.position.x = localRect.position.x - parentRect.position.x - parentRect.size.x;
                    localRect.position.y = localRect.position.y - parentRect.position.y - parentRect.size.y;
                    break;
            }

            // Undo position and width values
            switch (xType)
            {
                case eUIPosType::POS_RELATIVE:
                    localRect.position.x = localRect.position.x;
                    break;
                case eUIPosType::POS_PERCENTAGE:
                    localRect.position.x = localRect.position.x / parentRect.size.x;
                    break;
            }
            switch (yType)
            {
                case eUIPosType::POS_RELATIVE:
                    localRect.position.y = localRect.position.y;
                    break;
                case eUIPosType::POS_PERCENTAGE:
                    localRect.position.y = localRect.position.y / parentRect.size.y;
                    break;
            }
            switch (widthType)
            {
                case eUIDimType::DIM_ABSOLUTE:
                    localRect.size.x = localRect.size.x;
                    break;
                case eUIDimType::DIM_RELATIVE:
                    localRect.size.x = localRect.size.x - parentRect.size.x;
                    break;
                case eUIDimType::DIM_PERCENTAGE:
                    localRect.size.x = localRect.size.x / parentRect.size.x;
                    break;
            }
            switch (heightType)
            {
                case eUIDimType::DIM_ABSOLUTE:
                    localRect.size.y = localRect.size.y;
                    break;
                case eUIDimType::DIM_RELATIVE:
                    localRect.size.y = localRect.size.y - parentRect.size.y;
                    break;
                case eUIDimType::DIM_PERCENTAGE:
                    localRect.size.y = localRect.size.y / parentRect.size.y;
                    break;
            }

            rect = std::move(localRect);
        }
        else
        {
            rect = in_rect;
        }
    }

    sUIVector2 UIControl::getAnchorInPixel() const
    {
        sUIVector2 ret = anchor;
        if (xAnchorType == onut::eUIAnchorType::ANCHOR_PERCENTAGE)
        {
            ret.x = ret.x * rect.size.x;
        }
        if (yAnchorType == onut::eUIAnchorType::ANCHOR_PERCENTAGE)
        {
            ret.y = ret.y * rect.size.y;
        }
        return std::move(ret);
    }

    sUIVector2 UIControl::getAnchorInPercentage() const
    {
        sUIVector2 ret = anchor;
        if (xAnchorType == onut::eUIAnchorType::ANCHOR_PIXEL)
        {
            ret.x = ret.x / rect.size.x;
        }
        if (yAnchorType == onut::eUIAnchorType::ANCHOR_PIXEL)
        {
            ret.y = ret.y / rect.size.y;
        }
        return std::move(ret);
    }

    void UIControl::setAnchorPercent(const sUIVector2& in_anchor)
    {
        if (xAnchorType == onut::eUIAnchorType::ANCHOR_PIXEL)
        {
            anchor.x = rect.size.x * in_anchor.x;
        }
        else
        {
            anchor.x = in_anchor.x;
        }
        if (yAnchorType == onut::eUIAnchorType::ANCHOR_PIXEL)
        {
            anchor.y = rect.size.y * in_anchor.y;
        }
        else
        {
            anchor.y = in_anchor.y;
        }
    }

    eUIState UIControl::getState(const UIContext& context) const
    {
        if (isEnabled)
        {
            if (context.m_pHoverControl == this)
            {
                if (context.m_pDownControls[0] == this)
                {
                    return eUIState::DOWN;
                }
                else
                {
                    return eUIState::HOVER;
                }
            }
            else if (context.m_pDownControls[0] == this)
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

    bool UIControl::hasFocus(const UIContext& context) const
    {
        return (context.m_pFocus == this);
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
        pItem->retain();
        auto pParent = pItem->getParent();
        if (pParent) pParent->removeItem(pItem);
        pItem->setTreeView(this);
        m_items.push_back(pItem);
    }

    void UITreeView::addItemBefore(UITreeViewItem* pItem, UITreeViewItem* pBefore)
    {
        if (!pBefore)
        {
            addItem(pItem);
            return;
        }
        for (auto pMyItem : m_items)
        {
            if (pMyItem == pItem) return;
        }
        pItem->retain();
        auto pParent = pItem->getParent();
        if (pParent) pParent->removeItem(pItem);
        pItem->setTreeView(this);
        for (auto it = m_items.begin(); it != m_items.end(); ++it)
        {
            if (*it == pBefore)
            {
                m_items.insert(it, pItem);
                return;
            }
        }
    }

    void UITreeView::addItemAfter(UITreeViewItem* pItem, UITreeViewItem* pAfter)
    {
        if (!pAfter)
        {
            addItem(pItem);
            return;
        }
        for (auto pMyItem : m_items)
        {
            if (pMyItem == pItem) return;
        }
        pItem->retain();
        auto pParent = pItem->getParent();
        if (pParent) pParent->removeItem(pItem);
        pItem->setTreeView(this);
        for (auto it = m_items.begin(); it != m_items.end(); ++it)
        {
            if (*it == pAfter)
            {
                ++it;
                m_items.insert(it, pItem);
                return;
            }
        }
    }

    void UITreeView::removeItem(UITreeViewItem* pItem)
    {
        auto size = m_items.size();
        for (decltype(size) i = 0; i < size; ++i)
        {
            if (m_items[i] == pItem)
            {
                m_items.erase(m_items.begin() + i);
                pItem->m_pParent = nullptr;
                pItem->setTreeView(nullptr);
                pItem->release();
                return;
            }
        }
    }

    UITreeView::~UITreeView()
    {
        for (auto pItem : m_items)
        {
            delete pItem;
        }
    }

    void UITreeView::clear()
    {
        for (auto pItem : m_items)
        {
            delete pItem;
        }
        m_items.clear();
    }

    UITreeViewItem* UITreeView::getItemAtPosition(const sUIVector2& pos, const sUIRect& rect, bool* pPickedExpandButton, sUIRect* pItemRect) const
    {
        // Render it's items
        sUIRect itemRect = {rect.position, {rect.size.x, itemHeight}};
        itemRect.position.y -= m_scroll;
        for (auto pItem : m_items)
        {
            auto pRet = getItemAtPosition(pItem, pos, itemRect, pPickedExpandButton, pItemRect);
            if (pRet)
            {
                return pRet;
            }
        }
        return nullptr;
    }

    float UITreeView::getTotalHeight(UITreeViewItem* pItem) const
    {
        float ret = itemHeight;
        if (pItem)
        {
            if (pItem->isExpanded)
            {
                for (auto pChild : pItem->getItems())
                {
                    ret += getTotalHeight(pChild);
                }
            }
        }
        else
        {
            for (auto pChild : m_items)
            {
                ret += getTotalHeight(pChild);
            }
        }
        return ret;
    }

    UITreeViewItem* UITreeView::getItemAtPosition(UITreeViewItem* pItem, const sUIVector2& pos, sUIRect& rect, bool* pPickedExpandButton, sUIRect* pItemRect) const
    {
        if (pos.y >= rect.position.y &&
            pos.y <= rect.position.y + rect.size.y)
        {
            if (pos.x >= rect.position.x + expandClickWidth ||
                pos.x <= rect.position.x)
            {
                if (pItemRect) *pItemRect = rect;
                return pItem;
            }
            else if (pPickedExpandButton)
            {
                if (pItemRect) *pItemRect = rect;
                *pPickedExpandButton = true;
                return pItem;
            }
        }
        rect.position.y += itemHeight;
        if (pItem->isExpanded)
        {
            if (!pItem->m_items.empty())
            {
                auto xOffset = expandedXOffset;
                rect.position.x += xOffset;
                rect.size.x -= xOffset;
                for (auto pHisItem : pItem->m_items)
                {
                    auto pRet = getItemAtPosition(pHisItem, pos, rect, pPickedExpandButton, pItemRect);
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
            pItem->m_pParent->isExpanded = true;
            expandTo(pItem->m_pParent);
        }
    }

    void UITreeViewItem::setTreeView(UITreeView* pTreeView)
    {
        m_pTreeView = pTreeView;
        for (auto pItem : m_items)
        {
            pItem->setTreeView(pTreeView);
        }
    }

    void UITreeViewItem::addItem(UITreeViewItem* pItem)
    {
        pItem->retain();
        if (pItem->m_pParent) pItem->m_pParent->removeItem(pItem);
        pItem->m_pParent = this;
        pItem->setTreeView(m_pTreeView);
        m_items.push_back(pItem);
    }

    void UITreeViewItem::addItemBefore(UITreeViewItem* pItem, UITreeViewItem* pBefore)
    {
        if (!pBefore)
        {
            addItem(pItem);
            return;
        }
        pItem->retain();
        if (pItem->m_pParent) pItem->m_pParent->removeItem(pItem);
        pItem->m_pParent = this;
        pItem->setTreeView(m_pTreeView);
        for (auto it = m_items.begin(); it != m_items.end(); ++it)
        {
            if (*it == pBefore)
            {
                m_items.insert(it, pItem);
                return;
            }
        }
        pItem->release();
    }

    void UITreeViewItem::addItemAfter(UITreeViewItem* pItem, UITreeViewItem* pAfter)
    {
        if (!pAfter)
        {
            addItem(pItem);
            return;
        }
        pItem->retain();
        if (pItem->m_pParent) pItem->m_pParent->removeItem(pItem);
        pItem->m_pParent = this;
        pItem->setTreeView(m_pTreeView);
        for (auto it = m_items.begin(); it != m_items.end(); ++it)
        {
            if (*it == pAfter)
            {
                m_items.insert(it + 1, pItem);
                return;
            }
        }
        pItem->release();
    }

    void UITreeViewItem::retain()
    {
        ++m_refCount;
    }

    void UITreeViewItem::release()
    {
        --m_refCount;
        if (m_refCount <= 0)
        {
            delete this;
        }
    }

    void UITreeViewItem::removeItem(UITreeViewItem* pItem)
    {
        auto size = m_items.size();
        for (decltype(size) i = 0; i < size; ++i)
        {
            if (m_items[i] == pItem)
            {
                m_items.erase(m_items.begin() + i);
                pItem->setTreeView(nullptr);
                pItem->m_pParent = nullptr;
                pItem->release();
                return;
            }
        }
    }

    bool UITextBox::isCursorVisible() const
    {
        auto now = std::chrono::steady_clock::now();
        auto diff = now - m_cursorTime;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
        return (ms.count() % 1000) < 500;
    }

    void UITextBox::numerifyText()
    {
        if (m_isNumerical)
        {
            std::stringstream ss(textComponent.text);
            float value;
            if (!(ss >> value))
            {
                value = 0.f;
            }
            if (value < min) value = min;
            if (value > max) value = max;
            std::stringstream ssOut;
            ssOut << std::fixed << std::setprecision(static_cast<std::streamsize>(m_decimalPrecision)) << value;
            textComponent.text = ssOut.str();
        }
    }

    float UITextBox::getFloat() const
    {
        if (m_isNumerical)
        {
            std::stringstream ss(textComponent.text);
            float value;
            if (!(ss >> value))
            {
                value = 0.f;
            }
            return value;
        }
        return 0.f;
    }

    int UITextBox::getInt() const
    {
        if (m_isNumerical)
        {
            std::stringstream ss(textComponent.text);
            int value;
            if (!(ss >> value))
            {
                value = 0;
            }
            return value;
        }
        return 0;
    }

    void UITextBox::setFloat(float f)
    {
        textComponent.text = std::to_string(f);
        numerifyText();
    }

    void UITextBox::setInt(int i)
    {
        textComponent.text = std::to_string(i);
        numerifyText();
    }

    //--- Constructors
    UIButton::UIButton()
    {
        textComponent.font.align = eUIAlign::CENTER;
    }

    UILabel::UILabel()
    {
        textComponent.font.align = eUIAlign::LEFT;
    }

    //--- Copy
    UIButton::UIButton(const UIButton& other) :
        UIControl(other)
    {
        textComponent = other.textComponent;
        scale9Component = other.scale9Component;
    }

    UIPanel::UIPanel(const UIPanel& other) :
        UIControl(other)
    {
        color = other.color;
    }

    UILabel::UILabel(const UILabel& other) :
        UIControl(other)
    {
        textComponent = other.textComponent;
    }

    UIImage::UIImage(const UIImage& other) :
        UIControl(other)
    {
        scale9Component = other.scale9Component;
    }

    UICheckBox::UICheckBox(const UICheckBox& other) :
        UIControl(other)
    {
        iconComponent = other.iconComponent;
        textComponent = other.textComponent;
        behavior = other.behavior;
        m_isChecked = other.m_isChecked;
    }

    UITextBox::UITextBox(const UITextBox& other) :
        UIControl(other)
    {
        textComponent = other.textComponent;
        scale9Component = other.scale9Component;
        m_isNumerical = other.m_isNumerical;
        m_decimalPrecision = other.m_decimalPrecision;
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
        text(text)
    {
    }

    UITreeViewItem::UITreeViewItem(const UITreeViewItem& other)
    {
        for (auto pOtherItem : other.m_items)
        {
            m_items.push_back(new UITreeViewItem(*pOtherItem));
        }
        isExpanded = other.isExpanded;
        text = other.text;
    }

    //--- Properties
    void UICheckBox::setIsChecked(bool in_isChecked)
    {
        switch (behavior)
        {
            case eUICheckBehavior::NORMAL:
            {
                m_isChecked = in_isChecked;
                break;
            }
            case eUICheckBehavior::CHK_OPTIONAL:
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
                                if (pSibblingCheckBox->behavior == behavior)
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
                                if (pSibblingCheckBox->behavior == behavior)
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

    void UITextBox::setIsNumerical(bool isNumerical)
    {
        m_isNumerical = isNumerical;
        numerifyText();
    }

    void UITextBox::setIsDecimalPrecision(int decimalPrecision)
    {
        m_decimalPrecision = decimalPrecision;
        numerifyText();
    }

    //--- Loads / Saves
    void UIButton::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        textComponent = getJsonTextComponent(jsonNode["textComponent"]);
        scale9Component = getJsonScale9Component(jsonNode);
    }

    void UIButton::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonTextComponent(jsonNode, textComponent, allocator);
        setJsonScale9Component(jsonNode, scale9Component, allocator);
    }

    void UIPanel::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        color = getJsonColor(jsonNode["color"]);
    }

    void UIPanel::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonColor(jsonNode, "color", color, allocator);
    }

    void UILabel::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        textComponent = getJsonTextComponent(jsonNode["textComponent"]);
    }

    void UILabel::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonTextComponent(jsonNode, textComponent, allocator);
    }

    void UIImage::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        scale9Component = getJsonScale9Component(jsonNode);
    }

    void UIImage::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonScale9Component(jsonNode, scale9Component, allocator);
    }

    void UICheckBox::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        textComponent = getJsonTextComponent(jsonNode["textComponent"]);
        m_isChecked = getJsonBool(jsonNode["checked"], false);
        behavior = getJsonEnum(checkBehaviorMap, jsonNode["behavior"], eUICheckBehavior::NORMAL);
    }

    void UICheckBox::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonTextComponent(jsonNode, textComponent, allocator);
        setJsonBool(jsonNode, "checked", m_isChecked, allocator, false);
        setJsonString(jsonNode, "behavior", enumToString(checkBehaviorMap, behavior), allocator, "NORMAL");
    }

    void UITreeView::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        expandedXOffset = getJsonFloat(jsonNode["expandedXOffset"], 18.f);
        expandClickWidth = getJsonFloat(jsonNode["expandClickWidth"], 18.f);
        itemHeight = getJsonFloat(jsonNode["itemHeight"], 18.f);
    }

    void UITreeView::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonFloat(jsonNode, "expandedXOffset", expandedXOffset, allocator, 18.f);
        setJsonFloat(jsonNode, "expandClickWidth", expandClickWidth, allocator, 18.f);
        setJsonFloat(jsonNode, "itemHeight", itemHeight, allocator, 18.f);
    }

    void UITextBox::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        textComponent = getJsonTextComponent(jsonNode["textComponent"]);
        scale9Component = getJsonScale9Component(jsonNode);
        m_isNumerical = getJsonBool(jsonNode["numerical"]);
        m_decimalPrecision = getJsonInt(jsonNode["precision"]);
        numerifyText();
    }

    void UITextBox::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonTextComponent(jsonNode, textComponent, allocator);
        setJsonScale9Component(jsonNode, scale9Component, allocator);
        setJsonBool(jsonNode, "numerical", m_isNumerical, allocator);
        setJsonInt(jsonNode, "precision", m_decimalPrecision, allocator);
    }

    //--- Renders
    void renderScale9Component(const UIContext& context, UIControl* pControl, const sUIRect& rect, const sUIScale9Component& scale9Component)
    {
        if (scale9Component.image.filename.empty())
        {
            context.drawRect(pControl, rect, scale9Component.image.color);
        }
        else if (scale9Component.isScaled9)
        {
            context.drawScale9Rect(pControl, rect, scale9Component);
        }
        else
        {
            context.drawTexturedRect(pControl, rect, scale9Component.image);
        }
    }

    void renderTextComponent(const UIContext& context, UIControl* pControl, const sUIRect& rect, const sUITextComponent& textComponent)
    {
        auto textRect = rect;
        textRect.position.x += textComponent.font.padding.left;
        textRect.position.y += textComponent.font.padding.top;
        textRect.size.x -= textComponent.font.padding.left + textComponent.font.padding.right;
        textRect.size.y -= textComponent.font.padding.top + textComponent.font.padding.bottom;
        context.drawText(pControl, textRect, textComponent);
    }

    void UIButton::renderControl(const UIContext& context, const sUIRect& rect)
    {
        const auto& callback = context.getStyle<UIButton>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }
        else
        {
            renderScale9Component(context, this, rect, scale9Component);
            renderTextComponent(context, this, rect, textComponent);
        }
    }

    void UIPanel::renderControl(const UIContext& context, const sUIRect& rect)
    {
        const auto& callback = context.getStyle<UIPanel>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }
        else
        {
            context.drawRect(this, rect, color);
        }
    }

    void UILabel::renderControl(const UIContext& context, const sUIRect& rect)
    {
        const auto& callback = context.getStyle<UILabel>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }
        else
        {
            renderTextComponent(context, this, rect, textComponent);
        }
    }

    void UIImage::renderControl(const UIContext& context, const sUIRect& rect)
    {
        const auto& callback = context.getStyle<UIImage>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }
        else
        {
            renderScale9Component(context, this, rect, scale9Component);
        }
    }

    void UICheckBox::renderControl(const UIContext& context, const sUIRect& rect)
    {
        const auto& callback = context.getStyle<UICheckBox>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }
        else
        {
            renderTextComponent(context, this, rect, textComponent);
        }
    }

    void UITreeView::renderControl(const UIContext& context, const sUIRect& rect)
    {
        const auto& callback = context.getStyle<UITreeView>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }

        // Render it's items
        sUIRect itemRect = {rect.position, {rect.size.x, itemHeight}};
        itemRect.position.y -= m_scroll;
        const auto& itemCallback = context.getStyle<UITreeViewItem>(getStyle());
        if (itemCallback)
        {
            for (auto pItem : m_items)
            {
                pItem->render(itemCallback, this, rect, itemRect);
            }
        }

        if (allowReorder && m_isDragging && !m_selectedItems.empty())
        {
            auto pItem = m_selectedItems.front();
            itemRect.position = m_dragMousePos;
            itemRect.position.y += 8.f;
            itemRect.position.x += 8.f;
            pItem->renderDrag(itemCallback, this, rect, itemRect);
            if (m_dragAfterItem || m_dragBeforeItem)
            {
                if (context.drawInsert)
                {
                    context.drawInsert(m_dragInBetweenRect);
                }
            }
        }
    }

    void UITextBox::renderControl(const UIContext& context, const sUIRect& rect)
    {
        const auto& callback = context.getStyle<UITextBox>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }
        else
        {
            renderScale9Component(context, this, rect, scale9Component);

            if (isCursorVisible() && hasFocus(context))
            {
                auto prevText = textComponent.text;
                textComponent.text = prevText + "_";
                renderTextComponent(context, this, rect, textComponent);
                textComponent.text = prevText;
            }
            else
            {
                renderTextComponent(context, this, rect, textComponent);
            }
        }
    }

    //--- Internal events
    void UICheckBox::onClickInternal(const UIMouseEvent& evt)
    {
        switch (behavior)
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
            case eUICheckBehavior::CHK_OPTIONAL:
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
                                if (pSibblingCheckBox->behavior == behavior)
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
                                if (pSibblingCheckBox->behavior == behavior)
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
                pPicked->isExpanded = !pPicked->isExpanded;
            }
            else
            {
                if (!evt.isCtrlDown)
                {
                    unselectAll();
                    addSelectedItem(pPicked);
                }
                else
                {
                    bool found = false;
                    for (auto it = m_selectedItems.begin(); it != m_selectedItems.end(); ++it)
                    {
                        auto pItem = *it;
                        if (pItem == pPicked)
                        {
                            m_selectedItems.erase(it);
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        addSelectedItem(pPicked);
                    }
                }
                if (onSelectionChanged)
                {
                    UITreeViewSelectEvent myEvt;
                    myEvt.pContext = evt.pContext;
                    myEvt.pSelectedItems = &m_selectedItems;
                    onSelectionChanged(this, myEvt);
                }
            }
        }
        if (allowReorder)
        {
            m_mousePosOnDragStart = evt.mousePos;
        }
    }

    void UITreeView::onMouseMoveInternal(const UIMouseEvent& evt)
    {
        if (hasFocus(*evt.pContext) && evt.isMouseDown)
        {
            if (allowReorder)
            {
                if (!m_isDragging)
                {
                    if (std::abs(m_mousePosOnDragStart.y - evt.mousePos.y) >= 3)
                    {
                        m_isDragging = true;
                    }
                }
                else
                {
                    m_dragMousePos = evt.mousePos;
                    auto worldRect = getWorldRect(*evt.pContext);
                    bool pickedExpandButton = false;
                    m_dragBeforeItem = nullptr;
                    m_dragAfterItem = nullptr;
                    auto pPicked = getItemAtPosition(evt.mousePos, worldRect, &pickedExpandButton, &m_dragInBetweenRect);
                    if (pPicked)
                    {
                        if (m_dragHoverItem)
                        {
                            m_dragHoverItem->m_isSelected = false;
                            m_dragHoverItem = nullptr;
                        }
                        if (evt.mousePos.y - m_dragInBetweenRect.position.y < itemHeight / 4)
                        {
                            m_dragBeforeItem = pPicked;
                            m_dragInBetweenRect.size.y = 2.f;
                            m_dragInBetweenRect.position.y -= 1.f;
                        }
                        else if (evt.mousePos.y > m_dragInBetweenRect.position.y + m_dragInBetweenRect.size.y - itemHeight / 4)
                        {
                            m_dragAfterItem = pPicked;
                            m_dragInBetweenRect.position.y += m_dragInBetweenRect.size.y - 1.f;
                            m_dragInBetweenRect.size.y = 2.f;
                        }
                        if (!pPicked->getIsSelected() && !m_dragBeforeItem && !m_dragAfterItem)
                        {
                            m_dragHoverItem = pPicked;
                            m_dragHoverItem->m_isSelected = true;
                        }
                    }
                }
            }
        }
    }

    void UITreeView::onMouseUpInternal(const UIMouseEvent& evt)
    {
        if (!m_isDragging) return;

        auto dragHover = m_dragHoverItem;
        auto dragBefore = m_dragBeforeItem;
        auto dragAfter = m_dragAfterItem;

        m_isDragging = false;
        if (m_dragHoverItem)
        {
            m_dragHoverItem->m_isSelected = false;
        }
        m_dragHoverItem = nullptr;
        m_dragBeforeItem = nullptr;
        m_dragAfterItem = nullptr;

        if (dragHover)
        {
            for (auto pItem : m_selectedItems)
            {
                dragHover->addItem(pItem);
            }
            dragHover->isExpanded = true;
            if (onMoveItemInto)
            {
                UITreeViewMoveEvent myEvt;
                myEvt.pContext = evt.pContext;
                myEvt.pTarget = dragHover;
                myEvt.pSelectedItems = &m_selectedItems;
                onMoveItemInto(this, myEvt);
            }
        }
        else if (dragBefore)
        {
            int modified = 0;
            for (auto pItem : m_selectedItems)
            {
                if (pItem == dragBefore) continue;
                if (dragBefore->m_pParent)
                {
                    dragBefore->m_pParent->addItemBefore(pItem, dragBefore);
                    ++modified;
                }
            }
            dragBefore->isExpanded = true;
            if (onMoveItemBefore && modified)
            {
                UITreeViewMoveEvent myEvt;
                myEvt.pContext = evt.pContext;
                myEvt.pTarget = dragBefore;
                myEvt.pSelectedItems = &m_selectedItems;
                onMoveItemBefore(this, myEvt);
            }
        }
        else if (dragAfter)
        {
            int modified = 0;
            for (auto pItem : m_selectedItems)
            {
                if (pItem == dragAfter) continue;
                if (dragAfter->m_pParent)
                {
                    dragAfter->m_pParent->addItemAfter(pItem, dragAfter);
                    ++modified;
                }
            }
            dragAfter->isExpanded = true;
            if (onMoveItemAfter && modified)
            {
                UITreeViewMoveEvent myEvt;
                myEvt.pContext = evt.pContext;
                myEvt.pTarget = dragAfter;
                myEvt.pSelectedItems = &m_selectedItems;
                onMoveItemAfter(this, myEvt);
            }
        }
    }

    void UITreeView::onMouseScrollInternal(const UIMouseEvent& evt)
    {
        m_scroll -= evt.scroll;
        auto contentSize = getTotalHeight();
        auto worldRect = getWorldRect(*evt.pContext);
        auto scrollMaxSize = contentSize - worldRect.size.y;
        if (m_scroll > scrollMaxSize) m_scroll = scrollMaxSize;
        if (m_scroll < 0) m_scroll = 0.f;
    }

    void UITextBox::onGainFocusInternal(const UIFocusEvent& evt)
    {
        // Select all on focus
        selectAll();
        m_cursorTime = std::chrono::steady_clock::now();
        m_isTextChanged = false;
    }

    void UITextBox::onLoseFocusInternal(const UIFocusEvent& evt)
    {
        // Deselect all
        m_selectedTextRegion[0] = 0;
        m_selectedTextRegion[1] = 0;

        // Accept
        if (m_isTextChanged)
        {
            m_isTextChanged = false;
            numerifyText();
            if (onTextChanged)
            {
                UITextBoxEvent evt2;
                evt2.pContext = evt.pContext;
                onTextChanged(this, evt2);
            }
        }
    }

    void UITextBox::onMouseDownInternal(const UIMouseEvent& evt)
    {
        m_isSelecting = true;
        if (getIsNumerical())
        {
            m_mousePosOnDown = evt.mousePos.y;
            m_valueOnDown = getFloat();
        }
        else
        {
            const auto& callback = evt.pContext->getTextCaretSolver<UITextBox>(getStyle());
            if (callback)
            {
                auto caretPos = callback->getCaretPos(this, evt.localMousePos);
                m_cursorPos = m_selectedTextRegion[0] = m_selectedTextRegion[1] = caretPos;
            }
        }
    }

    void UITextBox::onMouseMoveInternal(const UIMouseEvent& evt)
    {
        if (!m_isSelecting) return; // Don't care
        if (getIsNumerical())
        {
            if (!m_isSpinning)
            {
                auto mouseDiff = evt.mousePos.y - m_mousePosOnDown;
                if (std::abs(mouseDiff) >= 3.f)
                {
                    m_isSpinning = true;
                    m_mousePosOnDown = evt.mousePos.y;
                    if (onNumberSpinStart)
                    {
                        UITextBoxEvent evt2;
                        evt2.pContext = evt.pContext;
                        onNumberSpinStart(this, evt2);
                    }
                }
            }
            if (m_isSpinning)
            {
                auto mouseDiff = evt.mousePos.y - m_mousePosOnDown;
                auto valueDiff = mouseDiff * -step;
                setFloat(m_valueOnDown + valueDiff);
                selectAll();
                if (onTextChanged)
                {
                    UITextBoxEvent evt2;
                    evt2.pContext = evt.pContext;
                    onTextChanged(this, evt2);
                }
            }
        }
        else
        {
            const auto& callback = evt.pContext->getTextCaretSolver<UITextBox>(getStyle());
            if (callback)
            {
                auto caretPos = callback->getCaretPos(this, evt.localMousePos);
                m_cursorPos = caretPos;
                m_selectedTextRegion[0] = std::min<>(m_cursorPos, m_selectedTextRegion[0]);
                m_selectedTextRegion[1] = std::max<>(m_cursorPos, m_selectedTextRegion[1]);
            }
        }
    }

    void UITextBox::onMouseUpInternal(const UIMouseEvent& evt)
    {
        m_isSelecting = false;
        if (m_isSpinning)
        {
            m_isSpinning = false;
            if (onNumberSpinEnd)
            {
                UITextBoxEvent evt2;
                evt2.pContext = evt.pContext;
                onNumberSpinEnd(this, evt2);
                evt.pContext->focus(nullptr);
            }
        }
    }

    void UITextBox::selectAll()
    {
        m_selectedTextRegion[0] = 0;
        m_selectedTextRegion[1] = textComponent.text.size();
        m_cursorPos = m_selectedTextRegion[1];
    }

    void UITextBox::onWriteInternal(char c, UIContext& context)
    {
        m_cursorTime = std::chrono::steady_clock::now();
        m_isTextChanged = true;
        const auto& callback = context.getStyle<UITextBox>(getStyle());
        if (!callback)
        {
            m_selectedTextRegion[1] = m_selectedTextRegion[0] = m_cursorPos;
        }
        if (c == '\b') // Backspace
        {
            if (m_selectedTextRegion[1] - m_selectedTextRegion[0])
            {
                textComponent.text = textComponent.text.substr(0, m_selectedTextRegion[0]) + textComponent.text.substr(m_selectedTextRegion[1]);
                m_cursorPos = m_selectedTextRegion[1] = m_selectedTextRegion[0];
            }
            else if (m_cursorPos)
            {
                textComponent.text = textComponent.text.substr(0, m_cursorPos - 1) + textComponent.text.substr(m_cursorPos);
                m_cursorPos = m_selectedTextRegion[0] = m_selectedTextRegion[1] = m_cursorPos - 1;
            }
        }
        else if (c == '\r') // Return
        {
            context.focus(nullptr);
            release();
        }
        else if (c == '\x1b') // Escape
        {
            context.focus(nullptr);
            release();
        }
        else if (c == '\t')
        {
            // We go to the next textfield
            auto pParent = getParent();
            UITextBox *pNext = nullptr;
            if (pParent)
            {
                float closest[2] = {100000.f, 100000.f};
                auto myRect = getWorldRect(context);
                for (auto pChild : pParent->getChildren())
                {
                    if (pChild->getType() == eUIType::UI_TEXTBOX &&
                        pChild != this)
                    {
                        auto childRect = pChild->getWorldRect(context);
                        float dists[2] = {
                            childRect.position.x - myRect.position.x,
                            childRect.position.y - myRect.position.y};
                        if (dists[0] <= -5.f || dists[1] <= -5.f) continue;
                        if (dists[1] < closest[1])
                        {
                            closest[0] = dists[0];
                            closest[1] = dists[1];
                            pNext = dynamic_cast<UITextBox *>(pChild);
                            continue;
                        }
                        if (dists[0] < closest[0] &&
                            dists[1] < closest[1] + 5.f)
                        {
                            closest[0] = dists[0];
                            closest[1] = dists[1];
                            pNext = dynamic_cast<UITextBox *>(pChild);
                            continue;
                        }
                    }
                }
            }
            if (pNext)
            {
                pNext->retain();
                context.focus(pNext);
                release();
            }
            else
            {
                context.focus(nullptr);
                release();
            }
        }
        else // Normal character
        {
            std::stringstream ss;
            ss << textComponent.text.substr(0, m_selectedTextRegion[0]);
            ss << c;
            ss << textComponent.text.substr(m_selectedTextRegion[1]);
            textComponent.text = ss.str();
            m_cursorPos = m_selectedTextRegion[0] = m_selectedTextRegion[1] = m_selectedTextRegion[0] + 1;
        }
    }

    void UITextBox::onKeyDownInternal(const UIKeyEvent& evt)
    {
        m_cursorTime = std::chrono::steady_clock::now();
        const auto& callback = evt.pContext->getStyle<UITextBox>(getStyle());
        switch (evt.key)
        {
            case KEY_END:
                if (callback)
                {
                    m_cursorPos = m_selectedTextRegion[0] = m_selectedTextRegion[1] = textComponent.text.size();
                }
                break;
            case KEY_HOME:
                if (callback)
                {
                    m_cursorPos = m_selectedTextRegion[0] = m_selectedTextRegion[1] = 0;
                }
                break;
            case KEY_LEFT:
            case KEY_UP:
                if (callback)
                {
                    if (m_cursorPos > 0)
                    {
                        --m_cursorPos;
                        m_selectedTextRegion[0] = m_selectedTextRegion[1] = m_cursorPos;
                    }
                }
                break;
            case KEY_RIGHT:
            case KEY_DOWN:
                if (callback)
                {
                    if (m_cursorPos < textComponent.text.size())
                    {
                        ++m_cursorPos;
                        m_selectedTextRegion[0] = m_selectedTextRegion[1] = m_cursorPos;
                    }
                }
                break;
            case KEY_DELETE:
                m_isTextChanged = true;
                if (!callback)
                {
                    selectAll();
                }
                if (m_selectedTextRegion[0] != m_selectedTextRegion[1])
                {
                    textComponent.text = textComponent.text.substr(0, m_selectedTextRegion[0]) + textComponent.text.substr(m_selectedTextRegion[1]);
                    m_cursorPos = m_selectedTextRegion[1] = m_selectedTextRegion[0];
                }
                else if (m_cursorPos < textComponent.text.size())
                {
                    textComponent.text = textComponent.text.substr(0, m_cursorPos) + textComponent.text.substr(m_cursorPos + 1);
                }
                break;
        }
    }
};
