#include "Component.h"
#include "Entity.h"
#include <JSBindings_Macros.h>

using namespace onut::js;

void createComponentGetterBindings(duk_context* ctx)
{
#define IMPL_COMPONENT_GETTER(__prop_name__, __comp_name__) \
    duk_push_string(ctx, #__prop_name__); \
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t \
    { \
        duk_push_this(ctx); \
        duk_get_prop_string(ctx, -1, "\xff""\xff""data"); \
        auto pEntity = (Entity*)duk_to_pointer(ctx, -1); \
        if (pEntity) \
        { \
            auto it = pEntity->components.find(#__comp_name__); \
            if (it == pEntity->components.end()) \
            { \
                OLogE("Entity has no component named: " #__comp_name__); \
                return 0; \
            } \
            auto pComponent = it->second.get(); \
            duk_push_heapptr(ctx, pComponent->js_object); \
            return 1; \
        } \
        return 0; \
    }, 0); \
    duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);

    IMPL_COMPONENT_GETTER(camera2D, Camera2D);
    IMPL_COMPONENT_GETTER(spriteAnimRenderer, SpriteAnimRenderer);
    IMPL_COMPONENT_GETTER(spriteRenderer, SpriteRenderer);
    IMPL_COMPONENT_GETTER(tiledMapRenderer, TiledMapRenderer);
}
