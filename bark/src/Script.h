#pragma once

#include <onut/ForwardDeclaration.h>
#include <onut/Resource.h>
#include <string>
#include <json/json.h>

OForwardDeclare(ContentManager);
ForwardDeclare(Script);
ForwardDeclare(Entity);
ForwardDeclare(Component);

class Script : public onut::Resource
{
public:
    Script();
    ~Script();

    static ScriptRef createFromFile(const std::string& filename, const OContentManagerRef& pContentManager);

    void call_onCreate(Entity* entity, Component* component);
    void call_onEnable(Entity* entity, Component* component);
    void call_onUpdate(Entity* entity, Component* component, float dt);
    void call_onDisable(Entity* entity, Component* component);
    void call_onDestroy(Entity* entity, Component* component);

    void* onCreatePtr = nullptr;
    void* onEnablePtr = nullptr;
    void* onUpdatePtr = nullptr;
    void* onDisablePtr = nullptr;
    void* onDestroyPtr = nullptr;

    Json::Value props;
};
