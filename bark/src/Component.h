#pragma once

#include <onut/ForwardDeclaration.h>
#include <vector>
#include "JsonHelper.h"
#include "JSObject.h"

#define COMPONENT_DECLARATION(__class__, __name__)
#define COMPONENT_PROPERTY(__type__, __name__, __default__, ...) __type__ __name__ = __default__;

// Flags
#define PROP_READ_ONLY
#define PROP_CPP_GETTER
#define PROP_CPP_SETTER

ForwardDeclare(Entity);
ForwardDeclare(Component);

class Component : public JSObject
{
public:
    bool enabled = true;

    Entity* entity = nullptr;

    virtual ~Component() {}

#if BARK_EDITOR
    virtual Json::Value serialize()
    {
        Json::Value json;

        setJsonBool(json, "enabled", enabled);

        return std::move(json);
    }
#endif

    virtual void deserialize(const Json::Value& json)
    {
        enabled = getJson_bool(json, "enabled", true);
    }

    virtual void onCreate() = 0;
    virtual void onEnable();
    virtual void onUpdate(float dt) = 0;
    virtual void onDisable();
    virtual void onDestroy() = 0;

    virtual void* getJSPrototype() = 0;
};
