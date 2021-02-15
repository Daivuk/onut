#pragma once

#include <onut/ForwardDeclaration.h>
#include "_2DRendererComponent.h"
#if BARK_EDITOR
#include "Gizmo2DRenderer.h"
#endif

OForwardDeclare(TiledMap);
ForwardDeclare(TiledMapRendererComponent);

class TiledMapRendererComponent final 
    : public _2DRendererComponent
#if BARK_EDITOR
    , public Gizmo2DRenderer
#endif
{
public:
    COMPONENT_PROPERTY(OTiledMapRef, tiledMap, nullptr);
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
    Rect getWorldRect() override;

    void render(onut::RenderStates& rs, OSpriteBatch* sb) override;

    COMPONENT_DECLARATION(TiledMapRendererComponent, TiledMapRenderer)
    // [GENERATED COMPONENT DECLARATION BEGIN]
public:
#if BARK_EDITOR
    Json::Value serialize() override
    {
        auto json = Component::serialize();

        setJson_OTiledMapRef(json, "tiledMap", tiledMap);
        setJson_bool(json, "smooth", smooth);
        setJson_int(json, "blendMode", blendMode);

        return std::move(json);
    }
#endif

    void deserialize(const Json::Value& json) override
    {
        Component::deserialize(json);

        tiledMap = getJson_OTiledMapRef(json, "tiledMap", nullptr);
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
        ctx->OTiledMapRef_prop(&tiledMap, "Tiled map");
        ctx->bool_prop(&smooth, "Smooth");
        ctx->int_prop(&blendMode, "Blend mode");
    }
#endif
    // [GENERATED COMPONENT DECLARATION END]
};
