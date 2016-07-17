#include "Chest.h"
#include "TreasureHunter.h"

#include <onut/Collider2DComponent.h>
#include <onut/Entity.h>

void TreasureHunter::onTriggerEnter(const OCollider2DComponentRef& pCollider)
{
    auto pChest = pCollider->getComponent<Chest>();
    if (pChest)
    {
        auto gold = pChest->getGold();
        pChest->open();
    }
}
