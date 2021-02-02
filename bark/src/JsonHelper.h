#pragma once

#include <onut/ForwardDeclaration.h>
#include <onut/Vector2.h>
#include <onut/Vector3.h>
#include <onut/Vector4.h>
#include <onut/Rect.h>
#include <onut/Color.h>
#include <onut/iRect.h>
#include <json/json.h>
#include <string>

OForwardDeclare(Texture);
OForwardDeclare(TiledMap);
OForwardDeclare(Sound);
OForwardDeclare(SpriteAnim);
OForwardDeclare(Model);
OForwardDeclare(Shader);
ForwardDeclare(Script);

bool getJson_bool(const Json::Value& json, const char* key, bool default_value = false);
int getJson_int(const Json::Value& json, const char* key, int default_value = 0);
float getJson_float(const Json::Value& json, const char* key, float default_value = 0.0f);
std::string getJson_std_string(const Json::Value& json, const char* key, const std::string& default_value = "");
Vector2 getJson_Vector2(const Json::Value& json, const char* key, const Vector2& default_value = Vector2::Zero);
Vector3 getJson_Vector3(const Json::Value& json, const char* key, const Vector3& default_value = Vector3::Zero);
Vector4 getJson_Vector4(const Json::Value& json, const char* key, const Vector4& default_value = Vector4::Zero);
Rect getJson_Rect(const Json::Value& json, const char* key, const Rect& default_value = Rect::Zero);
iRect getJson_iRect(const Json::Value& json, const char* key, const iRect& default_value = {0, 0, 0, 0});
Color getJson_Color(const Json::Value& json, const char* key, const Color& default_value = Color::White);
OTextureRef getJson_OTextureRef(const Json::Value& json, const char* key, const OTextureRef& default_value = nullptr);
OTiledMapRef getJson_OTiledMapRef(const Json::Value& json, const char* key, const OTiledMapRef& default_value = nullptr);
OSoundRef getJson_OSoundRef(const Json::Value& json, const char* key, const OSoundRef& default_value = nullptr);
OSpriteAnimRef getJson_OSpriteAnimRef(const Json::Value& json, const char* key, const OSpriteAnimRef& default_value = nullptr);
OModelRef getJson_OModelRef(const Json::Value& json, const char* key, const OModelRef& default_value = nullptr);
OShaderRef getJson_OShaderRef(const Json::Value& json, const char* key, const OShaderRef& default_value = nullptr);
ScriptRef getJson_ScriptRef(const Json::Value& json, const char* key, const ScriptRef& default_value = nullptr);

#if BARK_EDITOR
void setJson_bool(const Json::Value& json, const char* key, bool value);
void setJson_int(const Json::Value& json, const char* key, int value);
void setJson_float(const Json::Value& json, const char* key, float value);
void setJson_std_string(const Json::Value& json, const char* key, const std::string& value);
void setJson_Vector2(const Json::Value& json, const char* key, const Vector2& value);
void setJson_Vector3(const Json::Value& json, const char* key, const Vector3& value);
void setJson_Vector4(const Json::Value& json, const char* key, const Vector4& value);
void setJson_Rect(const Json::Value& json, const char* key, const Rect& value);
void setJson_iRect(const Json::Value& json, const char* key, const iRect& value);
void setJson_Color(const Json::Value& json, const char* key, const Color& value);
void setJson_OTextureRef(const Json::Value& json, const char* key, const OTextureRef& value);
void setJson_OTiledMapRef(const Json::Value& json, const char* key, const OTiledMapRef& value);
void setJson_OSoundRef(const Json::Value& json, const char* key, const OSoundRef& value);
void setJson_OSpriteAnimRef(const Json::Value& json, const char* key, const OSpriteAnimRef& value);
void setJson_OModelRef(const Json::Value& json, const char* key, const OModelRef& value);
void setJson_OShaderRef(const Json::Value& json, const char* key, const OShaderRef& value);
void setJson_ScriptRef(const Json::Value& json, const char* key, const ScriptRef& value);
#endif
