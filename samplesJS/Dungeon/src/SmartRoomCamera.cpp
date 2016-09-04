#include <onut/Entity.h>
#include <onut/SceneManager.h>
#include <onut/Point.h>
#include <onut/TiledMap.h>
#include <onut/TiledMapComponent.h>

#include "Defines.h"
#include "Dungeon.h"
#include "SmartRoomCamera.h"

SmartRoomCamera::SmartRoomCamera()
    : OComponent(FLAG_UPDATABLE)
{
}

void SmartRoomCamera::onCreate()
{
    auto& pSceneManager = getSceneManager();

    m_pTiledMap = getParentComponent<OTiledMapComponent>()->getTiledMap();

    m_paint.assign(m_pTiledMap->getWidth() * m_pTiledMap->getHeight(), 0);
}

void SmartRoomCamera::RefreshFogOfWar()
{
    auto pFowLayer = (OTiledMap::TileLayer*)m_pTiledMap->getLayer("fow");
    int w = pFowLayer->width;
    int h = pFowLayer->height;
    int x, y, x2, y2;

    // Fill the map with black
    for (y = 0; y < h; ++y)
    {
        for (x = 0; x < w; ++x)
        {
            m_pTiledMap->setTileAt(pFowLayer, x, y, 21);
        }
    }

    // Reveal the room + 2 tiles on each sides
    for (auto& k : m_pRoom->tileIdSet)
    {
        x = k % w;
        y = k / w;
        for (y2 = y - 2; y2 <= y + 2; ++y2)
        {
            for (x2 = x - 2; x2 <= x + 2; ++x2)
            {
                m_pTiledMap->setTileAt(pFowLayer, x2, y2, 0);
            }
        }
    }
}

void SmartRoomCamera::onUpdate()
{
    // Here we will check the bounds of the current room the player is in
    // and not scroll the view passed those bounds. Centering the room
    // if it is smaller than the bounds.
    auto pPlayer = g_pDungeon->getPlayer();
    if (pPlayer)
    {
        Vector2 pos = pPlayer->getWorldTransform().Translation();
        auto pRoom = g_pDungeon->getRoomAt(pos);
        if (!pRoom) return;
        if (pRoom != m_pRoom)
        {
            if (m_pRoom) boardcastMessage(Messages::LeaveRoom, (void*)m_pRoom);
            boardcastMessage(Messages::EnterRoom, (void*)pRoom);
            m_pRoom = pRoom;
            RefreshFogOfWar();
        }

        if (pRoom->bound.right - pRoom->bound.left < 10)
        {
            pos.x = ((float)pRoom->bound.left * 16.0f + (float)pRoom->bound.right * 16.0f) / 2.0f + 8.0f;
        }
        else
        {
            pos.x = onut::max(((float)pRoom->bound.left + 5) * 16.0f, pos.x);
            pos.x = onut::min(((float)pRoom->bound.right - 4) * 16.0f, pos.x);
        }
        if (pRoom->bound.bottom - pRoom->bound.top < 7)
        {
            pos.y = ((float)pRoom->bound.top * 16.0f + (float)pRoom->bound.bottom * 16.0f) / 2.0f + 8.0f;
        }
        else
        {
            pos.y = onut::max(((float)pRoom->bound.top + 3.5f) * 16.0f, pos.y);
            pos.y = onut::min(((float)pRoom->bound.bottom - 2.5f) * 16.0f, pos.y);
        }

        setLocalTransform(Matrix::CreateTranslation(pos));
    }
}
