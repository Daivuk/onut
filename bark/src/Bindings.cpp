#include "Bindings.h"
#include "Entity.h"
#include "Component.h"
#include "ComponentBindings.h"
#include "SceneManager.h"

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

void* pEntityPrototype = nullptr;

void createComponentGetterBindings(duk_context* ctx);

static void createEntityBindings()
{
    auto ctx = pContext;

    // Entity() 
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        return DUK_RET_TYPE_ERROR; // No constructor allowed
    }, 1);
    duk_push_object(ctx);

    // ~Entity()
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_get_prop_string(ctx, 0, "\xff""\xff""data");
        auto pEntity = (Entity*)duk_to_pointer(ctx, -1);
        if (pEntity)
        {
            duk_pop(ctx);
            duk_push_pointer(ctx, nullptr);
            duk_put_prop_string(ctx, 0, "\xff""\xff""data");
        }
        return 0;
    }, 1);
    duk_set_finalizer(ctx, -2);

    // getName()
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pEntity = (Entity*)duk_to_pointer(ctx, -1);
        if (pEntity)
        {
            duk_push_string(ctx, pEntity->name.c_str());
            return 1;
        }
        return 0;
    }, 0);
    duk_put_prop_string(ctx, -2, "getName");

    // setName(name)
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pEntity = (Entity*)duk_to_pointer(ctx, -1);
        if (pEntity)
        {
            auto nameBefore = pEntity->name;
            pEntity->name = JS_STRING(0, nameBefore.c_str());
        }
        return 0;
    }, 1);
    duk_put_prop_string(ctx, -2, "setName");

    // getPosition()
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pEntity = (Entity*)duk_to_pointer(ctx, -1);
        if (pEntity)
        {
            newVector3(ctx, pEntity->getLocalTransform().Translation());
            return 1;
        }
        return 0;
    }, 0);
    duk_put_prop_string(ctx, -2, "getPosition");

    // getWorldPosition()
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pEntity = (Entity*)duk_to_pointer(ctx, -1);
        if (pEntity)
        {
            newVector3(ctx, pEntity->getWorldTransform().Translation());
            return 1;
        }
        return 0;
    }, 0);
    duk_put_prop_string(ctx, -2, "getWorldPosition");

    // setPosition(position)
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pEntity = (Entity*)duk_to_pointer(ctx, -1);
        if (pEntity)
        {
            auto before = pEntity->getLocalTransform();
            before.Translation(JS_VECTOR3(0, before.Translation()));
            pEntity->setLocalTransform(before);
        }
        return 0;
    }, 1);
    duk_put_prop_string(ctx, -2, "setPosition");
    

    // getTransform()
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pEntity = (Entity*)duk_to_pointer(ctx, -1);
        if (pEntity)
        {
            newMatrix(ctx, pEntity->getLocalTransform());
            return 1;
        }
        return 0;
    }, 0);
    duk_put_prop_string(ctx, -2, "getTransform");

    // getWorldTransform()
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pEntity = (Entity*)duk_to_pointer(ctx, -1);
        if (pEntity)
        {
            newMatrix(ctx, pEntity->getWorldTransform());
            return 1;
        }
        return 0;
    }, 0);
    duk_put_prop_string(ctx, -2, "getWorldTransform");

    // setTransform(transform)
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pEntity = (Entity*)duk_to_pointer(ctx, -1);
        if (pEntity)
        {
            const auto& before = pEntity->getLocalTransform();
            pEntity->setLocalTransform(JS_MATRIX(0, before));
        }
        return 0;
    }, 1);
    duk_put_prop_string(ctx, -2, "setTransform");

    // getComponent(name)
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\xff""\xff""data");
        auto pEntity = (Entity*)duk_to_pointer(ctx, -1);
        if (pEntity)
        {
            auto component_name = JS_STRING(0);

            auto it = pEntity->components.find(component_name);
            if (it == pEntity->components.end())
            {
                OLogE(std::string("Entity has no component named: ") + component_name);
                return 0;
            }
            auto pComponent = it->second.get();

            duk_push_heapptr(ctx, pComponent->js_object);

            return 1;
        }
        return 0;
    }, 1);
    duk_put_prop_string(ctx, -2, "getComponent");
    
    // common components getters
    createComponentGetterBindings(ctx);

    // add(child)
    //duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    //{
    //    duk_push_this(ctx);
    //    duk_get_prop_string(ctx, -1, "\xff""\xff""data");
    //    auto ppEntity = (EntityRef*)duk_to_pointer(ctx, -1);
    //    if (ppEntity)
    //    {
    //        auto pEntity = ppEntity->get();
    //        pEntity->add(JS_UI(0));
    //    }
    //    return 0;
    //}, 1);
    //duk_put_prop_string(ctx, -2, "add");

    //// insert(child, before)
    //duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    //{
    //    duk_push_this(ctx);
    //    duk_get_prop_string(ctx, -1, "\xff""\xff""data");
    //    auto ppEntity = (EntityRef*)duk_to_pointer(ctx, -1);
    //    if (ppEntity)
    //    {
    //        auto pEntity = ppEntity->get();
    //        pEntity->insert(JS_UI(0), JS_UI(1));
    //    }
    //    return 0;
    //}, 2);
    //duk_put_prop_string(ctx, -2, "insert");

    //// remove(child)
    //duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    //{
    //    duk_push_this(ctx);
    //    duk_get_prop_string(ctx, -1, "\xff""\xff""data");
    //    auto ppEntity = (EntityRef*)duk_to_pointer(ctx, -1);
    //    if (ppEntity)
    //    {
    //        auto pEntity = ppEntity->get();
    //        pEntity->remove(JS_UI(0));
    //    }
    //    return 0;
    //}, 1);
    //duk_put_prop_string(ctx, -2, "remove");

    //// removeAll(child)
    //duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    //{
    //    duk_push_this(ctx);
    //    duk_get_prop_string(ctx, -1, "\xff""\xff""data");
    //    auto ppEntity = (EntityRef*)duk_to_pointer(ctx, -1);
    //    if (ppEntity)
    //    {
    //        auto pEntity = ppEntity->get();
    //        pEntity->removeAll();
    //    }
    //    return 0;
    //}, 0);
    //duk_put_prop_string(ctx, -2, "removeAll");

    //// getChildCount()
    //duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    //{
    //    duk_push_this(ctx);
    //    duk_get_prop_string(ctx, -1, "\xff""\xff""data");
    //    auto ppEntity = (EntityRef*)duk_to_pointer(ctx, -1);
    //    if (ppEntity)
    //    {
    //        auto pEntity = ppEntity->get();
    //        duk_push_uint(ctx, (duk_uint_t)pEntity->getChildren().size());
    //        return 1;
    //    }
    //    return 0;
    //}, 0);
    //duk_put_prop_string(ctx, -2, "getChildCount");

    //// getChild()
    //duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    //{
    //    duk_push_this(ctx);
    //    duk_get_prop_string(ctx, -1, "\xff""\xff""data");
    //    auto ppEntity = (EntityRef*)duk_to_pointer(ctx, -1);
    //    if (ppEntity)
    //    {
    //        auto pEntity = ppEntity->get();
    //        if (duk_is_string(ctx, 0))
    //        {
    //            auto pChild = pEntity->getChild(JS_STRING(0), JS_BOOL(1, true));
    //            if (pChild)
    //            {
    //                newUI(ctx, pChild);
    //                return 1;
    //            }
    //        }
    //        else if (duk_is_number(ctx, 0))
    //        {
    //            auto index = JS_UINT(0);
    //            if (index < pEntity->getChildren().size())
    //            {
    //                newUI(ctx, pEntity->getChildren()[index]);
    //                return 1;
    //            }
    //        }
    //    }
    //    return 0;
    //}, 2);
    //duk_put_prop_string(ctx, -2, "getChild");

    //// getParent()
    //duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    //{
    //    duk_push_this(ctx);
    //    duk_get_prop_string(ctx, -1, "\xff""\xff""data");
    //    auto ppEntity = (EntityRef*)duk_to_pointer(ctx, -1);
    //    if (ppEntity)
    //    {
    //        auto pEntity = ppEntity->get();
    //        if (pEntity->getParent())
    //        {
    //            newUI(ctx, pEntity->getParent());
    //            return 1;
    //        }
    //    }
    //    return 0;
    //}, 0);
    //duk_put_prop_string(ctx, -2, "getParent");

    //// copy()
    //duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    //{
    //    duk_push_this(ctx);
    //    duk_get_prop_string(ctx, -1, "\xff""\xff""data");
    //    auto ppEntity = (EntityRef*)duk_to_pointer(ctx, -1);
    //    if (ppEntity)
    //    {
    //        auto pEntity = ppEntity->get();
    //        newUI(ctx, pEntity->copy());
    //        return 1;
    //    }
    //    return 0;
    //}, 0);
    //duk_put_prop_string(ctx, -2, "copy");

    // Done with the object
    pEntityPrototype = duk_get_heapptr(ctx, -1);
    duk_put_prop_string(ctx, -2, "prototype");

    duk_put_global_string(ctx, "Entity");
}

EntityRef findEntityRecur(const EntityRef& entity, const std::string& name)
{
    if (entity->name == name) return entity;

    for (const auto& child : entity->children)
    {
        auto ret = findEntityRecur(child, name);
        if (ret) return ret;
    }

    return nullptr;
}

void createSceneBindings()
{
    auto ctx = pContext;

    JS_GLOBAL_FUNCTION_BEGIN
    {
        auto name = JS_STRING(0);
        extern std::string scene_to_load;
        scene_to_load = name;
        return 0;
    }
    JS_GLOBAL_FUNCTION_END("loadScene", 1);

    JS_GLOBAL_FUNCTION_BEGIN
    {
        auto name = JS_STRING(0);
        auto ret = findEntityRecur(root, name);
        if (ret)
        {
            duk_push_heapptr(ctx, ret->js_object);
            return 1;
        }
        return 0;
    }
    JS_GLOBAL_FUNCTION_END("findEntity", 1);
}

void createBindings()
{
    createEntityBindings();
    createSceneBindings();
    createComponentBindings();
}
