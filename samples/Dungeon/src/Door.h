#ifndef DOOR_H_INCLUDED
#define DOOR_H_INCLUDED

#include <onut/Component.h>
#include <onut/Point.h>
#include <onut/TiledMap.h>

// Forware declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Collider2DComponent);
OForwardDeclare(Sound);
OForwardDeclare(TiledMapComponent);
ForwardDeclare(Door);

class Door final : public OComponent
{
public:
    bool getOpen() const;
    void setOpen(bool open);

    OSoundRef getOpenSound() const;
    void setOpenSound(const OSoundRef& pSound);

    OSoundRef getCloseSound() const;
    void setCloseSound(const OSoundRef& pSound);

    OEntityRef getTarget() const;
    void setTarget(const OEntityRef& pTarget);

    bool getNeedBomb() const;
    void setNeedBomb(bool needBomb);

    const Point& getMapPos() const;
    Vector2 getExitPosition() const;

protected:
    void onCreate() override;
    void onMessage(int messageId, void* pData) override;

private:
    std::vector<Point> m_mapPositions;
    bool m_open = false;
    Vector2 m_closeSize;
    OCollider2DComponentRef m_pCollider;
    OTiledMapRef m_pTiledMap;
    OTiledMapComponentRef m_pTiledMapComponent;
    OTiledMap::TileLayer* m_pTileLayer;
    OSoundRef m_pOpenSound;
    OSoundRef m_pCloseSound;
    OEntityWeak m_pTargetDoor;
    bool m_needBomb = false;
};

#endif
