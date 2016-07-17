#include "Guard.h"

#include <onut/Entity.h>
#include <onut/SceneManager.h>

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
