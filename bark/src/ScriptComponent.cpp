#include "ScriptComponent.h"
#include "Script.h"

void ScriptComponent::onCreate()
{
    if (script) script->call_onCreate(entity.lock(), OThis);
}

void ScriptComponent::onEnable()
{
    if (script) script->call_onEnable(entity.lock(), OThis);
}

void ScriptComponent::onUpdate(float dt)
{
    if (script) script->call_onUpdate(entity.lock(), OThis, dt);
}

void ScriptComponent::onDisable()
{
    if (script) script->call_onDisable(entity.lock(), OThis);
}

void ScriptComponent::onDestroy()
{
    if (script) script->call_onDestroy(entity.lock(), OThis);
}
