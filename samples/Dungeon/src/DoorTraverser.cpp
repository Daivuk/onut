#include <onut/Entity.h>
#include <onut/EntityManager.h>
#include <onut/TiledMapComponent.h>

#include "DoorTraverser.h"
#include "MapCollider.h"

void DoorTraverser::onCreate()
{
    auto& pEntity = getEntity();
    auto& pEntityManager = getEntity()->getEntityManager();

    m_pTiledMap = pEntityManager->findEntity("dungeon")->getComponent<OTiledMapComponent>()->getTiledMap();
    m_pLayer = (OTiledMap::TileLayer*)m_pTiledMap->getLayer("info");
}
