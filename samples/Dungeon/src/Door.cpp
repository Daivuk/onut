#include "Defines.h"
#include "Door.h"

#include <onut/Collider2DComponent.h>
#include <onut/Entity.h>
#include <onut/Maths.h>
#include <onut/Sound.h>
#include <onut/TiledMapComponent.h>

#include <string>
#include <unordered_map>

bool Door::getOpen() const
{
    return m_open;
}

void Door::setOpen(bool open)
{
    if (m_open == open) return;
    m_open = open;

    if (m_pCollider)
    {
        if (m_open)
        {
            // We make the hit box smaller so the character needs to move inside the door's hole to traverse it
            auto openSize = m_closeSize;
            auto mapPos = getMapPos();
            if (m_pTiledMapComponent->getPassable(Point(mapPos.x - 1, mapPos.y)) ||
                m_pTiledMapComponent->getPassable(Point(mapPos.x + 1, mapPos.y)))
            {
                openSize.x = 1.0f;
            }
            else if (m_pTiledMapComponent->getPassable(Point(mapPos.x, mapPos.y - 1)) ||
                     m_pTiledMapComponent->getPassable(Point(mapPos.x, mapPos.y + 1)))
            {
                openSize.y = 1.0f;
            }
            m_pCollider->setSize(openSize);

            // Change the tiles id graphic. Open version are +2 ids
            for (auto& p : m_mapPositions)
            {
                auto tileId = m_pTileLayer->tileIds[p.y * m_pTiledMap->getWidth() + p.x];
                m_pTiledMap->setTileAt(m_pTileLayer, p.x, p.y, tileId + 2);

                // Set the tile to be passable, so the character can enter the door
                m_pTiledMapComponent->setPassable(p, true);
            }
        }
        else
        {
            m_pCollider->setSize(m_closeSize);
            for (auto& p : m_mapPositions)
            {
                auto tileId = m_pTileLayer->tileIds[p.y * m_pTiledMap->getWidth() + p.x];
                m_pTiledMap->setTileAt(m_pTileLayer, p.x, p.y, tileId - 2);
                m_pTiledMapComponent->setPassable(p, false);
            }
        }
    }
}

OSoundRef Door::getOpenSound() const
{
    return m_pOpenSound;
}

void Door::setOpenSound(const OSoundRef& pSound)
{
    m_pOpenSound = pSound;
}

OSoundRef Door::getCloseSound() const
{
    return m_pCloseSound;
}

void Door::setCloseSound(const OSoundRef& pSound)
{
    m_pCloseSound = pSound;
}

OEntityRef Door::getTarget() const
{
    return m_pTargetDoor.lock();
}

void Door::setTarget(const OEntityRef& pTarget)
{
    m_pTargetDoor = pTarget;
}

const Point& Door::getMapPos() const
{
    return m_mapPositions[0];
}

bool Door::getNeedBomb() const
{
    return m_needBomb;
}

void Door::setNeedBomb(bool needBomb)
{
    m_needBomb = needBomb;
}

Vector2 Door::getExitPosition() const
{
    Vector2 exitPos = getLocalTransform().Translation();
    Vector2 exitOffset;
    for (auto& mapPos : m_mapPositions)
    {
        if (m_pTiledMapComponent->getPassable(Point(mapPos.x - 1, mapPos.y)))
        {
            exitOffset.x -= 12.0f;
        }
        if (m_pTiledMapComponent->getPassable(Point(mapPos.x + 1, mapPos.y)))
        {
            exitOffset.x += 12.0f;
        }
        if (m_pTiledMapComponent->getPassable(Point(mapPos.x, mapPos.y - 1)))
        {
            exitOffset.y -= 12.0f;
        }
        if (m_pTiledMapComponent->getPassable(Point(mapPos.x, mapPos.y + 1)))
        {
            exitOffset.y += 12.0f;
        }
    }
    exitOffset.x = onut::max(-12.0f, exitOffset.x);
    exitOffset.x = onut::min(12.0f, exitOffset.x);
    exitOffset.y = onut::max(-12.0f, exitOffset.y);
    exitOffset.y = onut::min(12.0f, exitOffset.y);
    return exitPos + exitOffset;
}

void Door::onCreate()
{
    m_pCollider = getComponent<OCollider2DComponent>();
    if (m_pCollider)
    {
        m_closeSize = m_pCollider->getSize();

        // Use the collider size to find out which tiles of the map we overlay.
        auto size = m_closeSize;
        Vector2 pos = getLocalTransform().Translation();
        size.x -= 2.0f;
        size.y -= 2.0f;
        for (float y = pos.y - size.y / 2.0f; y < pos.y + size.y / 2.0f; y += 16.0f)
        {
            for (float x = pos.x - size.x / 2.0f; x < pos.x + size.x / 2.0f; x += 16.0f)
            {
                m_mapPositions.push_back(Point((int)(x / 16.0f), (int)(y / 16.0f)));
            }
        }
    }

    m_pTiledMapComponent = getParentComponent<OTiledMapComponent>();
    m_pTiledMap = m_pTiledMapComponent->getTiledMap();
    m_pTileLayer = (OTiledMap::TileLayer*)m_pTiledMap->getLayer("tiles");
}

void Door::onMessage(int messageId, void* pData)
{
    if (messageId == Messages::Damage && !getOpen())
    {
        if (*(int*)pData >= 5)
        {
            // This was at least a bomb, destroy the wall
            setOpen(true);
        }
    }
}
