#pragma once

#include <onut/ForwardDeclaration.h>
#include <onut/Color.h>
#include "Component.h"

ForwardDeclare(Camera2DComponent);

class Camera2DComponent final : public Component
{
public:
    COMPONENT_PROPERTY(float, zoom, 1.0f);
    COMPONENT_PROPERTY(bool, clearScreen, true);
    COMPONENT_PROPERTY(Color, clearColor, OColorHex(262c3b));
    COMPONENT_PROPERTY(Vector2, origin, OCenter);

    void onCreate() override {};
    void onEnable() override;
#if !BARK_EDITOR
    void onUpdate(float dt) override {};
#endif
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
        setJson_Vector2(json, "origin", origin);

        return std::move(json);
    }
#endif

    void deserialize(const Json::Value& json) override
    {
        Component::deserialize(json);

        zoom = getJson_float(json, "zoom", 1.0f);
        clearScreen = getJson_bool(json, "clearScreen", true);
        clearColor = getJson_Color(json, "clearColor", Color::Black);
        origin = getJson_Vector2(json, "origin", OCenter);
    }

#if !BARK_EDITOR
    static void* js_prototype;
    void* getJSPrototype() override { return js_prototype; };
#endif
    // [GENERATED COMPONENT DECLARATION END]
};
