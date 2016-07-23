#include "Dungeon.h"
#include "Exit.h"

#include <onut/Anim.h>
#include <onut/Collider2DComponent.h>
#include <onut/onut.h>
#include <onut/SceneManager.h>

extern OAnimFloat g_fadeAnim;

void Exit::onTriggerEnter(const OCollider2DComponentRef& pCollider)
{
    if (pCollider->getEntity() == g_pDungeon->getPlayer())
    {
        getSceneManager()->setPause(true);

        g_fadeAnim.playFromCurrent(0.0f, 2.0f, OTweenLinear, ODontLoop, []{ OQuit(); });
    }
}
