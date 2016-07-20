#include "DamageFlasher.h"
#include "Defines.h"
#include "Guard.h"

#include <onut/Entity.h>
#include <onut/SceneManager.h>
#include <onut/Sound.h>

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
}

void Guard::onMessage(int messageId, void* pData)
{
    if (messageId == Messages::Reset)
    {
        m_dir = m_resetDir;
    }
}

void Guard::onUpdate()
{
}
