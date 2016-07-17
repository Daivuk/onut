#include "Chest.h"

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
    destroy();
}
