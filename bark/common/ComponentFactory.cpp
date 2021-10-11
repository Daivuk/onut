// DO NOT EDIT - THIS FILE IS GENERATED

#include "ComponentFactory.h"
#include "Component.h"
#include <map>
#include <functional>
#include <onut/Log.h>

#include "Camera2DComponent.h"
#include "SpriteAnimRendererComponent.h"
#include "SpriteRendererComponent.h"
#include "TiledMapRendererComponent.h"

#include "ScriptComponent.h"

static std::map<std::string, std::function<ComponentRef()>> factories;

void initComponentFactory()
{
    factories["Camera2D"] = []()->ComponentRef {return OMake<Camera2DComponent>();};
    factories["SpriteAnimRenderer"] = []()->ComponentRef {return OMake<SpriteAnimRendererComponent>();};
    factories["SpriteRenderer"] = []()->ComponentRef {return OMake<SpriteRendererComponent>();};
    factories["TiledMapRenderer"] = []()->ComponentRef {return OMake<TiledMapRendererComponent>();};

    factories["Script"] = []()->ComponentRef {return OMake<ScriptComponent>();};
}

void shutdownComponentFactory()
{
    factories.clear();
}

ComponentRef createComponentByName(const std::string& name)
{
    auto it = factories.find(name);
    if (it == factories.end())
    {
        OLogE("No Component found: " + name);
        return nullptr;
    }
    auto component = it->second();
#if !BARK_EDITOR
    component->initJSObject(component->getJSPrototype());
#endif
    return component;
}