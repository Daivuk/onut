// DO NOT EDIT - THIS FILE IS GENERATED

#include <JSBindings_Macros.h>
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
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

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
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

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
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

    // Vector2 origin
    duk_push_string(ctx, "origin");
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (Camera2DComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pushJs_Vector2(ctx, pComponent->origin);
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
            pComponent->origin = getJs_Vector2(ctx, OCenter);
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

    Camera2DComponent::js_prototype = duk_get_heapptr(ctx, -1);
    duk_put_prop_string(ctx, -2, "prototype");

    duk_put_global_string(ctx, "Camera2D");
}

// SpriteAnimRendererComponent
#include "SpriteAnimRendererComponent.h"
    
void* SpriteAnimRendererComponent::js_prototype = nullptr;

void createSpriteAnimRendererComponentBindings()
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

    // OSpriteAnimRef spriteAnim
    duk_push_string(ctx, "spriteAnim");
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteAnimRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pushJs_OSpriteAnimRef(ctx, pComponent->spriteAnim);
            return 1;
        }
        return 0;
    }, 0);
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteAnimRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pComponent->set_spriteAnim(getJs_OSpriteAnimRef(ctx, nullptr));
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

    // std_string anim
    duk_push_string(ctx, "anim");
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteAnimRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pushJs_std_string(ctx, pComponent->anim);
            return 1;
        }
        return 0;
    }, 0);
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteAnimRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pComponent->set_anim(getJs_std_string(ctx, ""));
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

    // Color color
    duk_push_string(ctx, "color");
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteAnimRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pushJs_Color(ctx, pComponent->color);
            return 1;
        }
        return 0;
    }, 0);
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteAnimRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pComponent->color = getJs_Color(ctx, Color::White);
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

    // bool smooth
    duk_push_string(ctx, "smooth");
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteAnimRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pushJs_bool(ctx, pComponent->smooth);
            return 1;
        }
        return 0;
    }, 0);
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteAnimRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pComponent->smooth = getJs_bool(ctx, true);
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

    // int blendMode
    duk_push_string(ctx, "blendMode");
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteAnimRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pushJs_int(ctx, pComponent->blendMode);
            return 1;
        }
        return 0;
    }, 0);
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteAnimRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pComponent->blendMode = getJs_int(ctx, (int)OBlendPreMultiplied);
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

    SpriteAnimRendererComponent::js_prototype = duk_get_heapptr(ctx, -1);
    duk_put_prop_string(ctx, -2, "prototype");

    duk_put_global_string(ctx, "SpriteAnimRenderer");
}

// SpriteRendererComponent
#include "SpriteRendererComponent.h"
    
void* SpriteRendererComponent::js_prototype = nullptr;

void createSpriteRendererComponentBindings()
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

    // OTextureRef texture
    duk_push_string(ctx, "texture");
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pushJs_OTextureRef(ctx, pComponent->texture);
            return 1;
        }
        return 0;
    }, 0);
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pComponent->texture = getJs_OTextureRef(ctx, nullptr);
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

    // Color color
    duk_push_string(ctx, "color");
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pushJs_Color(ctx, pComponent->color);
            return 1;
        }
        return 0;
    }, 0);
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pComponent->color = getJs_Color(ctx, Color::White);
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

    // Vector2 origin
    duk_push_string(ctx, "origin");
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pushJs_Vector2(ctx, pComponent->origin);
            return 1;
        }
        return 0;
    }, 0);
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pComponent->origin = getJs_Vector2(ctx, OCenter);
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

    // bool smooth
    duk_push_string(ctx, "smooth");
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pushJs_bool(ctx, pComponent->smooth);
            return 1;
        }
        return 0;
    }, 0);
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pComponent->smooth = getJs_bool(ctx, true);
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

    // int blendMode
    duk_push_string(ctx, "blendMode");
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pushJs_int(ctx, pComponent->blendMode);
            return 1;
        }
        return 0;
    }, 0);
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (SpriteRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pComponent->blendMode = getJs_int(ctx, (int)OBlendPreMultiplied);
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

    SpriteRendererComponent::js_prototype = duk_get_heapptr(ctx, -1);
    duk_put_prop_string(ctx, -2, "prototype");

    duk_put_global_string(ctx, "SpriteRenderer");
}

// TiledMapRendererComponent
#include "TiledMapRendererComponent.h"
    
void* TiledMapRendererComponent::js_prototype = nullptr;

void createTiledMapRendererComponentBindings()
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

    // OTiledMapRef tiledMap
    duk_push_string(ctx, "tiledMap");
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (TiledMapRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pushJs_OTiledMapRef(ctx, pComponent->tiledMap);
            return 1;
        }
        return 0;
    }, 0);
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (TiledMapRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pComponent->tiledMap = getJs_OTiledMapRef(ctx, nullptr);
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

    // bool smooth
    duk_push_string(ctx, "smooth");
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (TiledMapRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pushJs_bool(ctx, pComponent->smooth);
            return 1;
        }
        return 0;
    }, 0);
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (TiledMapRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pComponent->smooth = getJs_bool(ctx, true);
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

    // int blendMode
    duk_push_string(ctx, "blendMode");
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (TiledMapRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pushJs_int(ctx, pComponent->blendMode);
            return 1;
        }
        return 0;
    }, 0);
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pComponent = (TiledMapRendererComponent*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pComponent->blendMode = getJs_int(ctx, (int)OBlendPreMultiplied);
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

    TiledMapRendererComponent::js_prototype = duk_get_heapptr(ctx, -1);
    duk_put_prop_string(ctx, -2, "prototype");

    duk_put_global_string(ctx, "TiledMapRenderer");
}

// Main function
void createComponentBindings()
{
    createCamera2DComponentBindings();
    createSpriteAnimRendererComponentBindings();
    createSpriteRendererComponentBindings();
    createTiledMapRendererComponentBindings();
}
