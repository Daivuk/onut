#ifndef SMARTROOMCAMERA_H_INCLUDED
#define SMARTROOMCAMERA_H_INCLUDED

#include "Dungeon.h"

#include <onut/Component.h>

#include <onut/ForwardDeclaration.h>
OForwardDeclare(Entity);
OForwardDeclare(TiledMap);
ForwardDeclare(SmartRoomCamera);

class SmartRoomCamera final : public OComponent
{
public:
    SmartRoomCamera();

    void onCreate() override;
    void onUpdate() override;

private:
    void RefreshFogOfWar();

    OTiledMapRef m_pTiledMap;
    std::vector<int> m_paint;
    int m_nextPaint = 1;
    std::vector<Point> m_paintQueue;
    const Dungeon::Room* m_pRoom = nullptr;
};

#endif
