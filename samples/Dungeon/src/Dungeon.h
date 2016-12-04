#ifndef DUNGEON_H_INCLUDED
#define DUNGEON_H_INCLUDED

// Onut includes
#include <onut/Maths.h>
#include <onut/Point.h>

// STL
#include <set>
#include <string>
#include <vector>

// Forward declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Entity)
OForwardDeclare(TiledMap)
ForwardDeclare(Dungeon)

class Dungeon
{
public:
    using TiledId = int;
    using TiledIdSet = std::set<TiledId>;

    struct Room
    {
        iRect bound;
        TiledIdSet tileIdSet;
    };

    Dungeon(const std::string& filename);

    const Room* getRoomAt(const Vector2& position) const;
    const Room* getRoomAt(const Point& tilePos) const;

    bool isInRoom(const Vector2& position, const Room* pRoom) const;
    bool isInRoom(const Point& tilePos, const Room* pRoom) const;

    OEntityRef getPlayer() const;
    OEntityRef getEntity() const;

    void showCoinAt(const Vector2& position, int count);
    void showBombAt(const Vector2& position, int count);

private:
    using Rooms = std::vector<Room>;

    void createMap(const std::string& filename);
    void createRooms();
    Room createRoomAt(const Point& mapPos, const uint32_t* pTiles);

    int m_width;
    int m_height;
    OTiledMapRef m_pTiledMap;
    Rooms m_rooms;
    OEntityWeak m_pPlayer;
    OEntityWeak m_pDungeonEntity;
};

extern DungeonRef g_pDungeon;

#endif
