#pragma once

#include <onut/BlendMode.h>
#include <onut/ForwardDeclaration.h>
#include "_2DRendererComponent.h"
#if BARK_EDITOR
#include "Gizmo2DRenderer.h"
#endif

OForwardDeclare(Texture);
ForwardDeclare(SpriteRendererComponent);

class SpriteRendererComponent final 
    : public _2DRendererComponent
#if BARK_EDITOR
    , public Gizmo2DRenderer
#endif
{
public:
    COMPONENT_PROPERTY(OTextureRef, texture, nullptr);
    COMPONENT_PROPERTY(Color, color, Color::White);
    COMPONENT_PROPERTY(Vector2, origin, OCenter);
    COMPONENT_PROPERTY(bool, smooth, true);
    COMPONENT_PROPERTY(int, blendMode, (int)OBlendPreMultiplied);

    void onCreate() override {};
    void onDestroy() override {};
#if !BARK_EDITOR
    void onUpdate(float dt) override {};
#else
    void onEnable() override;
    void onDisable() override;
    void renderGizmo(Gizmo2DContext* ctx) override;
#endif
    
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

#if !BARK_EDITOR
    static void* js_prototype;
    void* getJSPrototype() override { return js_prototype; };
#endif

#if BARK_EDITOR
    void drawProperties(GUIContext* ctx) override
    {
        ctx->OTextureRef_prop(&texture, "Texture");
        ctx->Color_prop(&color, "Color");
        ctx->Vector2_prop(&origin, "Origin");
        ctx->bool_prop(&smooth, "Smooth");
        ctx->int_prop(&blendMode, "Blend mode");
    }
#endif
    // [GENERATED COMPONENT DECLARATION END]
};
