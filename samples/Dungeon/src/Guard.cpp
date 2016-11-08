#include "DamageFlasher.h"
#include "Damager.h"
#include "Defines.h"
#include "Dungeon.h"
#include "Guard.h"

#include <onut/Collider2DComponent.h>
#include <onut/Entity.h>
#include <onut/EntityFactory.h>
#include <onut/SceneManager.h>
#include <onut/Sound.h>
#include <onut/SpriteAnimComponent.h>
#include <onut/Timing.h>

static const float ATTACK_DIST = 20.0f;
static const float MOVE_SPEED = 32.0f;
static const float TURN_EPSILON = 8.0f;
static const float PREPARE_ATTACK_TIME = 0.5f;

Guard::Guard()
    : OComponent(FLAG_UPDATABLE)
{
}

const std::string& Guard::getDir() const
{
    return m_dir;
}

void Guard::setDir(const std::string& dir)
{
    m_dir = dir;
}

void Guard::onCreate()
{
    m_resetDir = m_dir;

    m_pCollider = getComponent<OCollider2DComponent>();
    m_pSpriteAnimComponent = getComponent<OSpriteAnimComponent>();

    // Create the sword anim entity with it's damage collider trigger
    m_pSwordAttackEntity = OCreateSpriteAnimEntity("swordAttack.spriteanim");
    m_pSwordAttackEntity->setVisible(false);
    m_pSwordAttackEntity->setEnabled(false);

    // The trigger for the sword
    auto pSwordTrigger = OCreateCollider2DEntity(Vector2(20, 28), Vector2(11, 0));
    m_pSwordCollider = pSwordTrigger->getComponent<OCollider2DComponent>();
    m_pSwordCollider->setTrigger(true);
    m_pSwordCollider->setPhysicScale(16.0f);
    m_pSwordAttackEntity->add(pSwordTrigger);
    m_pSwordCollider->setEnabled(false);

    // The damager
    auto pDamager = pSwordTrigger->addComponent<Damager>();
    pDamager->setDamage(1);
    pDamager->addExclusion(getEntity());

    getEntity()->add(m_pSwordAttackEntity);
}

void Guard::onMessage(int messageId, void* pData)
{
    if (messageId == Messages::Reset)
    {
        m_dir = m_resetDir;
        m_pSpriteAnimComponent->stop();
        m_pSpriteAnimComponent->play("run_" + m_dir);
    }
}

void Guard::attack()
{
    m_state = State::Attacking;
    m_pSpriteAnimComponent->play("attack_" + m_dir);

    // Show the attack effect
    m_pSwordAttackEntity->setDrawIndex(DrawIndexes::SwordAttack);
    if (m_dir == "s")
    {
        m_pSwordAttackEntity->setLocalTransform(Matrix::CreateRotationZ(OPIDIV2) * Matrix::CreateTranslation(Vector2(0.0f, -4.0f)));
        m_pSwordCollider->setSize(Vector2(28, 20));
    }
    else if (m_dir == "e")
    {
        m_pSwordAttackEntity->setLocalTransform(Matrix::CreateTranslation(Vector2(0.0f, -4.0f)));
        m_pSwordCollider->setSize(Vector2(20, 28));
    }
    else if (m_dir == "w")
    {
        m_pSwordAttackEntity->setLocalTransform(Matrix::CreateRotationZ(OPI) * Matrix::CreateTranslation(Vector2(0.0f, -4.0f)));
        m_pSwordCollider->setSize(Vector2(20, 28));
    }
    else if (m_dir == "n")
    {
        m_pSwordAttackEntity->setLocalTransform(Matrix::CreateRotationZ(-OPIDIV2) * Matrix::CreateTranslation(Vector2(0.0f, -4.0f)));
        m_pSwordCollider->setSize(Vector2(28, 20));
    }
    m_pSwordAttackEntity->setVisible(true);
    m_pSwordAttackEntity->setEnabled(true);
    m_pSwordCollider->setEnabled(true);
    auto pSpriteAnimComponent = m_pSwordAttackEntity->getComponent<OSpriteAnimComponent>();
    pSpriteAnimComponent->stop();
    pSpriteAnimComponent->play("attack");

    // Play a sound
    OPlaySound("swoosh.wav", 0.75f);
}

void Guard::followPlayer()
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
            if ((std::abs(dir.x) < std::abs(dir.y) && std::abs(dir.x) > TURN_EPSILON) || std::abs(dir.y) <= TURN_EPSILON)
            {
                if (dir.x > 0.0f)
                {
                    dir = {1, 0};
                }
                else
                {
                    dir = {-1, 0};
                }
            }
            else
            {
                if (dir.y > 0.0f)
                {
                    dir = {0, 1};
                }
                else
                {
                    dir = {0, -1};
                }
            }

            m_pCollider->setVelocity(m_pCollider->getVelocity() + dir * MOVE_SPEED);
        }
        else
        {
            // Look toward the player
            dir.Normalize();

            // Prepare to attack
            m_prepareAttackDelay = PREPARE_ATTACK_TIME;
            m_state = State::PrepareAttack;
        }

        if (dir.y > .7f) m_dir = "s";
        else if (dir.x < -.7f) m_dir = "w";
        else if (dir.x > .7f) m_dir = "e";
        else if (dir.y < -.7f) m_dir = "n";
    
        // Update animation
        if (m_pSpriteAnimComponent)
        {
            m_pSpriteAnimComponent->play("run_" + m_dir);
        }
    }
    else
    {
        if (m_pSpriteAnimComponent)
        {
            m_pSpriteAnimComponent->play("idle_" + m_dir);
        }
    }
}

void Guard::prepareAttack()
{
    m_prepareAttackDelay -= ODT;
    if (m_prepareAttackDelay <= 0.0f)
    {
        attack();
        return;
    }
    
    // Update animation
    if (m_pSpriteAnimComponent)
    {
        m_pSpriteAnimComponent->play("idle_" + m_dir);
    }
}

void Guard::onUpdate()
{
    switch (m_state)
    {
        case State::Follow:
        {
            followPlayer();
            break;
        }
        case State::PrepareAttack:
        {
            prepareAttack();
            break;
        }
        case State::Attacking:
            if (!m_pSwordAttackEntity->getComponent<OSpriteAnimComponent>()->isPlaying())
            {
                m_state = State::Follow;
                m_pSwordAttackEntity->setVisible(false);
                m_pSwordAttackEntity->setEnabled(false);
                m_pSwordCollider->setEnabled(false);
            }
            break;
    }
}

void Guard::onDestroy()
{
    auto pPuff = OCreateSpriteAnimEntity("puff.spriteanim", getLocalTransform().Translation());
    pPuff->getComponent<OSpriteAnimComponent>()->setDefaultAnim("puff");
    auto pParent = getEntity()->getParent();
    if (pParent) pParent->add(pPuff);
    OPlaySound("puff.wav");
}
