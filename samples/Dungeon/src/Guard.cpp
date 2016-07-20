#include "DamageFlasher.h"
#include "Defines.h"
#include "Guard.h"

#include <onut/Collider2DComponent.h>
#include <onut/Entity.h>
#include <onut/SceneManager.h>
#include <onut/Sound.h>
#include <onut/SpriteAnimComponent.h>

static const float ATTACK_DIST = 24.0f;
static const float MOVE_SPEED = 32.0f;

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
    m_pPlayer = getSceneManager()->findEntity("player");
    m_resetDir = m_dir;

    m_pCollider = getComponent<OCollider2DComponent>();
    m_pSpriteAnimComponent = getComponent<OSpriteAnimComponent>();
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

void Guard::onUpdate()
{
    // Move toward player
    auto pPlayer = m_pPlayer.lock();
    if (pPlayer)
    {
        Vector2 myPosition = getWorldTransform().Translation();
        Vector2 playerPosition = pPlayer->getWorldTransform().Translation();

        auto dir = playerPosition - myPosition;
        if (dir.LengthSquared() > ATTACK_DIST * ATTACK_DIST)
        {
            dir.Normalize();

            if (dir.y > .7f) m_dir = "s";
            else if (dir.x < -.7f) m_dir = "w";
            else if (dir.x > .7f) m_dir = "e";
            else if (dir.y < -.7f) m_dir = "n";

            m_pCollider->setVelocity(m_pCollider->getVelocity() + dir * MOVE_SPEED);
        }
    }
    
    // Update animation
    if (m_pSpriteAnimComponent)
    {
        m_pSpriteAnimComponent->play("run_" + m_dir);
    }
}
