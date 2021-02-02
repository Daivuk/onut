#include "JSTypeHelper.h"
#include <onut/ContentManager.h>
#include <onut/Texture.h>
#include <onut/TiledMap.h>
#include <onut/Sound.h>
#include <onut/SpriteAnim.h>
#include <onut/Model.h>
#include <onut/Shader.h>
#include "Script.h"
using namespace onut::js;

bool getJs_bool(duk_context *ctx, bool default_value)
{
    return JS_BOOL(0, default_value);
}

int getJs_int(duk_context *ctx, int default_value)
{
    return JS_INT(0, default_value);
}

float getJs_float(duk_context *ctx, float default_value)
{
    return JS_FLOAT(0, default_value);
}

std::string getJs_std_string(duk_context *ctx, const std::string& default_value)
{
    return JS_STRING(0, default_value.c_str());
}

Vector2 getJs_Vector2(duk_context *ctx, const Vector2& default_value)
{
    return JS_VECTOR2(0, default_value);
}

Vector3 getJs_Vector3(duk_context *ctx, const Vector3& default_value)
{
    return JS_VECTOR3(0, default_value);
}

Vector4 getJs_Vector4(duk_context *ctx, const Vector4& default_value)
{
    return JS_VECTOR4(0, default_value);
}

Rect getJs_Rect(duk_context *ctx, const Rect& default_value)
{
    return JS_RECT(0, default_value);
}

iRect getJs_iRect(duk_context *ctx, const iRect& default_value)
{
    return JS_iRECT(0, default_value);
}

Color getJs_Color(duk_context *ctx, const Color& default_value)
{
    return JS_COLOR(0, default_value);
}

OTextureRef getJs_OTextureRef(duk_context *ctx, const OTextureRef& default_value)
{
    auto res = JS_TEXTURE(0);
    if (!res) res = default_value;
    return res;
}

OTiledMapRef getJs_OTiledMapRef(duk_context *ctx, const OTiledMapRef& default_value)
{
    auto res = getResource<OTiledMap>(ctx, 0);
    if (!res) res = default_value;
    return res;
}

OSoundRef getJs_OSoundRef(duk_context *ctx, const OSoundRef& default_value)
{
    auto res = getResource<OSound>(ctx, 0);
    if (!res) res = default_value;
    return res;
}

OSpriteAnimRef getJs_OSpriteAnimRef(duk_context *ctx, const OSpriteAnimRef& default_value)
{
    auto res = getResource<OSpriteAnim>(ctx, 0);
    if (!res) res = default_value;
    return res;
}

OModelRef getJs_OModelRef(duk_context *ctx, const OModelRef& default_value)
{
    auto res = getResource<OModel>(ctx, 0);
    if (!res) res = default_value;
    return res;
}

OShaderRef getJs_OShaderRef(duk_context *ctx, const OShaderRef& default_value)
{
    auto res = getResource<OShader>(ctx, 0);
    if (!res) res = default_value;
    return res;
}

ScriptRef getJs_ScriptRef(duk_context *ctx, const ScriptRef& default_value)
{
    auto res = getResource<Script>(ctx, 0);
    if (!res) res = default_value;
    return res;
}


void pushJs_bool(duk_context *ctx, bool value)
{
    duk_push_boolean(ctx, value);
}

void pushJs_int(duk_context *ctx, int value)
{
    duk_push_int(ctx, value);
}

void pushJs_float(duk_context *ctx, float value)
{
    duk_push_number(ctx, (duk_double_t)value);
}

void pushJs_std_string(duk_context *ctx, const std::string& value)
{
    duk_push_string(ctx, value.c_str());
}

void pushJs_Vector2(duk_context *ctx, const Vector2& value)
{
    newVector2(ctx, value);
}

void pushJs_Vector3(duk_context *ctx, const Vector3& value)
{
    newVector3(ctx, value);
}

void pushJs_Vector4(duk_context *ctx, const Vector4& value)
{
    newVector4(ctx, value);
}

void pushJs_Rect(duk_context *ctx, const Rect& value)
{
    newRect(ctx, value);
}

void pushJs_iRect(duk_context *ctx, const iRect& value)
{
    newIRect(ctx, value);
}

void pushJs_Color(duk_context *ctx, const Color& value)
{
    newColor(ctx, value);
}

void pushJs_OTextureRef(duk_context *ctx, const OTextureRef& value)
{
    newTexture(ctx, value);
}

void pushJs_OTiledMapRef(duk_context *ctx, const OTiledMapRef& value)
{
    newTiledMap(ctx, value);
}

void pushJs_OSoundRef(duk_context *ctx, const OSoundRef& value)
{
    newSound(ctx, value);
}

void pushJs_OSpriteAnimRef(duk_context *ctx, const OSpriteAnimRef& value)
{
    newSpriteAnim(ctx, value);
}

void pushJs_OModelRef(duk_context *ctx, const OModelRef& value)
{
    newModel(ctx, value);
}

void pushJs_OShaderRef(duk_context *ctx, const OShaderRef& value)
{
    newShader(ctx, value);
}
