#pragma once

#include "Script.h"
#include "Component.h"

class ScriptComponent final : public Component, public std::enable_shared_from_this<ScriptComponent>
{
public:
    ScriptRef script;

#if !BARK_EDITOR
    void addScriptPropertiesToJSObject();
#endif

    void onCreate() override;
    void onEnable() override;
#if !BARK_EDITOR
    void onUpdate(float dt) override;
#endif
    void onDisable() override;
    void onDestroy() override;

public:
#if BARK_EDITOR
    Json::Value serialize() override
    {
        auto json = Component::serialize();
        json["properties"] = script->props;
        return std::move(json);
    }
#endif

    void deserialize(const Json::Value& json) override
    {
        Component::deserialize(json);
        script->props = json["properties"];
    }

#if !BARK_EDITOR
    void* getJSPrototype() override { return nullptr; }; // Unused
#endif
};
