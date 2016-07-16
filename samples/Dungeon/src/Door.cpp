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
            m_pCollider->setSize(m_openSize);

            // Change the tile id graphic. Open version are +2 ids
            auto tileId = m_pTileLayer->tileIds[m_mapPos.y * m_pTiledMap->getWidth() + m_mapPos.x];
            m_pTiledMap->setTileAt(m_pTileLayer, m_mapPos.x, m_mapPos.y, tileId + 2);

            // Set the tile to be passable, so the character can enter the door
            m_pTiledMapComponent->setPassable(m_mapPos, true);
        }
        else
        {
            m_pCollider->setSize(m_closeSize);
            auto tileId = m_pTileLayer->tileIds[m_mapPos.y * m_pTiledMap->getWidth() + m_mapPos.x];
            m_pTiledMap->setTileAt(m_pTileLayer, m_mapPos.x, m_mapPos.y, tileId - 2);
            m_pTiledMapComponent->setPassable(m_mapPos, false);
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
    return m_mapPos;
}

void Door::onCreate()
{
    Vector2 pos = getLocalTransform().Translation() / 16.0f;
    m_mapPos.x = (int)pos.x;
    m_mapPos.y = (int)pos.y;

    m_pCollider = getComponent<OCollider2DComponent>();
    if (m_pCollider)
    {
        m_closeSize = m_pCollider->getSize();
        m_openSize = m_closeSize / 2.0f;
    }

    m_pTiledMapComponent = getParentComponent<OTiledMapComponent>();
    m_pTiledMap = m_pTiledMapComponent->getTiledMap();
    m_pTileLayer = (OTiledMap::TileLayer*)m_pTiledMap->getLayer("tiles");
}
