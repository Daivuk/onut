#include "ScriptComponent.h"
#include "Script.h"
#include "JsonHelper.h"
#include "JSTypeHelper.h"

#include <JSBindings_Macros.h>
namespace onut
{
    namespace js
    {
        extern duk_context* pContext;
    }
}

using namespace onut::js;

void ScriptComponent::addScriptPropertiesToJSObject()
{
    if (!script) return;

    auto ctx = pContext;

    const auto& props = script->props;
    auto members = script->props.getMemberNames();

    duk_push_heapptr(ctx, js_object); // c

    for (const auto& key : members)
    {
        const auto& prop = props[key];
        auto type = prop["type"].asString();

#define PUSH_JS_FOR_TYPE(__type__) else if (type == #__type__) pushJs_ ## __type__(ctx, getJson_ ## __type__(prop, "default"));

        if (type == "bool") pushJs_bool(ctx, getJson_bool(prop, "default"));
        PUSH_JS_FOR_TYPE(int)
        PUSH_JS_FOR_TYPE(float)
        PUSH_JS_FOR_TYPE(std_string)
        PUSH_JS_FOR_TYPE(Vector2)
        PUSH_JS_FOR_TYPE(Vector3)
        PUSH_JS_FOR_TYPE(Vector4)
        PUSH_JS_FOR_TYPE(Rect)
        PUSH_JS_FOR_TYPE(iRect)
        PUSH_JS_FOR_TYPE(Color)
        PUSH_JS_FOR_TYPE(OTextureRef)
        PUSH_JS_FOR_TYPE(OTiledMapRef)
        PUSH_JS_FOR_TYPE(OSoundRef)
        PUSH_JS_FOR_TYPE(OSpriteAnimRef)
        PUSH_JS_FOR_TYPE(OModelRef)
        PUSH_JS_FOR_TYPE(OShaderRef)
        else
        {
            OLogE("Invalid property type: " + type + " " + key);
            break;
        }

        duk_put_prop_string(ctx, -2, key.c_str());
    }

    duk_pop(ctx); // c
}

void ScriptComponent::onCreate()
{
    if (script) script->call_onCreate(entity, this);
}

void ScriptComponent::onEnable()
{
    Component::onEnable();

    if (script) script->call_onEnable(entity, this);
}

void ScriptComponent::onUpdate(float dt)
{
    if (script) script->call_onUpdate(entity, this, dt);
}

void ScriptComponent::onDisable()
{
    Component::onDisable();

    if (script) script->call_onDisable(entity, this);
}

void ScriptComponent::onDestroy()
{
    if (script) script->call_onDestroy(entity, this);
}
