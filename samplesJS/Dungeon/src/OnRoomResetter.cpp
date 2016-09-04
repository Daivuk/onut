#include "Defines.h"
#include "OnRoomResetter.h"

#include <onut/Entity.h>
#include <onut/Collider2DComponent.h>
#include <onut/Timing.h>

void OnRoomResetter::onCreate()
{
    m_resetTransform = getWorldTransform();
    m_pRoom = g_pDungeon->getRoomAt(m_resetTransform.Translation());
    getEntity()->setEnabled(false);
    getEntity()->setVisible(false);
}

void OnRoomResetter::onMessage(int messageId, void* pData)
{
    if (messageId == Messages::EnterRoom)
    {
        if (pData == m_pRoom) // Player enters our room
        {
            // Reset his world position
            setWorldTransform(m_resetTransform);

            // If it's a collider, make sure we teleport his physic body
            auto pCollider = getComponent<OCollider2DComponent>();
            if (pCollider)
            {
                pCollider->teleport(m_resetTransform.Translation());
            }

            // Send the message to the rest of its components that we want to reset their state
            sendMessage(Messages::Reset);

            // Enable/Show self
            getEntity()->setEnabled(true);
            getEntity()->setVisible(true);
        }
    }
    else if (messageId == Messages::LeaveRoom)
    {
        if (pData == m_pRoom) // Player leaves our room
        {
            // Disable/Hide self
            getEntity()->setEnabled(false);
            getEntity()->setVisible(false);
        }
    }
}
