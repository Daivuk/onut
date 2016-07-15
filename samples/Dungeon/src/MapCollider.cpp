#include <onut/Entity.h>
#include <onut/EntityManager.h>
#include <onut/Point.h>
#include <onut/TiledMap.h>
#include <onut/TiledMapComponent.h>

#include "MapCollider.h"

static const float COLLIDER_RADIUS = 3.0f;
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

    Point lastMapPos((int)(m_lastPosition.x / 16.0f), (int)(m_lastPosition.y / 16.0f));

    auto dir = currentPosition - m_lastPosition;
    auto result = currentPosition;
    if (dir.x < 0)
    {
        if (!walkableAt(Vector2(currentPosition.x - COLLIDER_RADIUS, m_lastPosition.y - COLLIDER_RADIUS), m_pInfoLayer, m_pInfoTileset))
        {
            result.x = (float)(lastMapPos.x) * 16.0f + COLLIDER_RADIUS;
        }
        else if (!walkableAt(Vector2(currentPosition.x - COLLIDER_RADIUS, m_lastPosition.y + COLLIDER_RADIUS), m_pInfoLayer, m_pInfoTileset))
        {
            result.x = (float)(lastMapPos.x) * 16.0f + COLLIDER_RADIUS;
        }
    }
    else if (dir.x > 0)
    {
        if (!walkableAt(Vector2(currentPosition.x + COLLIDER_RADIUS, m_lastPosition.y - COLLIDER_RADIUS), m_pInfoLayer, m_pInfoTileset))
        {
            result.x = (float)(lastMapPos.x + 1) * 16.0f - COLLIDER_RADIUS - 0.1f;
        }
        else if (!walkableAt(Vector2(currentPosition.x + COLLIDER_RADIUS, m_lastPosition.y + COLLIDER_RADIUS), m_pInfoLayer, m_pInfoTileset))
        {
            result.x = (float)(lastMapPos.x + 1) * 16.0f - COLLIDER_RADIUS - 0.1f;
        }
    }
    if (dir.y < 0)
    {
        if (!walkableAt(Vector2(m_lastPosition.x - COLLIDER_RADIUS, currentPosition.y - COLLIDER_RADIUS), m_pInfoLayer, m_pInfoTileset))
        {
            result.y = (float)(lastMapPos.y) * 16.0f + COLLIDER_RADIUS;
        }
        else if(!walkableAt(Vector2(m_lastPosition.x + COLLIDER_RADIUS, currentPosition.y - COLLIDER_RADIUS), m_pInfoLayer, m_pInfoTileset))
        {
            result.y = (float)(lastMapPos.y) * 16.0f + COLLIDER_RADIUS;
        }
    }
    else if (dir.y > 0)
    {
        if (!walkableAt(Vector2(m_lastPosition.x - COLLIDER_RADIUS, currentPosition.y + COLLIDER_RADIUS), m_pInfoLayer, m_pInfoTileset))
        {
            result.y = (float)(lastMapPos.y + 1) * 16.0f - COLLIDER_RADIUS - 0.1f;
        }
        else if(!walkableAt(Vector2(m_lastPosition.x + COLLIDER_RADIUS, currentPosition.y + COLLIDER_RADIUS), m_pInfoLayer, m_pInfoTileset))
        {
            result.y = (float)(lastMapPos.y + 1) * 16.0f - COLLIDER_RADIUS - 0.1f;
        }
    }

    m_lastPosition = result;
    getEntity()->setLocalTransform(Matrix::CreateTranslation(m_lastPosition));
}
