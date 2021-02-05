#include "Script.h"
#include <onut/ContentManager.h>
#include <onut/Files.h>
#include <onut/Strings.h>
#include <regex>
#include "Entity.h"
#include "Component.h"
#include <sstream>
#include "JsonHelper.h"

#include <JSBindings_Macros.h>
namespace onut
{
    namespace js
    {
        extern duk_context* pContext; // Haxor. This context already contains all standard onut bindings
    }
}

using namespace onut::js;

Script::Script() {}

Script::~Script() {}

ScriptRef Script::createFromFile(const std::string& filename, const OContentManagerRef& pContentManager)
{
    auto ctx = onut::js::pContext;
    auto ret = OMake<Script>();

    // Lets do some regex! Replace _init, _update, with filename without extension
    auto name = onut::getFilenameWithoutExtension(filename);
    auto source = onut::getFileString(filename);

    // Remove :Entity typescript syntax. This is allowed for better intellisense.
    source = std::regex_replace(source, std::regex("\\:\\s*Entity\\b"), "");

    // Replace function names with the component name prefix. e.g: player.js -> function player_onUpdate()
#define REPLACE_FUNCTION_NAME(__name__) \
    source = std::regex_replace(source, \
                                std::regex("function\\s+_" #__name__ "\\("), \
                                "function " + name + "_" #__name__ "(");

    REPLACE_FUNCTION_NAME(onCreate)
    REPLACE_FUNCTION_NAME(onEnable)
    REPLACE_FUNCTION_NAME(onUpdate)
    REPLACE_FUNCTION_NAME(onDisable)
    REPLACE_FUNCTION_NAME(onDestroy)

    // Replace _props also with name prefix. e.g: player.js -> player_props
    source = std::regex_replace(source, std::regex("\\b_props\\b"), name + "_props");

    // Compile script
    auto filename_without_path = onut::getFilename(filename);
    duk_push_string(ctx, filename_without_path.c_str());
    if (duk_pcompile_lstring_filename(ctx, 0, source.c_str(), source.size()) != 0)
    {
        logJSStack(ctx, onut::getFilename(filename) + " failed");
        duk_pop(ctx);
        return ret;
    }
    
    // Run it
    if (duk_pcall(ctx, 0) != 0)
    {
        logJSStack(ctx, onut::getFilename(filename) + " failed");
        duk_pop(ctx);
        return ret;
    }
    duk_pop(ctx);

    // Grab callbacks
#define GRAB_FUNCTION_PTR(__name__) \
    if (duk_get_global_string(ctx, (name + "_" #__name__).c_str())) \
    { \
        ret->__name__ ## Ptr = duk_get_heapptr(ctx, -1); \
        duk_pop(ctx); \
    }

    GRAB_FUNCTION_PTR(onCreate)
    GRAB_FUNCTION_PTR(onEnable)
    GRAB_FUNCTION_PTR(onUpdate)
    GRAB_FUNCTION_PTR(onDisable)
    GRAB_FUNCTION_PTR(onDestroy)

    // Grab default props, and store them as json
    if (duk_get_global_string(ctx, (name + "_props").c_str())) // _props
    {
        duk_enum(ctx, -1, 0); // _props, enum
        while (duk_next(ctx, -1, 1)) // _props, enum, key, prop
        {
            auto prop_name = duk_get_string(ctx, -2);

            if (!duk_has_prop_string(ctx, -1, "type"))
            {
                OLogE(std::string("Prop ") + prop_name + " is missing type field");
                duk_pop_2(ctx); // prop, key
                break;
            }
            bool has_default = duk_has_prop_string(ctx, -1, "default");

            Json::Value json_prop;

            duk_get_prop_string(ctx, -1, "type"); // _props, enum, key, prop, type
            std::string type = duk_get_string(ctx, -1);
            duk_pop(ctx); // type

            if (has_default) duk_get_prop_string(ctx, -1, "default"); // _props, enum, key, prop, type, default

            if (type == "bool")
            {
                json_prop["type"] = "bool";
                setJson_bool(json_prop, "default", has_default ? JS_BOOL(-1) : false);
            }
            else if (type == "int")
            {
                json_prop["type"] = "int";
                setJson_int(json_prop, "default", has_default ? JS_INT(-1) : 0);
            }
            else if (type == "number")
            {
                json_prop["type"] = "float";
                setJson_float(json_prop, "default", has_default ? JS_FLOAT(-1) : 0.0f);
            }
            else if (type == "string")
            {
                json_prop["type"] = "std_string";
                setJson_std_string(json_prop, "default", has_default ? JS_STRING(-1) : "");
            }
            else if (type == "Vector2")
            {
                json_prop["type"] = "Vector2";
                setJson_Vector2(json_prop, "default", has_default ? JS_VECTOR2(-1) : Vector2::Zero);
            }
            else if (type == "Vector3")
            {
                json_prop["type"] = "Vector3";
                setJson_Vector3(json_prop, "default", has_default ? JS_VECTOR3(-1) : Vector3::Zero);
            }
            else if (type == "Vector4")
            {
                json_prop["type"] = "Vector4";
                setJson_Vector4(json_prop, "default", has_default ? JS_VECTOR4(-1) : Vector4::Zero);
            }
            else if (type == "Rect")
            {
                json_prop["type"] = "Rect";
                setJson_Rect(json_prop, "default", has_default ? JS_RECT(-1) : Rect::Zero);
            }
            else if (type == "iRect")
            {
                json_prop["type"] = "iRect";
                setJson_iRect(json_prop, "default", has_default ? JS_iRECT(-1) : iRect{0, 0, 0, 0});
            }
            else if (type == "Color")
            {
                json_prop["type"] = "Color";
                setJson_Color(json_prop, "default", has_default ? JS_COLOR(-1) : Color::White);
            }
            else if (type == "Texture")
            {
                json_prop["type"] = "OTextureRef";
                setJson_OTextureRef(json_prop, "default", has_default ? JS_TEXTURE(-1) : nullptr);
            }
            else if (type == "TiledMap")
            {
                json_prop["type"] = "OTiledMapRef";
                setJson_OTiledMapRef(json_prop, "default", has_default ? getResource<OTiledMap>(ctx, -1) : nullptr);
            }
            else if (type == "Sound")
            {
                json_prop["type"] = "OSoundRef";
                setJson_OSoundRef(json_prop, "default", has_default ? JS_SOUND(-1) : nullptr);
            }
            else if (type == "SpriteAnim")
            {
                json_prop["type"] = "OSpriteAnimRef";
                setJson_OSpriteAnimRef(json_prop, "default", has_default ? getResource<OSpriteAnim>(ctx, -1) : nullptr);
            }
            else if (type == "Model")
            {
                json_prop["type"] = "OModelRef";
                setJson_OModelRef(json_prop, "default", has_default ? getResource<OModel>(ctx, -1) : nullptr);
            }
            else if (type == "Shader")
            {
                json_prop["type"] = "OShaderRef";
                setJson_OShaderRef(json_prop, "default", has_default ? getResource<OShader>(ctx, -1) : nullptr);
            }

            if (has_default) duk_pop(ctx); // default

            duk_pop_2(ctx); // prop, key

            ret->props[prop_name] = json_prop;
        }
        duk_pop_2(ctx); // prop, key
    }

    return ret;
}

static void dukCall(const char* name, void** ptr, int arg_count)
{
    if (duk_pcall(onut::js::pContext, arg_count) != 0)
    {
        logJSStack(onut::js::pContext, std::string(name) + ", call failed: ");
        ptr = nullptr;
    }
    duk_pop(onut::js::pContext);
}

void Script::call_onCreate(Entity* entity, Component* component)
{
    if (onCreatePtr)
    {
        duk_push_heapptr(onut::js::pContext, onCreatePtr);
        duk_push_heapptr(onut::js::pContext, entity->js_object);
        duk_push_heapptr(onut::js::pContext, component->js_object);
        dukCall("onCreate", &onCreatePtr, 2);
    }
}

void Script::call_onEnable(Entity* entity, Component* component)
{
    if (onEnablePtr)
    {
        duk_push_heapptr(onut::js::pContext, onEnablePtr);
        duk_push_heapptr(onut::js::pContext, entity->js_object);
        duk_push_heapptr(onut::js::pContext, component->js_object);
        dukCall("onEnable", &onEnablePtr, 2);
    }
}

void Script::call_onUpdate(Entity* entity, Component* component, float dt)
{
    if (onUpdatePtr)
    {
        duk_push_heapptr(onut::js::pContext, onUpdatePtr);
        duk_push_heapptr(onut::js::pContext, entity->js_object);
        duk_push_heapptr(onut::js::pContext, component->js_object);
        duk_push_number(onut::js::pContext, (duk_double_t)dt);
        dukCall("onUpdate", &onUpdatePtr, 3);
    }
}

void Script::call_onDisable(Entity* entity, Component* component)
{
    if (onDisablePtr)
    {
        duk_push_heapptr(onut::js::pContext, onDisablePtr);
        duk_push_heapptr(onut::js::pContext, entity->js_object);
        duk_push_heapptr(onut::js::pContext, component->js_object);
        dukCall("onDisable", &onDisablePtr, 2);
    }
}

void Script::call_onDestroy(Entity* entity, Component* component)
{
    if (onDestroyPtr)
    {
        duk_push_heapptr(onut::js::pContext, onDestroyPtr);
        duk_push_heapptr(onut::js::pContext, entity->js_object);
        duk_push_heapptr(onut::js::pContext, component->js_object);
        dukCall("onDestroy", &onDestroyPtr, 2);
    }
}
