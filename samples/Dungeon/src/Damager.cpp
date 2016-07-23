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

void Damager::addExclusion(const OEntityRef& pEntity)
{
    m_exclusions.push_back(pEntity);
}

void Damager::onTriggerEnter(const OCollider2DComponentRef& pCollider)
{
    auto pColliderEntity = pCollider->getEntity().get();
    for (auto& pEntity : m_exclusions)
    {
        if (pEntity.lock().get() == pColliderEntity) return;
    }
    Message message;
    message.damage = m_damage;
    message.direction = 
        Vector2(pCollider->getWorldTransform().Translation()) -
        Vector2(getWorldTransform().Translation());
    message.direction.Normalize();
    pCollider->sendMessage(Messages::Damage, &message);
}
