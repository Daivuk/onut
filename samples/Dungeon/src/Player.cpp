#include "Bomb.h"
#include "Damager.h"
#include "Dungeon.h"
#include "Defines.h"
#include "Player.h"

#include <onut/Anim.h>
#include <onut/Collider2DComponent.h>
#include <onut/Entity.h>
#include <onut/EntityFactory.h>
#include <onut/GamePad.h>
#include <onut/Input.h>
#include <onut/onut.h>
#include <onut/Random.h>
#include <onut/SceneManager.h>
#include <onut/Sound.h>
#include <onut/SpriteAnim.h>
#include <onut/SpriteAnimComponent.h>
#include <onut/Timing.h>

extern OAnimFloat g_fadeAnim;

Player::Player()
    : OComponent(FLAG_UPDATABLE)
{
}

float Player::getSpeed() const
{
    return m_speed;
}

void Player::setSpeed(float speed)
{
    m_speed = speed;
}

void Player::onCreate()
{
    m_pSpriteAnimComponent = getComponent<OSpriteAnimComponent>();
    m_pCollider2DComponent = getComponent<OCollider2DComponent>();

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

const OSoundRef& Player::getStepSound() const
{
    return m_pStepSound;
}

void Player::setStepSound(const OSoundRef& pSound)
{
    m_pStepSound = pSound;
}

int Player::getBombCount() const
{
    return m_bombs;
}

void Player::setBombCount(int count)
{
    m_bombs = count;
}

int Player::getCoinCount() const
{
    return m_coins;
}

void Player::setCoinCount(int count)
{
    m_coins = count;
}

void Player::placeBomb()
{
    if (!m_bombs) return;
    --m_bombs;

    auto pDungeon = g_pDungeon->getEntity();
    if (pDungeon)
    {
        auto pBomb = OCreateSpriteAnimEntity("bomb.spriteanim", getLocalTransform().Translation(), "fuse");

        pBomb->addComponent<Bomb>();

        auto pBombCollider = pBomb->addComponent<OCollider2DComponent>();
        pBombCollider->setSize(Vector2(32, 32));
        pBombCollider->setTrigger(true);
        pBombCollider->setEnabled(false);

        auto pBombDamager = pBomb->addComponent<Damager>();
        pBombDamager->setDamage(5);

        pDungeon->add(pBomb);
    }
}

void Player::attack()
{
    m_state = State::Attacking;
    m_pCollider2DComponent->setVelocity(Vector2::Zero);
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

void Player::onUpdate()
{
    switch (m_state)
    {
        case State::Idle:
        case State::Moving:
        {
            if (OInputJustPressed(OKeySpaceBar) || OInputJustPressed(OXArcadeLButton1))
            {
                attack();
                return;
            }

            if (OInputJustPressed(OKeyB) || OInputJustPressed(OXArcadeLButton2))
            {
                placeBomb();
            }

            Vector2 dir;
            bool isMoving = false;

            dir = OGetGamePad(0)->getLeftThumb();

            if (OInputPressed(OKeyA) || OGamePadPressed(OGamePadDPadLeft) || OInputPressed(OXArcadeLJoyLeft))
            {
                dir -= Vector2::UnitX;
                isMoving = true;
            }
            else if (OInputPressed(OKeyD) || OGamePadPressed(OGamePadDPadRight) || OInputPressed(OXArcadeLJoyRight))
            {
                dir += Vector2::UnitX;
                isMoving = true;
            }
            if (OInputPressed(OKeyW) || OGamePadPressed(OGamePadDPadUp) || OInputPressed(OXArcadeLJoyUp))
            {
                dir -= Vector2::UnitY;
                isMoving = true;
            }
            else if (OInputPressed(OKeyS) || OGamePadPressed(OGamePadDPadDown) || OInputPressed(OXArcadeLJoyDown))
            {
                dir += Vector2::UnitY;
                isMoving = true;
            }

            if (!isMoving)
            {
                m_state = State::Idle;
                m_stepDelay = 0.15f;
                if (m_pSpriteAnimComponent) m_pSpriteAnimComponent->play("idle_" + m_dir);
                return;
            }

            m_state = State::Moving;

            dir.Normalize();

            if (dir.y > .7f) m_dir = "s";
            else if (dir.x < -.7f) m_dir = "w";
            else if (dir.x > .7f) m_dir = "e";
            else if (dir.y < -.7f) m_dir = "n";

            if (m_pSpriteAnimComponent) m_pSpriteAnimComponent->play("run_" + m_dir);

            m_stepDelay -= ODT;
            if (m_stepDelay <= 0.0f)
            {
                m_stepDelay = 0.3f;
                if (m_pStepSound)
                {
                    m_pStepSound->play(0.25f, 0.0f, ORandFloat(.9f, 1.1f));
                }
            }

            Vector2 position = getLocalTransform().Translation();
            m_pCollider2DComponent->setVelocity(m_pCollider2DComponent->getVelocity() + dir * m_speed);

            break;
        }
        case State::Attacking:
            if (!m_pSwordAttackEntity->getComponent<OSpriteAnimComponent>()->isPlaying())
            {
                m_state = State::Idle;
                m_pSwordAttackEntity->setVisible(false);
                m_pSwordAttackEntity->setEnabled(false);
                m_pSwordCollider->setEnabled(false);
            }
            break;
    };
}

void Player::onDestroy()
{
    getSceneManager()->setPause(true);

    g_fadeAnim.playFromCurrent(1.0f, 2.0f, OTweenLinear);
    g_fadeAnim.queue(0.0f, .5f, OTweenLinear, []{ OQuit(); });

    auto pDyingPlayer = OCreateSpriteAnimEntity("baltAnims.spriteanim", getWorldTransform().Translation(), "die");
    pDyingPlayer->setDrawIndex(DrawIndexes::DyingPlayer);
    pDyingPlayer->getComponent<OSpriteAnimComponent>()->setPlayPaused(true);

    OPlaySound("gameover.wav");
}
