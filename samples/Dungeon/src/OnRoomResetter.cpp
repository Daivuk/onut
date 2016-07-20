#include "Defines.h"
#include "OnRoomResetter.h"

#include <onut/Entity.h>
#include <onut/Collider2DComponent.h>
#include <onut/Timing.h>

void OnRoomResetter::onCreate()
{
    m_resetTransform = getWorldTransform();
}

void OnRoomResetter::onMessage(int messageId, void* pData)
{
    if (messageId == Messages::EnterRoom)
    {
        setWorldTransform(m_resetTransform);
        auto pCollider = getComponent<OCollider2DComponent>();
        if (pCollider)
        {
            pCollider->teleport(m_resetTransform.Translation());
        }
        sendMessage(Messages::Reset);
    }
}
