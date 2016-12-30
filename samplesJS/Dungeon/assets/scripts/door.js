var doors = [];
var doorsById = {};

function door_init(entity)
{
    
}

function door_openTiles(x, y, sx, sy) 
{
    for (var j = y; j < y + sy; ++j)
    {
        for (var i = x; i < x + sx; ++i) 
        {
            tiledMap.setCollision(i, j, true);
            var tileId = tiledMap.getTileAt("tiles", i, j);
            tiledMap.setTileAt("tiles", i, j, tileId + 2);
        }
    }
}

function door_open(door) 
{
    var target = door.target;

    door_openTiles(door.position.x / 16, door.position.y / 16, door.size.x / 16, door.size.y / 16);
    door_openTiles(target.position.x / 16, target.position.y / 16, target.size.x / 16, target.size.y / 16);

    door.isOpen = true;
    target.isOpen = true;

    playSound("prisonDoorOpen.wav");
}

function door_traverse(entity, door)
{
    var target = door.target;

    if (door.position.x < target.position.x)
    {
        entity.position.x = target.position.x + target.size.x - 2 + entity.size.x / 2;
        entity.position.y = target.position.y + target.size.y / 2;
    }
    else if (door.position.x > target.position.x)
    {
        entity.position.x = target.position.x + 2 - entity.size.x / 2;
        entity.position.y = target.position.y + target.size.y / 2;
    }
    else if (door.position.y < target.position.y) 
    {
        entity.position.y = target.position.y + target.size.y - 2 + entity.size.y / 2;
        entity.position.x = target.position.x + target.size.x / 2;
    }
    else if (door.position.y > target.position.y)
    {
        entity.position.y = target.position.y + 2 - entity.size.y / 2;
        entity.position.x = target.position.x + target.size.x / 2;
    }
}

function door_create(mapObj)
{
    entities.push(mapObj);
}

function doors_init() 
{
    // Parse map entities
    var objCount = tiledMap.getObjectCount("entities");
    for (var i = 0; i < objCount; ++i) 
    {
        var obj = tiledMap.getObject("entities", i);
        if (obj.type == "door")
        { 
            obj.isOpen = false;
            doorsById[obj.id] = obj;
            doors.push(obj);
        }
    }

    // Link doors
    for (var i = 0; i < doors.length; ++i) 
    {
        var target = doorsById[doors[i].properties["Door:Target"]];
        if (target) 
        {
            target.target = doors[i];
            doors[i].target = target;
        }
    }
}
