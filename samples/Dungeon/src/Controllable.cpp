#include "Controllable.h"
#include "Damager.h"
#include "Defines.h"

#include <onut/Collider2DComponent.h>
#include <onut/Entity.h>
#include <onut/EntityFactory.h>
#include <onut/GamePad.h>
#include <onut/Input.h>
#include <onut/Random.h>
#include <onut/Sound.h>
#include <onut/SpriteAnimComponent.h>
#include <onut/Timing.h>

Controllable::Controllable()
    : OComponent(FLAG_UPDATABLE)
{
}

float Controllable::getSpeed() const
{
    return m_speed;
}

void Controllable::setSpeed(float speed)
{
    m_speed = speed;
}

void Controllable::onCreate()
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

const OSoundRef& Controllable::getStepSound() const
{
    return m_pStepSound;
}

void Controllable::setStepSound(const OSoundRef& pSound)
{
    m_pStepSound = pSound;
}

void Controllable::attack()
{
    m_state = State::Attacking;
    m_pCollider2DComponent->setVelocity(Vector2::Zero);
    m_pSpriteAnimComponent->play("attack_" + m_dir);

    // Show the attack effect
    m_pSwordAttackEntity->setDrawIndex(SWORD_ATTACK_DRAW_INDEX);
    if (m_dir == "s")
    {
        m_pSwordAttackEntity->setLocalTransform(Matrix::CreateRotationZ(DirectX::XM_PIDIV2) * Matrix::CreateTranslation(Vector2(0.0f, -4.0f)));
        m_pSwordCollider->setSize(Vector2(28, 20));
    }
    else if (m_dir == "e")
    {
        m_pSwordAttackEntity->setLocalTransform(Matrix::CreateTranslation(Vector2(0.0f, -4.0f)));
        m_pSwordCollider->setSize(Vector2(20, 28));
    }
    else if (m_dir == "w")
    {
        m_pSwordAttackEntity->setLocalTransform(Matrix::CreateRotationZ(DirectX::XM_PI) * Matrix::CreateTranslation(Vector2(0.0f, -4.0f)));
        m_pSwordCollider->setSize(Vector2(20, 28));
    }
    else if (m_dir == "n")
    {
        m_pSwordAttackEntity->setLocalTransform(Matrix::CreateRotationZ(-DirectX::XM_PIDIV2) * Matrix::CreateTranslation(Vector2(0.0f, -4.0f)));
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

void Controllable::onUpdate()
{
    switch (m_state)
    {
        case State::Idle:
        case State::Moving:
        {
            if (OInputJustPressed(OKeySpaceBar))
            {
                attack();
                return;
            }

            Vector2 dir;
            bool isMoving = false;

            dir = OGetGamePad(0)->getLeftThumb();

            if (OInputPressed(OKeyA) || OGamePadPressed(OGamePadDPadLeft))
            {
                dir -= Vector2::UnitX;
                isMoving = true;
            }
            else if (OInputPressed(OKeyD) || OGamePadPressed(OGamePadDPadRight))
            {
                dir += Vector2::UnitX;
                isMoving = true;
            }
            if (OInputPressed(OKeyW) || OGamePadPressed(OGamePadDPadUp))
            {
                dir -= Vector2::UnitY;
                isMoving = true;
            }
            else if (OInputPressed(OKeyS) || OGamePadPressed(OGamePadDPadDown))
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
