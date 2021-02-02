#include "JsonHelper.h"
#include <onut/ContentManager.h>
#include <onut/Texture.h>
#include <onut/TiledMap.h>
#include <onut/Sound.h>
#include <onut/SpriteAnim.h>
#include <onut/Model.h>
#include <onut/Shader.h>
#include "Script.h"

bool getJson_bool(const Json::Value& json, const char* key, bool default_value)
{
    if (!json.isMember(key)) return default_value;
    return json[key].asBool();
}

int getJson_int(const Json::Value& json, const char* key, int default_value)
{
    if (!json.isMember(key)) return default_value;
    return json[key].asInt();
}

float getJson_float(const Json::Value& json, const char* key, float default_value)
{
    if (!json.isMember(key)) return default_value;
    return json[key].asFloat();
}

std::string getJson_std_string(const Json::Value& json, const char* key, const std::string& default_value)
{
    if (!json.isMember(key)) return default_value;
    return json[key].asString();
}

Vector2 getJson_Vector2(const Json::Value& json, const char* key, const Vector2& default_value)
{
    if (!json.isMember(key)) return default_value;
    const auto& json_val = json[key];
    Vector2 ret;
    ret.x = json_val["x"].asFloat();
    ret.y = json_val["y"].asFloat();
    return ret;
}

Vector3 getJson_Vector3(const Json::Value& json, const char* key, const Vector3& default_value)
{
    if (!json.isMember(key)) return default_value;
    const auto& json_val = json[key];
    Vector3 ret;
    ret.x = json_val["x"].asFloat();
    ret.y = json_val["y"].asFloat();
    ret.z = json_val["z"].asFloat();
    return ret;
}

Vector4 getJson_Vector4(const Json::Value& json, const char* key, const Vector4& default_value)
{
    if (!json.isMember(key)) return default_value;
    const auto& json_val = json[key];
    Vector4 ret;
    ret.x = json_val["x"].asFloat();
    ret.y = json_val["y"].asFloat();
    ret.z = json_val["z"].asFloat();
    ret.w = json_val["w"].asFloat();
    return ret;
}

Rect getJson_Rect(const Json::Value& json, const char* key, const Rect& default_value)
{
    if (!json.isMember(key)) return default_value;
    const auto& json_val = json[key];
    Rect ret;
    ret.x = json_val["x"].asFloat();
    ret.y = json_val["y"].asFloat();
    ret.z = json_val["w"].asFloat();
    ret.w = json_val["h"].asFloat();
    return ret;
}

iRect getJson_iRect(const Json::Value& json, const char* key, const iRect& default_value)
{
    if (!json.isMember(key)) return default_value;
    const auto& json_val = json[key];
    iRect ret;
    ret.left = json_val["left"].asInt();
    ret.top = json_val["top"].asInt();
    ret.right = json_val["right"].asInt();
    ret.bottom = json_val["bottom"].asInt();
    return ret;
}

Color getJson_Color(const Json::Value& json, const char* key, const Color& default_value)
{
    if (!json.isMember(key)) return default_value;
    const auto& json_val = json[key];
    Color ret;
    ret.r = json_val["r"].asFloat();
    ret.g = json_val["g"].asFloat();
    ret.b = json_val["b"].asFloat();
    ret.a = json_val["a"].asFloat();
    return ret;
}

OTextureRef getJson_OTextureRef(const Json::Value& json, const char* key, const OTextureRef& default_value)
{
    if (!json.isMember(key)) return default_value;
    return OGetTexture(json[key].asString());
}

OTiledMapRef getJson_OTiledMapRef(const Json::Value& json, const char* key, const OTiledMapRef& default_value)
{
    if (!json.isMember(key)) return default_value;
    return OGetTiledMap(json[key].asString());
}

OSoundRef getJson_OSoundRef(const Json::Value& json, const char* key, const OSoundRef& default_value)
{
    if (!json.isMember(key)) return default_value;
    return OGetSound(json[key].asString());
}

OSpriteAnimRef getJson_OSpriteAnimRef(const Json::Value& json, const char* key, const OSpriteAnimRef& default_value)
{
    if (!json.isMember(key)) return default_value;
    return OGetSpriteAnim(json[key].asString());
}

OModelRef getJson_OModelRef(const Json::Value& json, const char* key, const OModelRef& default_value)
{
    if (!json.isMember(key)) return default_value;
    return OGetModel(json[key].asString());
}

OShaderRef getJson_OShaderRef(const Json::Value& json, const char* key, const OShaderRef& default_value)
{
    if (!json.isMember(key)) return default_value;
    return OGetShader(json[key].asString());
}

ScriptRef getJson_ScriptRef(const Json::Value& json, const char* key, const ScriptRef& default_value)
{
    if (!json.isMember(key)) return default_value;
    return oContentManager->getResourceAs<Script>(json[key].asString());
}

#if BARK_EDITOR

void setJson_bool(const Json::Value& json, const char* key, bool value)
{
    json[key] = value;
}

void setJson_int(const Json::Value& json, const char* key, int value)
{
    json[key] = value;
}

void setJson_float(const Json::Value& json, const char* key, float value)
{
    json[key] = value;
}

void setJson_std_string(const Json::Value& json, const char* key, const std::string& value)
{
    json[key] = value;
}

void setJson_Vector2(const Json::Value& json, const char* key, const Vector2& value)
{
    json[key]["x"] = value.x;
    json[key]["y"] = value.y;
}

void setJson_Vector3(const Json::Value& json, const char* key, const Vector3& value)
{
    json[key]["x"] = value.x;
    json[key]["y"] = value.y;
    json[key]["z"] = value.z;
}

void setJson_Vector4(const Json::Value& json, const char* key, const Vector4& value)
{
    json[key]["x"] = value.x;
    json[key]["y"] = value.y;
    json[key]["z"] = value.z;
    json[key]["w"] = value.w;
}

void setJson_Rect(const Json::Value& json, const char* key, const Rect& value)
{
    json[key]["x"] = value.x;
    json[key]["y"] = value.y;
    json[key]["w"] = value.z;
    json[key]["h"] = value.w;
}

void setJson_iRect(const Json::Value& json, const char* key, const iRect& value)
{
    json[key]["left"] = value.left;
    json[key]["top"] = value.top;
    json[key]["right"] = value.right;
    json[key]["bottom"] = value.bottom;
}

void setJson_Color(const Json::Value& json, const char* key, const Color& value)
{
    json[key]["r"] = value.r;
    json[key]["g"] = value.g;
    json[key]["b"] = value.b;
    json[key]["a"] = value.a;
}

void setJson_OTextureRef(const Json::Value& json, const char* key, const OTextureRef& value)
{
    json[key] = value->getName();
}

void setJson_OTiledMapRef(const Json::Value& json, const char* key, const OTiledMapRef& value)
{
    json[key] = value->getName();
}

void setJson_OSoundRef(const Json::Value& json, const char* key, const OSoundRef& value)
{
    json[key] = value->getName();
}

void setJsonSpriteAnim(const Json::Value& json, const char* key, const OSpriteAnimRef& value)
{
    json[key] = value->getName();
}

void setJson_OModelRef(const Json::Value& json, const char* key, const OModelRef& value)
{
    json[key] = value->getName();
}

void setJson_OShaderRef(const Json::Value& json, const char* key, const OShaderRef& value)
{
    json[key] = value->getName();
}

void setJson_ScriptRef(const Json::Value& json, const char* key, const ScriptRef& value)
{
    json[key] = value->getName();
}

#endif
