#include "Dungeon.h"
#include "ItemJump.h"
#include "Player.h"

#include <onut/EntityFactory.h>
#include <onut/SceneManager.h>
#include <onut/Sound.h>
#include <onut/SpriteAnimComponent.h>
#include <onut/TiledMap.h>
#include <onut/TiledMapComponent.h>

DungeonRef g_pDungeon;

Dungeon::Dungeon(const std::string& filename)
{
    // Load the map
    createMap(filename);

    // Find our rooms
    createRooms();

    // Find our player
    m_pPlayer = oSceneManager->findEntity("player");
}

void Dungeon::createMap(const std::string& filename)
{
    // Create our tiled map entity.
    // The TiledMapComponent will look for the "entities" object layer and 
    // instantiate entities and their components based on properties
    // set in Tiled editor.
    auto pTiledMapEntity = OCreateTiledMapEntity(filename);
    auto pTiledMapComponent = pTiledMapEntity->getComponent<OTiledMapComponent>();
    m_pTiledMap = pTiledMapComponent->getTiledMap();

    m_width = m_pTiledMap->getWidth();
    m_height = m_pTiledMap->getHeight();
    m_pDungeonEntity = pTiledMapEntity;
}

const Dungeon::Room* Dungeon::getRoomAt(const Vector2& position) const
{
    return getRoomAt(Point((int)(position.x / 16.0f), (int)(position.y / 16.0f)));
}

const Dungeon::Room* Dungeon::getRoomAt(const Point& tilePos) const
{
    for (auto& room : m_rooms)
    {
        if (isInRoom(tilePos, &room))
        {
            return &room;
        }
    }
    return nullptr;
}

bool Dungeon::isInRoom(const Vector2& position, const Room* pRoom) const
{
    return isInRoom(Point((int)(position.x / 16.0f), (int)(position.y / 16.0f)), pRoom);
}

bool Dungeon::isInRoom(const Point& tilePos, const Room* pRoom) const
{
    if (tilePos.x < pRoom->bound.left || tilePos.x > pRoom->bound.right ||
        tilePos.y < pRoom->bound.top || tilePos.y > pRoom->bound.bottom) return false;
    int k = tilePos.y * m_width + tilePos.x;
    return pRoom->tileIdSet.count(k) == 1;
}

void Dungeon::createRooms()
{
    auto pRoomLayer = dynamic_cast<OTiledMap::TileLayer*>(m_pTiledMap->getLayer("room"));
    auto roomTiles = pRoomLayer->tileIds;
    auto w = pRoomLayer->width;
    auto h = pRoomLayer->height;
    int x, y, k;

    for (y = 0; y < h; ++y)
    {
        for (x = 0; x < w; ++x)
        {
            k = y * w + x;

            // If this is not a room tile, continue.
            if (!roomTiles[k]) continue;

            // Check to see if this tile is not already part of a room
            bool hasRoom = false;
            for (auto& room : m_rooms)
            {
                if (isInRoom(Point(x, y), &room))
                {
                    hasRoom = true;
                    break;
                }
            }
            if (hasRoom) continue;

            // Create the new room
            m_rooms.push_back(createRoomAt(Point(x, y), roomTiles));
        }
    }
}

Dungeon::Room Dungeon::createRoomAt(const Point& mapPos, const uint32_t* pTiles)
{
    using PaintQueue = std::vector<Point>;

    Room room;
    Point minBound = mapPos;
    Point maxBound = mapPos;
    PaintQueue paintQueue;
    int k;
    int w = m_width;
    int h = m_height;
    auto pRoomLayer = dynamic_cast<OTiledMap::TileLayer*>(m_pTiledMap->getLayer("room"));
    auto roomTiles = pRoomLayer->tileIds;

    paintQueue.push_back(mapPos);
    while (!paintQueue.empty())
    {
        auto& point = paintQueue.back();

        minBound.x = onut::min(minBound.x, point.x);
        minBound.y = onut::min(minBound.y, point.y);
        maxBound.x = onut::max(maxBound.x, point.x);
        maxBound.y = onut::max(maxBound.y, point.y);

        k = (point.y) * w + (point.x - 1);
        if (point.x > 0 && !room.tileIdSet.count(k))
        {
            auto tileId = pTiles[k];
            if (tileId)
            {
                room.tileIdSet.insert(k);
                paintQueue.push_back({point.x - 1, point.y});
                continue;
            }
        }

        k = (point.y) * w + (point.x + 1);
        if (point.x < w - 1 && !room.tileIdSet.count(k))
        {
            auto tileId = pTiles[k];
            if (tileId)
            {
                room.tileIdSet.insert(k);
                paintQueue.push_back({point.x + 1, point.y});
                continue;
            }
        }

        k = (point.y - 1) * w + (point.x);
        if (point.y > 0 && !room.tileIdSet.count(k))
        {
            auto tileId = pTiles[k];
            if (tileId)
            {
                room.tileIdSet.insert(k);
                paintQueue.push_back({point.x, point.y - 1});
                continue;
            }
        }

        k = (point.y + 1) * w + (point.x);
        if (point.y < h - 1 && !room.tileIdSet.count(k))
        {
            auto tileId = pTiles[k];
            if (tileId)
            {
                room.tileIdSet.insert(k);
                paintQueue.push_back({point.x, point.y + 1});
                continue;
            }
        }

        paintQueue.pop_back();
    }

    room.bound.left = minBound.x;
    room.bound.top = minBound.y;
    room.bound.right = maxBound.x;
    room.bound.bottom = maxBound.y;

    return std::move(room);
}

OEntityRef Dungeon::getPlayer() const
{
    return m_pPlayer.lock();
}

OEntityRef Dungeon::getEntity() const
{
    return m_pDungeonEntity.lock();
}

void Dungeon::showCoinAt(const Vector2& position, int count)
{
    OPlaySound("gold.wav");

    auto pDungeon = m_pDungeonEntity.lock();
    if (pDungeon)
    {
        auto pPlayer = getPlayer();
        if (pPlayer)
        {
            auto pPlayerComponent = pPlayer->getComponent<Player>();
            pPlayerComponent->setCoinCount(pPlayerComponent->getCoinCount() + count);
        }
        for (int i = 0; i < count; ++i)
        {
            auto pCoinEntity = OCreateSpriteAnimEntity("coin.spriteanim", position, "spin");
            pCoinEntity->addComponent<ItemJump>();
            pDungeon->add(pCoinEntity);
        }
    }
}

void Dungeon::showBombAt(const Vector2& position, int count)
{
    OPlaySound("hit.wav");

    auto pDungeon = m_pDungeonEntity.lock();
    if (pDungeon)
    {
        auto pPlayer = getPlayer();
        if (pPlayer)
        {
            auto pPlayerComponent = pPlayer->getComponent<Player>();
            pPlayerComponent->setBombCount(pPlayerComponent->getBombCount() + count);
        }
        for (int i = 0; i < count; ++i)
        {
            auto pCoinEntity = OCreateSpriteAnimEntity("bomb.spriteanim", position, "idle");
            pCoinEntity->addComponent<ItemJump>();
            pDungeon->add(pCoinEntity);
        }
    }
}
