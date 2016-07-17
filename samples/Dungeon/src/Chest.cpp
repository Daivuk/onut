#include "Chest.h"

#include <onut/Entity.h>
#include <onut/Sound.h>
#include <onut/TiledMap.h>
#include <onut/TiledMapComponent.h>

int Chest::getGold() const
{
    return m_gold;
}

void Chest::setGold(int gold)
{
    m_gold = gold;
}

const OSoundRef& Chest::getOpenSound() const
{
    return m_pOpenSound;
}

void Chest::setOpenSound(const OSoundRef& pSound)
{
    m_pOpenSound = pSound;
}

void Chest::open()
{
    // Play the opening sound
    if (m_pOpenSound) m_pOpenSound->play();

    // Change the graphic to an open chest
    auto pTiledMapComponent = getParentComponent<OTiledMapComponent>();
    if (pTiledMapComponent)
    {
        auto pTiledMap = pTiledMapComponent->getTiledMap();
        auto pTileLayer = (OTiledMap::TileLayer*)pTiledMap->getLayer("obj");
        Vector2 muhPos = getLocalTransform().Translation();
        Point muhMapPos((int)(muhPos.x / 16.0f), (int)(muhPos.y / 16.0f));
        auto muhTile = pTiledMap->getTileAt(pTileLayer, muhMapPos.x, muhMapPos.y);
        pTiledMap->setTileAt(pTileLayer, muhMapPos.x, muhMapPos.y, muhTile + 1);
    }

    // Kill self
    destroy();
}
