#include "Defines.h"
#include "Dungeon.h"
#include "Vase.h"

#include <onut/Entity.h>
#include <onut/Sound.h>
#include <onut/TiledMap.h>
#include <onut/TiledMapComponent.h>

int Vase::getGold() const
{
    return m_gold;
}

void Vase::setGold(int gold)
{
    m_gold = gold;
}

void Vase::onMessage(int messageId, void* pData)
{
    if (messageId == Messages::Damage)
    {
        auto damage = *(int*)pData;
        if (damage >= 1)
        {
            // Play a nice lil' sound
            OPlaySound("potBreak.wav");

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

            destroy();
        }
    }
}
