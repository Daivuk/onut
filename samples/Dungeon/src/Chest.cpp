#include "Chest.h"
#include "Dungeon.h"

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

int Chest::getBomb() const
{
    return m_bomb;
}

void Chest::setBomb(int bomb)
{
    m_bomb = bomb;
}

void Chest::open()
{
    // Play the opening sound
    OPlaySound("chestOpen.wav");

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

    // Spawn the gold
    if (m_gold)
    {
        g_pDungeon->showCoinAt(getLocalTransform().Translation(), m_gold);
    }
    if (m_bomb)
    {
        g_pDungeon->showBombAt(getLocalTransform().Translation(), m_bomb);
    }

    // Kill self
    destroy();
}
