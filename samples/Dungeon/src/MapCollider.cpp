#include <onut/Entity.h>
#include <onut/EntityManager.h>
#include <onut/Point.h>
#include <onut/TiledMap.h>
#include <onut/TiledMapComponent.h>

#include "MapCollider.h"

static const float COLLIDER_RADIUS = 0.35f;
static const uint32_t INFO_WALKABLE = 0;

void MapCollider::onCreate()
{
    auto& pEntity = getEntity();
    auto& pEntityManager = getEntity()->getEntityManager();

    m_pTiledMap = pEntityManager->findEntity("dungeon")->getComponent<OTiledMapComponent>()->getTiledMap();
    m_pInfoLayer = dynamic_cast<OTiledMap::TileLayer*>(m_pTiledMap->getLayer("info"));
    m_pInfoTileset = m_pTiledMap->getTileSet("info");

    m_lastPosition = pEntity->getLocalTransform().Translation();
}

static bool walkableAt(const Vector2& pos, OTiledMap::TileLayer* pLayer, OTiledMap::TileSet* pTileset)
{
    Point mapPos((int)(pos.x / 16.0f), (int)(pos.y / 16.0f));
    if (mapPos.x < 0 || mapPos.y < 0 || mapPos.x >= pLayer->width || mapPos.y >= pLayer->height) return false;
    auto tileIdAtPos = pLayer->tileIds[mapPos.y * pLayer->width + mapPos.x] - pTileset->firstId;
    return tileIdAtPos == INFO_WALKABLE;
}

void MapCollider::onUpdate()
{
    Vector2 currentPosition = getEntity()->getLocalTransform().Translation();
    if (currentPosition == m_lastPosition) return;

    if (!walkableAt(currentPosition, m_pInfoLayer, m_pInfoTileset))
    {
        getEntity()->setLocalTransform(Matrix::CreateTranslation(m_lastPosition));
    }
    else
    {
        m_lastPosition = currentPosition;
    }
}
