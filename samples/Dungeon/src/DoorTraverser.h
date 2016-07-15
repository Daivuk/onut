#pragma once
#include <onut/Component.h>
#include <onut/TiledMap.h>

class DoorTraverser final : public OComponent
{
public:
    void onCreate() override;

private:
    OTiledMapRef m_pTiledMap;
    OTiledMap::TileLayer* m_pLayer = nullptr;
};
