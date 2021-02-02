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
#include <JSBindings_Macros.h>

OForwardDeclare(Texture);
OForwardDeclare(TiledMap);
OForwardDeclare(Sound);
OForwardDeclare(SpriteAnim);
OForwardDeclare(Model);
OForwardDeclare(Shader);
ForwardDeclare(Script);

bool getJs_bool(duk_context *ctx, bool default_value);
int getJs_int(duk_context *ctx, int default_value);
float getJs_float(duk_context *ctx, float default_value);
std::string getJs_std_string(duk_context *ctx, const std::string& default_value);
Vector2 getJs_Vector2(duk_context *ctx, const Vector2& default_value);
Vector3 getJs_Vector3(duk_context *ctx, const Vector3& default_value);
Vector4 getJs_Vector4(duk_context *ctx, const Vector4& default_value);
Rect getJs_Rect(duk_context *ctx, const Rect& default_value);
iRect getJs_iRect(duk_context *ctx, const iRect& default_value);
Color getJs_Color(duk_context *ctx, const Color& default_value);
OTextureRef getJs_OTextureRef(duk_context *ctx, const OTextureRef& default_value);
OTiledMapRef getJs_OTiledMapRef(duk_context *ctx, const OTiledMapRef& default_value);
OSoundRef getJs_OSoundRef(duk_context *ctx, const OSoundRef& default_value);
OSpriteAnimRef getJs_OSpriteAnimRef(duk_context *ctx, const OSpriteAnimRef& default_value);
OModelRef getJs_OModelRef(duk_context *ctx, const OModelRef& default_value);
OShaderRef getJs_OShaderRef(duk_context *ctx, const OShaderRef& default_value);
ScriptRef getJs_ScriptRef(duk_context *ctx, const ScriptRef& default_value);

void pushJs_bool(duk_context *ctx, bool value);
void pushJs_int(duk_context *ctx, int value);
void pushJs_float(duk_context *ctx, float value);
void pushJs_std_string(duk_context *ctx, const std::string& value);
void pushJs_Vector2(duk_context *ctx, const Vector2& value);
void pushJs_Vector3(duk_context *ctx, const Vector3& value);
void pushJs_Vector4(duk_context *ctx, const Vector4& value);
void pushJs_Rect(duk_context *ctx, const Rect& value);
void pushJs_iRect(duk_context *ctx, const iRect& value);
void pushJs_Color(duk_context *ctx, const Color& value);
void pushJs_OTextureRef(duk_context *ctx, const OTextureRef& value);
void pushJs_OTiledMapRef(duk_context *ctx, const OTiledMapRef& value);
void pushJs_OSoundRef(duk_context *ctx, const OSoundRef& value);
void pushJs_OSpriteAnimRef(duk_context *ctx, const OSpriteAnimRef& value);
void pushJs_OModelRef(duk_context *ctx, const OModelRef& value);
void pushJs_OShaderRef(duk_context *ctx, const OShaderRef& value);
void pushJs_ScriptRef(duk_context *ctx, const ScriptRef& value);
