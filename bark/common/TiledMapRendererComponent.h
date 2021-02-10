#pragma once

#include <onut/ForwardDeclaration.h>
#include "_2DRendererComponent.h"

OForwardDeclare(TiledMap);
ForwardDeclare(TiledMapRendererComponent);

class TiledMapRendererComponent final : public _2DRendererComponent
{
public:
    COMPONENT_PROPERTY(OTiledMapRef, tiledMap, nullptr);
    COMPONENT_PROPERTY(bool, smooth, true);
    COMPONENT_PROPERTY(int, blendMode, (int)OBlendPreMultiplied);

    void onCreate() override {};
#if !BARK_EDITOR
    void onUpdate(float dt) override {};
#endif
    void onDestroy() override {};

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
