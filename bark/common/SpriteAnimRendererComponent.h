#pragma once

#include <onut/BlendMode.h>
#include <onut/ForwardDeclaration.h>
#include "_2DRendererComponent.h"
#if BARK_EDITOR
#include "Gizmo2DRenderer.h"
#endif

OForwardDeclare(SpriteAnim);
OForwardDeclare(SpriteAnimInstance);
ForwardDeclare(SpriteAnimRendererComponent);

class SpriteAnimRendererComponent final 
    : public _2DRendererComponent
#if BARK_EDITOR
    , public Gizmo2DRenderer
#endif
{
public:
    COMPONENT_PROPERTY(OSpriteAnimRef, spriteAnim, nullptr, PROP_CPP_SETTER);
    COMPONENT_PROPERTY(std::string, anim, "", PROP_CPP_SETTER);
    COMPONENT_PROPERTY(Color, color, Color::White);
    COMPONENT_PROPERTY(bool, smooth, true);
    COMPONENT_PROPERTY(int, blendMode, (int)OBlendPreMultiplied);

#if !BARK_EDITOR
    OSpriteAnimInstanceRef spriteAnimInstance;
#endif

    void onCreate() override;
    void onDestroy() override {};
    void render(onut::RenderStates& rs, OSpriteBatch* sb) override;
#if !BARK_EDITOR
    void onUpdate(float dt) override {};
    void set_spriteAnim(const OSpriteAnimRef& value);
    void set_anim(const std::string& value);
#else
    void onEnable() override;
    void onDisable() override;
    void renderGizmo(Gizmo2DContext* ctx) override;
#endif
    Rect getWorldRect() override;
    
    COMPONENT_DECLARATION(SpriteAnimRendererComponent, SpriteAnimRenderer)
    // [GENERATED COMPONENT DECLARATION BEGIN]
public:
#if BARK_EDITOR
    Json::Value serialize() override
    {
        auto json = Component::serialize();

        setJson_OSpriteAnimRef(json, "spriteAnim", spriteAnim);
        setJson_std_string(json, "anim", anim);
        setJson_Color(json, "color", color);
        setJson_bool(json, "smooth", smooth);
        setJson_int(json, "blendMode", blendMode);

        return std::move(json);
    }
#endif

    void deserialize(const Json::Value& json) override
    {
        Component::deserialize(json);

        spriteAnim = getJson_OSpriteAnimRef(json, "spriteAnim", nullptr);
        anim = getJson_std_string(json, "anim", "");
        color = getJson_Color(json, "color", Color::White);
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
        ctx->OSpriteAnimRef_prop(&spriteAnim, "Sprite anim");
        ctx->std_string_prop(&anim, "Anim");
        ctx->Color_prop(&color, "Color");
        ctx->bool_prop(&smooth, "Smooth");
        ctx->int_prop(&blendMode, "Blend mode");
    }
#endif
    // [GENERATED COMPONENT DECLARATION END]
};
