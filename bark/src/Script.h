#pragma once

#include <onut/ForwardDeclaration.h>
#include <onut/Resource.h>
#include <string>

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

    void call_onCreate(const EntityRef& entity, const ComponentRef& component);
    void call_onEnable(const EntityRef& entity, const ComponentRef& component);
    void call_onUpdate(const EntityRef& entity, const ComponentRef& component, float dt);
    void call_onDisable(const EntityRef& entity, const ComponentRef& component);
    void call_onDestroy(const EntityRef& entity, const ComponentRef& component);

    void* onCreatePtr = nullptr;
    void* onEnablePtr = nullptr;
    void* onUpdatePtr = nullptr;
    void* onDisablePtr = nullptr;
    void* onDestroyPtr = nullptr;
};
