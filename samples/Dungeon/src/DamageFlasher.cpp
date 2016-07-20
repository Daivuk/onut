#include "DamageFlasher.h"
#include "Defines.h"

#include <onut/Entity.h>
#include <onut/SpriteAnimComponent.h>
#include <onut/SpriteComponent.h>
#include <onut/Timing.h>

DamageFlasher::DamageFlasher()
    : OComponent(FLAG_UPDATABLE)
{
}

void DamageFlasher::onCreate()
{
    m_pSpriteAnimComponent = getComponent<OSpriteAnimComponent>();
    m_pSpriteComponent = getComponent<OSpriteComponent>();
}

void DamageFlasher::onUpdate()
{
    if (m_time > 0.0f)
    {
        m_time -= ODT;
        Color color = Color::White;
        if (m_time > 0.0f)
        {
            if ((int)(m_time * 15) % 2)
            {
                color = Color(100, 100, 100, 1);
            }
        }
        if (m_pSpriteAnimComponent)
        {
            m_pSpriteAnimComponent->setColor(color);
        }
        if (m_pSpriteComponent)
        {
            m_pSpriteComponent->setColor(color);
        }
    }
}

void DamageFlasher::onMessage(int messageId, void* pData)
{
    if (messageId == Messages::Damage)
    {
        m_time = .8f;
    }
}
