#include "Damager.h"
#include "Defines.h"
#include "PushBack.h"

#include <onut/Entity.h>
#include <onut/Collider2DComponent.h>
#include <onut/Timing.h>

PushBack::PushBack()
    : OComponent(FLAG_UPDATABLE)
{
}

float PushBack::getStrength() const
{
    return m_strength;
}

void PushBack::setStrength(float strength)
{
    m_strength = strength;
}

void PushBack::onCreate()
{
    m_pCollider = getComponent<OCollider2DComponent>();
}

void PushBack::onUpdate()
{
    if (m_time > 0.0f)
    {
        m_time -= ODT;
        auto s = m_strength / .25f * (m_time / .25f * 2);
        m_pCollider->setVelocity(m_pCollider->getVelocity() + m_direction * s);
    }
}

void PushBack::onMessage(int messageId, void* pData)
{
    if (messageId == Messages::Damage)
    {
        m_time = .25f;
        auto pMessage = (Damager::Message*)pData;
        m_direction = pMessage->direction;
    }
}
