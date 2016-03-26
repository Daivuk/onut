// Private
#include "UIJson.h"

// STL
#include <unordered_map>

namespace onut
{
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

    static std::unordered_map<std::string, UIFontComponent::Flag> fontFlagMap = {
        {"AUTO_FIT_SIZE", UIFontComponent::AutoFitSize},
        {"ELLIPSIS", UIFontComponent::Ellipsis},
        {"WORD_WRAP", UIFontComponent::WordWrap}
    };

    const char* getJsonString(const rapidjson::Value& jsonNode, const char* szDefault)
    {
        if (jsonNode.IsString()) return jsonNode.GetString();
        else return szDefault;
    }

    float getJsonFloat(const rapidjson::Value& jsonNode, const float default)
    {
        if (jsonNode.IsNumber()) return static_cast<float>(jsonNode.GetDouble());
        else return default;
    }

    int getJsonInt(const rapidjson::Value& jsonNode, const int default)
    {
        if (jsonNode.IsInt()) return jsonNode.GetInt();
        else return default;
    }

    bool getJsonBool(const rapidjson::Value& jsonNode, const bool default)
    {
        if (jsonNode.IsBool()) return jsonNode.GetBool();
        else return default;
    }

    void setJsonFloat(rapidjson::Value& jsonNode, const char* szName, float value, rapidjson::Allocator& allocator, float default)
    {
        if (value == default) return;
        jsonNode.AddMember(szName, value, allocator);
    }

    void setJsonInt(rapidjson::Value& jsonNode, const char* szName, int value, rapidjson::Allocator& allocator, int default)
    {
        if (value == default) return;
        jsonNode.AddMember(szName, value, allocator);
    }

    void setJsonString(rapidjson::Value& jsonNode, const char* szName, const char* szValue, rapidjson::Allocator& allocator, const char* default)
    {
        if (!strcmp(szValue, default)) return;
        jsonNode.AddMember(szName, szValue, allocator);
    }

    void setJsonString(rapidjson::Value& jsonNode, const char* szName, const std::string& value, rapidjson::Allocator& allocator, const char* default)
    {
        if (!strcmp(value.c_str(), default)) return;
        rapidjson::Value jsonValue;
        jsonValue.SetString(value.c_str(), allocator);
        jsonNode.AddMember(szName, jsonValue, allocator);
    }

    void setJsonBool(rapidjson::Value& jsonNode, const char* szName, const bool value, rapidjson::Allocator& allocator, bool default)
    {
        if (value == default) return;
        jsonNode.AddMember(szName, value, allocator);
    }

    void setJsonColor(rapidjson::Value& jsonNode,
                             const char* szName,
                             const Color& value,
                             rapidjson::Allocator& allocator,
                             const Color& default)
    {
        if (value.pack() == default.pack()) return;

        rapidjson::Value jsonValue;
        char buffer[10];
        int len = sprintf_s(buffer, "%08x", value.pack()); // dynamically created string.
        jsonValue.SetString(buffer, len, allocator);
        jsonNode.AddMember(szName, jsonValue, allocator);
    }

    Color getJsonColor(const rapidjson::Value& jsonNode, const Color& default)
    {
        if (jsonNode.IsString())
        {
            Color ret;
            auto szStr = jsonNode.GetString();
            auto len = jsonNode.GetStringLength();
            if (len != 8)
            {
                return default;
            }
            ret.unpack(static_cast<uint32_t>(strtoul(szStr, NULL, 16)));
            return std::move(ret);
        }
        else
        {
            return default;
        }
    }

    Vector4 getJsonPadding(const rapidjson::Value& node)
    {
        Vector4 ret;
        if (!node.IsNull())
        {
            ret.x = getJsonFloat(node["left"]);
            ret.z = getJsonFloat(node["right"]);
            ret.y = getJsonFloat(node["top"]);
            ret.w = getJsonFloat(node["bottom"]);
        }
        return std::move(ret);
    }

    void setJsonPadding(rapidjson::Value& node, const Vector4& padding, rapidjson::Allocator& allocator)
    {
        rapidjson::Value paddingNode(rapidjson::kObjectType);
        setJsonFloat(paddingNode, "left", padding.x, allocator);
        setJsonFloat(paddingNode, "right", padding.z, allocator);
        setJsonFloat(paddingNode, "top", padding.y, allocator);
        setJsonFloat(paddingNode, "bottom", padding.w, allocator);
        node.AddMember("padding", paddingNode, allocator);
    }

    UIFontComponent getJsonFont(const rapidjson::Value& node)
    {
        UIFontComponent ret;
        if (!node.IsNull())
        {
            ret.color = getJsonColor(node["color"]);
            ret.align = getJsonEnum(alignMap, node["align"], onut::Align::TopLeft);
            ret.padding = getJsonPadding(node["padding"]);
            ret.typeFace = getJsonString(node["typeFace"], "Arial");
            ret.size = getJsonFloat(node["size"], 12.f);
            ret.flags = getJsonBitmask(fontFlagMap, node["flags"]);
            ret.minSize = getJsonFloat(node["minSize"], 12.f);
        }
        return std::move(ret);
    }

    void setJsonFont(rapidjson::Value& node, const UIFontComponent& font, rapidjson::Allocator& allocator)
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

    UITextComponent getJsonTextComponent(const rapidjson::Value& node)
    {
        UITextComponent ret;
        if (!node.IsNull())
        {
            ret.text = getJsonString(node["text"]);
            ret.font = getJsonFont(node["font"]);
        }
        return std::move(ret);
    }

    UIImageComponent getJsonImageComponent(const rapidjson::Value& node)
    {
        UIImageComponent ret;
        if (!node.IsNull())
        {
            ret.filename = getJsonString(node["filename"]);
            ret.color = getJsonColor(node["color"]);
        }
        return std::move(ret);
    }

    UIScale9Component getJsonScale9Component(const rapidjson::Value& node)
    {
        UIScale9Component scale9Component;
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

    void setJsonImageComponent(rapidjson::Value& node, const UIImageComponent& imageComponent, rapidjson::Allocator& allocator)
    {
        rapidjson::Value imageNode(rapidjson::kObjectType);
        setJsonString(imageNode, "filename", imageComponent.filename, allocator);
        setJsonColor(imageNode, "color", imageComponent.color, allocator);
        node.AddMember("imageComponent", imageNode, allocator);
    }

    void setJsonScale9Component(rapidjson::Value& node, const UIScale9Component& scale9Component, rapidjson::Allocator& allocator)
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
        if (scale9Component.image.color != Color::White)
        {
            setJsonImageComponent(node, scale9Component.image, allocator);
        }
        else
        {
            setJsonString(node, "image", scale9Component.image.filename, allocator);
        }
    }

    void setJsonTextComponent(rapidjson::Value& node, const UITextComponent& textComponent, rapidjson::Allocator& allocator)
    {
        rapidjson::Value textNode(rapidjson::kObjectType);
        setJsonString(textNode, "text", textComponent.text, allocator);
        setJsonFont(textNode, textComponent.font, allocator);
        node.AddMember("textComponent", textNode, allocator);
    }
};
