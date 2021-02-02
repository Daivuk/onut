// DO NOT EDIT - THIS FILE IS GENERATED

#include "ComponentBindings.h"
#include "Component.h"
#include "JSTypeHelper.h"

namespace onut
{
    namespace js
    {
        extern duk_context* pContext;
    }
}

using namespace onut::js;

// Camera2DComponent
#include "Camera2DComponent.h"
    
void* Camera2DComponent::js_prototype = nullptr;

void createCamera2DComponentBindings()
{
    auto ctx = pContext;

    // Constructor
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        return DUK_RET_TYPE_ERROR; // No constructor allowed
    }, 1);
    duk_push_object(ctx);

    // Destructor
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_pointer(ctx, nullptr);
        duk_put_prop_string(ctx, 0, "\xff""\xff""data");
        return 0;
    }, 1);
    duk_set_finalizer(ctx, -2);

    // float zoom
    duk_push_string(ctx, "zoom");
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (Camera2DComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pushJs_float(ctx, pComponent->zoom);
            return 1;
        }
        return 0;
    }, 0);
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (Camera2DComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pComponent->zoom = getJs_float(ctx, 1.0f);
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, 
                    DUK_DEFPROP_HAVE_GETTER |
                    DUK_DEFPROP_HAVE_SETTER);

    // bool clearScreen
    duk_push_string(ctx, "clearScreen");
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (Camera2DComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pushJs_bool(ctx, pComponent->clearScreen);
            return 1;
        }
        return 0;
    }, 0);
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (Camera2DComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pComponent->clearScreen = getJs_bool(ctx, true);
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, 
                    DUK_DEFPROP_HAVE_GETTER |
                    DUK_DEFPROP_HAVE_SETTER);

    // Color clearColor
    duk_push_string(ctx, "clearColor");
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (Camera2DComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pushJs_Color(ctx, pComponent->clearColor);
            return 1;
        }
        return 0;
    }, 0);
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (Camera2DComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pComponent->clearColor = getJs_Color(ctx, Color::Black);
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, 
                    DUK_DEFPROP_HAVE_GETTER |
                    DUK_DEFPROP_HAVE_SETTER);

    Camera2DComponent::js_prototype = duk_get_heapptr(ctx, -1);
    duk_put_prop_string(ctx, -2, "prototype");

    duk_put_global_string(ctx, "Camera2D");
}

// Main function
void createComponentBindings()
{
    createCamera2DComponentBindings();
}
