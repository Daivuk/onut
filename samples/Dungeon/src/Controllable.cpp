#include "Controllable.h"

#include <onut/Collider2DComponent.h>
#include <onut/Entity.h>
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
}

const OSoundRef& Controllable::getStepSound() const
{
    return m_pStepSound;
}

void Controllable::setStepSound(const OSoundRef& pSound)
{
    m_pStepSound = pSound;
}

void Controllable::onUpdate()
{
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
        m_stepDelay = 0.15f;
        m_pCollider2DComponent->setVelocity(Vector2::Zero);
        if (m_pSpriteAnimComponent) m_pSpriteAnimComponent->play("idle_" + m_dir);
        return;
    }

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
            m_pStepSound->play(0.8f, 0.0f, ORandFloat(.9f, 1.1f));
        }
    }

    Vector2 position = getLocalTransform().Translation();
    m_pCollider2DComponent->setVelocity(dir * m_speed);
}
