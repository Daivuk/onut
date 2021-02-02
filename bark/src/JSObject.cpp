#include "JSObject.h"

#include <JSBindings_Macros.h>
namespace onut
{
    namespace js
    {
        extern duk_context* pContext; // Haxor. This context already contains all standard onut bindings

        void newUI(duk_context* ctx, const OUIControlRef& pUIControl);
    }
}

using namespace onut::js;

void* JSObject::js_objects_heap_ptr = nullptr;
uint64_t JSObject::next_script_unique_id = 0;

void JSObject::initJSObjects()
{
    auto ctx = pContext;

    // Create empty js object
    duk_push_object(ctx);
    duk_put_global_string(ctx, "___js_objects");

    // Get it's heap ptr
    duk_get_global_string(ctx, "___js_objects");
    js_objects_heap_ptr = duk_get_heapptr(ctx, -1);
    duk_pop(ctx);
}

void JSObject::initJSObject(void* prototype)
{
    js_global_name = "obj_" + std::to_string(next_script_unique_id++);
    auto ctx = pContext;

    duk_push_heapptr(ctx, js_objects_heap_ptr);

    // Create empty js object
    duk_push_object(ctx);
    duk_push_pointer(ctx, this);
    duk_put_prop_string(ctx, -2, "\xff""\xff""data");

    if (prototype)
    {
        duk_push_heapptr(ctx, prototype);
        duk_set_prototype(ctx, -2);
    }

    duk_put_prop_string(ctx, -2, js_global_name.c_str());

    // Get it's heap ptr
    duk_get_prop_string(ctx, -1, js_global_name.c_str());
    js_object = duk_get_heapptr(ctx, -1);

    duk_pop(ctx);
    duk_pop(ctx);
}

JSObject::~JSObject()
{
    auto ctx = pContext;
    
    duk_push_heapptr(ctx, js_objects_heap_ptr);
    duk_del_prop_string(ctx, -1, js_global_name.c_str());
    duk_pop(ctx);
}
