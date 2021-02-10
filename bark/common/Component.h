#pragma once

#include <onut/ForwardDeclaration.h>
#include <vector>
#include "JsonHelper.h"
#if !BARK_EDITOR
#include "JSObject.h"
#endif
#if BARK_EDITOR
#include "GUIContext.h"
#endif

#define COMPONENT_DECLARATION(__class__, __name__)
#define COMPONENT_PROPERTY(__type__, __name__, __default__, ...) __type__ __name__ = __default__;

// Flags
#define PROP_READ_ONLY
#define PROP_CPP_GETTER
#define PROP_CPP_SETTER

ForwardDeclare(Entity);
ForwardDeclare(Component);

class Component 
#if !BARK_EDITOR
    : public JSObject
#endif
{
public:
    bool            enabled     = true;
    Entity*         entity      = nullptr;
#if BARK_EDITOR
    bool            expanded    = true;
#endif

    virtual ~Component() {}

#if BARK_EDITOR
    virtual Json::Value serialize()
    {
        Json::Value json;

        setJson_bool(json, "enabled", enabled);

        return std::move(json);
    }
#endif

    virtual void deserialize(const Json::Value& json)
    {
        enabled = getJson_bool(json, "enabled", true);
    }

    virtual void onCreate() = 0;
    virtual void onEnable();
#if !BARK_EDITOR
    virtual void onUpdate(float dt) = 0;
#endif
    virtual void onDisable();
    virtual void onDestroy() = 0;

#if !BARK_EDITOR
    virtual void* getJSPrototype() = 0;
#endif

#if BARK_EDITOR
    virtual void drawProperties(GUIContext* ctx) = 0;
#endif
};
