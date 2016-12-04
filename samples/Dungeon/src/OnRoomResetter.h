#ifndef ONROOMRESETTER_H_INCLUDED
#define ONROOMRESETTER_H_INCLUDED

#include "Dungeon.h"

#include <onut/Component.h>

class OnRoomResetter final : public OComponent
{
private:
    void onCreate() override;
    void onMessage(int messageId, void* pData) override;

    Matrix m_resetTransform;
    const Dungeon::Room* m_pRoom = nullptr;
};

#endif
