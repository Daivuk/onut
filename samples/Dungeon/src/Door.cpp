#include "Door.h"

#include <onut/Entity.h>
#include <onut/Maths.h>

#include <string>

bool Door::getOpen() const
{
    return m_open;
}

void Door::setOpen(bool open)
{
    m_open = open;
}

void Door::onCreate()
{
    Vector2 pos = getEntity()->getLocalTransform().Translation() / 16.0f;
    m_mapPos.x = (int)pos.x;
    m_mapPos.y = (int)pos.y;
}

void Door::onMessage(int messageId, void *pData)
{
}
