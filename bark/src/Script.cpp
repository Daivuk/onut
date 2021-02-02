#include "Script.h"
#include <onut/ContentManager.h>
#include <onut/Files.h>
#include <onut/Strings.h>
#include <regex>
#include "Entity.h"
#include "Component.h"

#include <JSBindings_Macros.h>
namespace onut
{
    namespace js
    {
        extern duk_context* pContext; // Haxor. This context already contains all standard onut bindings
    }
}

Script::Script() {}

Script::~Script() {}

ScriptRef Script::createFromFile(const std::string& filename, const OContentManagerRef& pContentManager)
{
    auto ret = OMake<Script>();

    // Lets do some regex! Replace _init, _update, with filename without extension
    auto name = onut::getFilenameWithoutExtension(filename);
    auto source = onut::getFileString(filename);

#define REPLACE_FUNCTION_NAME(__name__) \
    source = std::regex_replace(source, \
                                std::regex("function\\s+_" #__name__ "\\("), \
                                "function " + name + "_" #__name__ "(");

    REPLACE_FUNCTION_NAME(onCreate)
    REPLACE_FUNCTION_NAME(onEnable)
    REPLACE_FUNCTION_NAME(onUpdate)
    REPLACE_FUNCTION_NAME(onDisable)
    REPLACE_FUNCTION_NAME(onDestroy)

    auto filename_without_path = onut::getFilename(filename);
    duk_push_string(onut::js::pContext, filename_without_path.c_str());
    if (duk_pcompile_lstring_filename(onut::js::pContext, 0, source.c_str(), source.size()) != 0)
    {
        logJSStack(onut::js::pContext, onut::getFilename(filename) + " failed");
        duk_pop(onut::js::pContext);
        return ret;
    }
                
    if (duk_pcall(onut::js::pContext, 0) != 0)
    {
        logJSStack(onut::js::pContext, onut::getFilename(filename) + " failed");
        duk_pop(onut::js::pContext);
        return ret;
    }

    duk_pop(onut::js::pContext);

#define GRAB_FUNCTION_PTR(__name__) \
    if (duk_get_global_string(onut::js::pContext, (name + "_" #__name__).c_str())) \
    { \
        ret->__name__ ## Ptr = duk_get_heapptr(onut::js::pContext, -1); \
        duk_pop(onut::js::pContext); \
    }

    GRAB_FUNCTION_PTR(onCreate)
    GRAB_FUNCTION_PTR(onEnable)
    GRAB_FUNCTION_PTR(onUpdate)
    GRAB_FUNCTION_PTR(onDisable)
    GRAB_FUNCTION_PTR(onDestroy)

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

void Script::call_onCreate(const EntityRef& entity, const ComponentRef& component)
{
    if (onCreatePtr)
    {
        duk_push_heapptr(onut::js::pContext, onCreatePtr);
        duk_push_heapptr(onut::js::pContext, entity->js_object);
        duk_push_heapptr(onut::js::pContext, component->js_object);
        dukCall("onCreate", &onCreatePtr, 2);
    }
}

void Script::call_onEnable(const EntityRef& entity, const ComponentRef& component)
{
    if (onEnablePtr)
    {
        duk_push_heapptr(onut::js::pContext, onEnablePtr);
        duk_push_heapptr(onut::js::pContext, entity->js_object);
        duk_push_heapptr(onut::js::pContext, component->js_object);
        dukCall("onEnable", &onEnablePtr, 2);
    }
}

void Script::call_onUpdate(const EntityRef& entity, const ComponentRef& component, float dt)
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

void Script::call_onDisable(const EntityRef& entity, const ComponentRef& component)
{
    if (onDisablePtr)
    {
        duk_push_heapptr(onut::js::pContext, onDisablePtr);
        duk_push_heapptr(onut::js::pContext, entity->js_object);
        duk_push_heapptr(onut::js::pContext, component->js_object);
        dukCall("onDisable", &onDisablePtr, 2);
    }
}

void Script::call_onDestroy(const EntityRef& entity, const ComponentRef& component)
{
    if (onDestroyPtr)
    {
        duk_push_heapptr(onut::js::pContext, onDestroyPtr);
        duk_push_heapptr(onut::js::pContext, entity->js_object);
        duk_push_heapptr(onut::js::pContext, component->js_object);
        dukCall("onDestroy", &onDestroyPtr, 2);
    }
}
