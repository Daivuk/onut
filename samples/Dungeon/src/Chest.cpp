#include "Chest.h"

#include <onut/Entity.h>
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

void Chest::open()
{
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

    int tmp;
    tmp = 5;
}
