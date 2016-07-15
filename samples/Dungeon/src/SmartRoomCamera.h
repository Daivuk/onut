#pragma once
#include <onut/Component.h>

#include <onut/ForwardDeclaration.h>
OForwardDeclare(Entity);
OForwardDeclare(TiledMap);
ForwardDeclare(SmartRoomCamera);

class SmartRoomCamera final : public OComponent
{
public:
    void onCreate() override;
    void onUpdate() override;

private:
    OEntityRef m_pPlayer;
    OTiledMapRef m_pTiledMap;
    std::vector<int> m_paint;
    int m_nextPaint = 1;
    std::vector<Point> m_paintQueue;
};
