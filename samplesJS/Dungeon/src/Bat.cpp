#include "Bat.h"
#include "DamageFlasher.h"
#include "Damager.h"
#include "Defines.h"
#include "Dungeon.h"

#include <onut/Collider2DComponent.h>
#include <onut/Entity.h>
#include <onut/EntityFactory.h>
#include <onut/SceneManager.h>
#include <onut/Sound.h>
#include <onut/SpriteAnimComponent.h>
#include <onut/Timing.h>

static const float ATTACK_DIST = 12.0f;
static const float MOVE_SPEED = 24.0f;
static const float ATTACK_DELAY = 1.5f;

Bat::Bat()
    : OComponent(FLAG_UPDATABLE)
{
}

void Bat::onCreate()
{
    m_pCollider = getComponent<OCollider2DComponent>();
    m_pSpriteAnimComponent = getComponent<OSpriteAnimComponent>();
}

void Bat::onMessage(int messageId, void* pData)
{
    if (messageId == Messages::Reset)
    {
        m_pSpriteAnimComponent->stop();
        m_pSpriteAnimComponent->play("fly");
    }
}

void Bat::followPlayer()
{
    // Move toward player
    auto pPlayer = g_pDungeon->getPlayer();
    if (pPlayer)
    {
        Vector2 myPosition = getWorldTransform().Translation();
        Vector2 playerPosition = pPlayer->getWorldTransform().Translation();

        auto dir = playerPosition - myPosition;
        if (dir.LengthSquared() > ATTACK_DIST * ATTACK_DIST)
        {
            dir.Normalize();
            m_pCollider->setVelocity(m_pCollider->getVelocity() + dir * MOVE_SPEED);
        }
        else
        {
            dir.Normalize();

            m_state = State::Attacking;
            m_pSpriteAnimComponent->stop();
            m_pSpriteAnimComponent->play("attack");
            OPlaySound("bat_attack.wav", 0.75f);
            m_attackDelay = ATTACK_DELAY;

            Damager::Message msg;
            msg.damage = 1;
            msg.direction = dir;
            pPlayer->sendMessage(Messages::Damage, &msg);
        }
    }
}

void Bat::onUpdate()
{
    switch (m_state)
    {
        case State::Follow:
        {
            followPlayer();
            break;
        }
        case State::Attacking:
        {
            m_attackDelay -= ODT;
            if (m_attackDelay < 0.0f)
            {
                m_state = State::Follow;
            }
            break;
        }
    }
}

void Bat::onDestroy()
{
    auto pParent = getEntity()->getParent();
    if (pParent)
    {
        auto pDeadEntity = OCreateSpriteAnimEntity("bat.spriteanim", getLocalTransform().Translation());
        pDeadEntity->getComponent<OSpriteAnimComponent>()->setDefaultAnim("die");
        OPlaySound("bat_wake.wav");
    }
}
