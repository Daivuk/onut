#include "Door.h"
#include "DoorTraverser.h"

#include <onut/Collider2DComponent.h>
#include <onut/Entity.h>
#include <onut/SceneManager.h>
#include <onut/Sound.h>
#include <onut/TiledMapComponent.h>

extern OAnimFloat g_fadeAnim;

void DoorTraverser::onCreate()
{
    m_pTiledMapComponent = getParentComponent<OTiledMapComponent>();
}

void DoorTraverser::onTriggerEnter(const OCollider2DComponentRef& pCollider)
{
    auto pDoor = pCollider->getComponent<Door>();
    if (pDoor)
    {
        if (pDoor->getOpen())
        {
            // Traverse
            auto pTarget = pDoor->getTarget();
            if (pTarget)
            {
                auto pTargetDoor = pTarget->getComponent<Door>();
                auto pCollider = getComponent<OCollider2DComponent>();
                if (pTargetDoor)
                {
                    Vector2 targetPos = pTargetDoor->getExitPosition();

                    auto pEntity = getEntity();
                    auto pSceneManager = pEntity->getSceneManager();

                    // Pause, fade out then teleport
                    pSceneManager->setPause(true);
                    g_fadeAnim.queue(0.0f, .5f, OTweenLinear, 
                                     [pEntity, pSceneManager, pCollider, targetPos]
                    {
                        if (pCollider)
                        {
                            pCollider->teleport(targetPos);
                        }
                        else
                        {
                            pEntity->setLocalTransform(Matrix::CreateTranslation(targetPos));
                        }
                        pSceneManager->setPause(false);
                        pSceneManager->update();
                        pSceneManager->setPause(true);
                    });

                    // Fade in then unpause
                    g_fadeAnim.queue(1.0f, .5f, OTweenLinear, 
                                     [pSceneManager]
                    {
                        pSceneManager->setPause(false);
                    });

                    g_fadeAnim.play();
                }
            }
        }
        else
        {
            // Open the door, and it's target
            if (!pDoor->getOpen() && !pDoor->getNeedBomb())
            {
                pDoor->setOpen(true);

                auto pSound = pDoor->getOpenSound();
                if (pSound) pSound->play();

                auto pTarget = pDoor->getTarget();
                if (pTarget)
                {
                    auto pTargetDoor = pTarget->getComponent<Door>();
                    if (pTargetDoor)
                    {
                        pTargetDoor->setOpen(true);
                    }
                }
            }
        }
    }
}
