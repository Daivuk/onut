#include "Bomb.h"

#include <onut/Collider2DComponent.h>
#include <onut/Curve.h>
#include <onut/Entity.h>
#include <onut/EntityFactory.h>
#include <onut/Sound.h>
#include <onut/SpriteAnim.h>
#include <onut/SpriteAnimComponent.h>
#include <onut/Timing.h>

static const float DURATION = 4.0f;

Bomb::Bomb()
    : OComponent(FLAG_UPDATABLE)
{
}

void Bomb::onCreate()
{
    m_pSpriteAnim = getComponent<OSpriteAnimComponent>()->getSpriteAnim();
}

void Bomb::onUpdate()
{
    if (m_dieNextFrame)
    {
        destroy();
        return;
    }

    float percent = m_progress / DURATION;
    float fps = OLerp(4.0f, 12.0f, percent);
    getComponent<OSpriteAnimComponent>()->setFPS(fps);

    m_progress += ODT;
    if (m_progress >= DURATION)
    {
        // Play sound
        OPlaySound("explosion.wav");

        // Spawn explosion sprite
        auto pExplosion = OCreateSpriteAnimEntity("explosion.spriteanim", getLocalTransform().Translation(), "explosion");
        auto pParent = getEntity()->getParent();
        if (pParent) pParent->add(pExplosion);

        // Toggle collider, it would pick it up for the last frame before dying
        getComponent<OCollider2DComponent>()->setEnabled(true);

        // Destroy the next frame. So our collider can trigger collisions for one last frame.
        m_dieNextFrame = true;
    }
}
