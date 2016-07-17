#include <onut/Entity.h>
#include <onut/SceneManager.h>
#include <onut/Point.h>
#include <onut/TiledMap.h>
#include <onut/TiledMapComponent.h>

#include "Defines.h"
#include "SmartRoomCamera.h"

SmartRoomCamera::SmartRoomCamera()
    : OComponent(FLAG_UPDATABLE)
{
}

void SmartRoomCamera::onCreate()
{
    auto& pSceneManager = getSceneManager();

    m_pPlayer = pSceneManager->findEntity("player");
    m_pTiledMap = getParentComponent<OTiledMapComponent>()->getTiledMap();

    m_paint.assign(m_pTiledMap->getWidth() * m_pTiledMap->getHeight(), 0);
}

void SmartRoomCamera::onUpdate()
{
    // Here we will use a Fill algorithm to check the bounds of the current room the
    // player is in. And not scroll the view passed those bounds. Centering the room
    // if it is smaller than the bounds.
    Vector2 pos = m_pPlayer->getWorldTransform().Translation();
    Point mapPos((int)(pos.x / 16.0f), (int)(pos.y / 16.0f));

    auto pInfoLayer = dynamic_cast<OTiledMap::TileLayer*>(m_pTiledMap->getLayer("info"));
    auto infoTileset = m_pTiledMap->getTileSet("info");

    int w = m_pTiledMap->getWidth();
    int h = m_pTiledMap->getHeight();

    if (mapPos.x < 0 || mapPos.y < 0 || mapPos.x >= w || mapPos.y >= h) return;
    auto tileIdAtPos = pInfoLayer->tileIds[mapPos.y * w + mapPos.x] - infoTileset->firstId;
    if (tileIdAtPos != INFO_ROOM) return;

    Point minBound = mapPos;
    Point maxBound = mapPos;
    m_paint[mapPos.y * w + mapPos.x] = m_nextPaint;
    m_paintQueue.push_back(mapPos);
    while (!m_paintQueue.empty())
    {
        auto& point = m_paintQueue.back();

        minBound.x = onut::min(minBound.x, point.x);
        minBound.y = onut::min(minBound.y, point.y);
        maxBound.x = onut::max(maxBound.x, point.x);
        maxBound.y = onut::max(maxBound.y, point.y);

        if (point.x > 0 && m_paint[(point.y) * w + (point.x - 1)] != m_nextPaint)
        {
            auto tileId = pInfoLayer->tileIds[(point.y) * w + (point.x - 1)] - infoTileset->firstId;
            if (tileId == INFO_ROOM)
            {
                m_paint[(point.y) * w + (point.x - 1)] = m_nextPaint;
                m_paintQueue.push_back({point.x - 1, point.y});
                continue;
            }
        }
        if (point.x < w - 1 && m_paint[(point.y) * w + (point.x + 1)] != m_nextPaint)
        {
            auto tileId = pInfoLayer->tileIds[(point.y) * w + (point.x + 1)] - infoTileset->firstId;
            if (tileId == INFO_ROOM)
            {
                m_paint[(point.y) * w + (point.x + 1)] = m_nextPaint;
                m_paintQueue.push_back({point.x + 1, point.y});
                continue;
            }
        }
        if (point.y > 0 && m_paint[(point.y - 1) * w + (point.x)] != m_nextPaint)
        {
            auto tileId = pInfoLayer->tileIds[(point.y - 1) * w + (point.x)] - infoTileset->firstId;
            if (tileId == INFO_ROOM)
            {
                m_paint[(point.y - 1) * w + (point.x)] = m_nextPaint;
                m_paintQueue.push_back({point.x, point.y - 1});
                continue;
            }
        }
        if (point.y < h - 1 && m_paint[(point.y + 1) * w + (point.x)] != m_nextPaint)
        {
            auto tileId = pInfoLayer->tileIds[(point.y + 1) * w + (point.x)] - infoTileset->firstId;
            if (tileId == INFO_ROOM)
            {
                m_paint[(point.y + 1) * w + (point.x)] = m_nextPaint;
                m_paintQueue.push_back({point.x, point.y + 1});
                continue;
            }
        }
        m_paintQueue.pop_back();
    }
    ++m_nextPaint;

    if (maxBound.x - minBound.x < 10)
    {
        pos.x = ((float)minBound.x * 16.0f + (float)maxBound.x * 16.0f) / 2.0f + 8.0f;
    }
    else
    {
        pos.x = onut::max(((float)minBound.x + 5) * 16.0f, pos.x);
        pos.x = onut::min(((float)maxBound.x - 4) * 16.0f, pos.x);
    }
    if (maxBound.y - minBound.y < 7)
    {
        pos.y = ((float)minBound.y * 16.0f + (float)maxBound.y * 16.0f) / 2.0f + 8.0f;
    }
    else
    {
        pos.y = onut::max(((float)minBound.y + 3.5f) * 16.0f, pos.y);
        pos.y = onut::min(((float)maxBound.y - 2.5f) * 16.0f, pos.y);
    }

    setLocalTransform(Matrix::CreateTranslation(pos));
}
