#include <onut/Component.h>

#include <onut/ForwardDeclaration.h>
OForwardDeclare(TiledMap);
ForwardDeclare(MapCollider);

class MapCollider final : public OComponent
{
public:
    void onCreate() override;
    void onUpdate() override;

private:
    OTiledMapRef m_pTiledMap;
    Vector2 m_lastPosition;
    OTiledMap::TileLayer* m_pInfoLayer = nullptr;
    OTiledMap::TileSet* m_pInfoTileset = nullptr;
};
