#include "Damager.h"
#include "Defines.h"

#include <onut/Collider2DComponent.h>

int Damager::getDamage() const
{
    return m_damage;
}

void Damager::setDamage(int damage)
{
    m_damage = damage;
}

void Damager::onTriggerEnter(const OCollider2DComponentRef& pCollider)
{
    pCollider->sendMessage(MESSAGE_DAMAGE, &m_damage);
}
