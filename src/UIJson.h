#pragma once
// Onut
#include <onut/Maths.h>
#include <onut/UIComponents.h>

// Third party
#include <rapidjson/document.h>
#include <rapidjson/filestream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

// Forward
namespace rapidjson
{
    typedef GenericValue<UTF8<char>, MemoryPoolAllocator<CrtAllocator>> Value;
    typedef MemoryPoolAllocator<CrtAllocator> Allocator;
}

namespace onut
{
    const char* getJsonString(const rapidjson::Value& jsonNode, const char* szDefault = "");
    float getJsonFloat(const rapidjson::Value& jsonNode, const float default = 0.f);
    int getJsonInt(const rapidjson::Value& jsonNode, const int default = 0);
    bool getJsonBool(const rapidjson::Value& jsonNode, const bool default = false);
    void setJsonFloat(rapidjson::Value& jsonNode, const char* szName, float value, rapidjson::Allocator& allocator, float default = 0.f);
    void setJsonInt(rapidjson::Value& jsonNode, const char* szName, int value, rapidjson::Allocator& allocator, int default = 0);
    void setJsonString(rapidjson::Value& jsonNode, const char* szName, const char* szValue, rapidjson::Allocator& allocator, const char* default = "");
    void setJsonString(rapidjson::Value& jsonNode, const char* szName, const std::string& value, rapidjson::Allocator& allocator, const char* default = "");
    void setJsonBool(rapidjson::Value& jsonNode, const char* szName, const bool value, rapidjson::Allocator& allocator, bool default = false);
    void setJsonColor(rapidjson::Value& jsonNode, const char* szName, const Color& value, rapidjson::Allocator& allocator, const Color& default = {1.f, 1.f, 1.f, 1.f});
    Color getJsonColor(const rapidjson::Value& jsonNode, const Color& default = {1.f, 1.f, 1.f, 1.f});
    Vector4 getJsonPadding(const rapidjson::Value& node);
    void setJsonPadding(rapidjson::Value& node, const Vector4& padding, rapidjson::Allocator& allocator);
    UIFontComponent getJsonFont(const rapidjson::Value& node);
    void setJsonFont(rapidjson::Value& node, const UIFontComponent& font, rapidjson::Allocator& allocator);
    UITextComponent getJsonTextComponent(const rapidjson::Value& node);
    UIImageComponent getJsonImageComponent(const rapidjson::Value& node);
    UIScale9Component getJsonScale9Component(const rapidjson::Value& node);
    void setJsonImageComponent(rapidjson::Value& node, const UIImageComponent& imageComponent, rapidjson::Allocator& allocator);
    void setJsonScale9Component(rapidjson::Value& node, const UIScale9Component& scale9Component, rapidjson::Allocator& allocator);
    void setJsonTextComponent(rapidjson::Value& node, const UITextComponent& textComponent, rapidjson::Allocator& allocator);

    template <typename Tmap, typename Tenum>
    Tenum getJsonEnum(const Tmap& enumMap, const rapidjson::Value& jsonNode, Tenum default)
    {
        Tenum ret = default;
        if (jsonNode.IsString())
        {
            ret = stringToEnum(enumMap, jsonNode.GetString(), default);
        }
        return ret;
    }

    template<typename Tmap, typename Tenum>
    Tenum stringToEnum(const Tmap& map, const std::string& key, Tenum default)
    {
        auto it = map.find(key);
        if (it == map.end()) return default;
        return it->second;
    }

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


    template <typename Tmap, typename Tenum = Tmap::mapped_type>
    Tenum getJsonBitmask(const Tmap& enumMap, const rapidjson::Value& jsonNode)
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
    void setJsonBitmask(rapidjson::Value& node, const char* szName, const Tmap& enumMap, Tenum value, rapidjson::Allocator& allocator)
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
};
