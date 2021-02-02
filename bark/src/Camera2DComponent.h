#pragma once

#include <onut/ForwardDeclaration.h>
#include <onut/Color.h>
#include "Component.h"

ForwardDeclare(Camera2DComponent);

class Camera2DComponent final : public Component, public std::enable_shared_from_this<Camera2DComponent>
{
public:
    COMPONENT_PROPERTY(float, zoom, 1.0f);
    COMPONENT_PROPERTY(bool, clearScreen, true);
    COMPONENT_PROPERTY(Color, clearColor, Color::Black);

    void onCreate() override {};
    void onEnable() override;
    void onUpdate(float dt) override {};
    void onDisable() override;
    void onDestroy() override {};
    
    COMPONENT_DECLARATION(Camera2DComponent, Camera2D)
    // [GENERATED COMPONENT DECLARATION BEGIN]
public:
#if BARK_EDITOR
    Json::Value serialize() override
    {
        auto json = Component::serialize();

        setJson_float(json, "zoom", zoom);
        setJson_bool(json, "clearScreen", clearScreen);
        setJson_Color(json, "clearColor", clearColor);

        return std::move(json);
    }
#endif

    void deserialize(const Json::Value& json) override
    {
        Component::deserialize(json);

        getJson_float(json, "zoom", 1.0f);
        getJson_bool(json, "clearScreen", true);
        getJson_Color(json, "clearColor", Color::Black);
    }

    static void* js_prototype;
    void* getJSPrototype() override { return js_prototype; };
    // [GENERATED COMPONENT DECLARATION END]
};
