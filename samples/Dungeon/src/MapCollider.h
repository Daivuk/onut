#pragma once
#include <onut/Component.h>
#include <onut/Point.h>
#include <onut/TiledMap.h>

#include <onut/ForwardDeclaration.h>
ForwardDeclare(MapCollider);

class MapCollider final : public OComponent
{
public:
    struct CollisionInfo
    {
        uint32_t tileId;
        Point tilePosition;
        Vector2 direction;
    };

    void onCreate() override;
    void onUpdate() override;

private:
    OTiledMapRef m_pTiledMap;
    Vector2 m_lastPosition;
    OTiledMap::TileLayer* m_pInfoLayer = nullptr;
    OTiledMap::TileSet* m_pInfoTileset = nullptr;
};
