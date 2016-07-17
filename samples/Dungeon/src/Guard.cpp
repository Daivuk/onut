#include "DamageFlasher.h"
#include "Defines.h"
#include "Guard.h"

#include <onut/Entity.h>
#include <onut/SceneManager.h>
#include <onut/Sound.h>

int Guard::getLife() const
{
    return m_life;
}

void Guard::setLife(int life)
{
    m_life = life;
}

void Guard::onCreate()
{
    m_pPlayer = getSceneManager()->findEntity("player");
}

void Guard::onMessage(int messageId, void* pData)
{
    if (messageId == MESSAGE_DAMAGE)
    {
        OPlaySound("flesh.wav", .5f);
    }
}
