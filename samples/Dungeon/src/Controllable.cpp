#include "Controllable.h"

#include <onut/Entity.h>
#include <onut/GamePad.h>
#include <onut/Input.h>
#include <onut/SpriteAnimComponent.h>
#include <onut/Timing.h>

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
    m_pSpriteAnimComponent = getEntity()->getComponent<OSpriteAnimComponent>();
}

void Controllable::onUpdate()
{
    Vector2 dir;
    bool isMoving = false;

    if (OInputPressed(OKeyA) || OGamePadPressed(OGamePadLeftThumbLeft))
    {
        dir -= Vector2::UnitX;
        isMoving = true;
    }
    else if (OInputPressed(OKeyD) || OGamePadPressed(OGamePadLeftThumbRight))
    {
        dir += Vector2::UnitX;
        isMoving = true;
    }
    if (OInputPressed(OKeyW) || OGamePadPressed(OGamePadLeftThumbUp))
    {
        dir -= Vector2::UnitY;
        isMoving = true;
    }
    else if (OInputPressed(OKeyS) || OGamePadPressed(OGamePadLeftThumbDown))
    {
        dir += Vector2::UnitY;
        isMoving = true;
    }
    if (!isMoving)
    {
        if (m_pSpriteAnimComponent) m_pSpriteAnimComponent->play("idle_" + m_dir);
        return;
    }

    dir.Normalize();

    if (dir.y > .7f) m_dir = "s";
    else if (dir.x < -.7f) m_dir = "w";
    else if (dir.x > .7f) m_dir = "e";
    else if (dir.y < -.7f) m_dir = "n";

    if (m_pSpriteAnimComponent) m_pSpriteAnimComponent->play("run_" + m_dir);

    auto& pEntity = getEntity();
    Vector2 position = pEntity->getLocalTransform().Translation();
    position += dir * m_speed * ODT;
    pEntity->setLocalTransform(Matrix::CreateTranslation(position));
}
