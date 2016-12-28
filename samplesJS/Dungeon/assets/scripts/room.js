var rooms = [];

function room_getAt(x, y)
{
    var tileNum = y * tiledMap.getSize().x + x;
    for (var i = 0; i < rooms.length; ++i)
    {
        if (tileNum in rooms[i].tiles)
        {
            return rooms[i];
        }
    }
    return null;
}

function room_createAt(x, y)
{
    var room = {
        tiles: {},
        tilesToReveal: {},
        topLeft: new Vector2(1000, 1000),
        bottomRight: new Vector2(0, 0),
        center: new Vector2()
    };
    var w = tiledMap.getSize().x;
    var toCheck = {};
    toCheck[y * w + x] = true;
    var roomLayer = tiledMap.getLayerIndex("room");

    while (Object.keys(toCheck).length)
    {
        var toCheckValue = Object.keys(toCheck)[0];
        x = toCheckValue % w;
        y = Math.floor(toCheckValue / w);
        if (tiledMap.getTileAt(roomLayer, x + 1, y) != 0 && !((y * w + x + 1) in room.tiles)) toCheck[y * w + x + 1] = true;
        if (tiledMap.getTileAt(roomLayer, x - 1, y) != 0 && !((y * w + x - 1) in room.tiles)) toCheck[y * w + x - 1] = true;
        if (tiledMap.getTileAt(roomLayer, x, y + 1) != 0 && !(((y + 1) * w + x) in room.tiles)) toCheck[(y + 1) * w + x] = true;
        if (tiledMap.getTileAt(roomLayer, x, y - 1) != 0 && !(((y - 1) * w + x) in room.tiles)) toCheck[(y - 1) * w + x] = true;
        room.tiles[toCheckValue] = true;
        for (var j = y - 2; j <= y + 2; ++j)
        {
            for (var i = x - 2; i <= x + 2; ++i)
            {
                room.tilesToReveal[j * w + i] = true;
            }
        }
        room.topLeft.x = Math.min(room.topLeft.x, x);
        room.topLeft.y = Math.min(room.topLeft.y, y);
        room.bottomRight.x = Math.max(room.bottomRight.x, x);
        room.bottomRight.y = Math.max(room.bottomRight.y, y);
        delete toCheck[toCheckValue];
    }

    room.center = room.topLeft.add(room.bottomRight.add(1)).mul(8);

    return room;
}

function room_init()
{
    var roomLayer = tiledMap.getLayerIndex("room");
    var mapSize = tiledMap.getSize();
    for (var y = 0; y < mapSize.y; ++y)
    {
        for (var x = 0; x < mapSize.x; ++x)
        {
            var tileId = tiledMap.getTileAt(roomLayer, x, y);
            if (tileId == 0) continue; // Not a room tile

            // Check if that tile is not already in a room
            if (room_getAt(x, y)) continue;

            rooms.push(room_createAt(x, y));
        }
    }
}

function room_show(room)
{
    var fowLayer = tiledMap.getLayerIndex("fow");
    var mapSize = tiledMap.getSize();

    // Put all dark first
    for (var y = 0; y < mapSize.y; ++y)
    {
        for (var x = 0; x < mapSize.x; ++x)
        {
            tiledMap.setTileAt(fowLayer, x, y, 21);
        }
    }

    // Turn on this room's tiles
    tiles = Object.keys(room.tilesToReveal);
    for (var i = 0; i < tiles.length; ++i)
    {
        var x = tiles[i] % mapSize.x;
        var y = Math.floor(tiles[i] / mapSize.x);
        tiledMap.setTileAt(fowLayer, x, y, 0);
    }
}
