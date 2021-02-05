#pragma once

#include <onut/BlendMode.h>
#include <onut/ForwardDeclaration.h>
#include "_2DRendererComponent.h"

OForwardDeclare(Texture);
ForwardDeclare(SpriteRendererComponent);

class SpriteRendererComponent final : public _2DRendererComponent
{
public:
    COMPONENT_PROPERTY(OTextureRef, texture, nullptr);
    COMPONENT_PROPERTY(Color, color, Color::White);
    COMPONENT_PROPERTY(Vector2, origin, OCenter);
    COMPONENT_PROPERTY(bool, smooth, true);
    COMPONENT_PROPERTY(int, blendMode, (int)OBlendPreMultiplied);

    void onCreate() override {};
    void onUpdate(float dt) override {};
    void onDestroy() override {};
    
    void render(onut::RenderStates& rs, OSpriteBatch* sb) override;
    
    COMPONENT_DECLARATION(SpriteRendererComponent, SpriteRenderer)
    // [GENERATED COMPONENT DECLARATION BEGIN]
public:
#if BARK_EDITOR
    Json::Value serialize() override
    {
        auto json = Component::serialize();

        setJson_OTextureRef(json, "texture", texture);
        setJson_Color(json, "color", color);
        setJson_Vector2(json, "origin", origin);
        setJson_bool(json, "smooth", smooth);
        setJson_int(json, "blendMode", blendMode);

        return std::move(json);
    }
#endif

    void deserialize(const Json::Value& json) override
    {
        Component::deserialize(json);

        texture = getJson_OTextureRef(json, "texture", nullptr);
        color = getJson_Color(json, "color", Color::White);
        origin = getJson_Vector2(json, "origin", OCenter);
        smooth = getJson_bool(json, "smooth", true);
        blendMode = getJson_int(json, "blendMode", (int)OBlendPreMultiplied);
    }

    static void* js_prototype;
    void* getJSPrototype() override { return js_prototype; };
    // [GENERATED COMPONENT DECLARATION END]
};
