function door_init(entity)
{
    entity.isOpen = false;

    // Link doors
    var targetId = entity.properties["Door:Target"];
    if (targetId)
    {
        for (var i = 0; i < entities.length; ++i) 
        {
            var other = entities[i];
            if (other.id == targetId) 
            {
                entity.target = other;
                other.target = entity;
            }
        }
    }

    // Touch callback
    entity.touchFn = door_touch;
}

function door_openTiles(door, x, y, sx, sy) 
{
    var w = tiledMap.getSize().x;
    for (var j = y; j < y + sy; ++j)
    {
        for (var i = x; i < x + sx; ++i) 
        {
            tiledMap.setPassable(i, j, true);
            var tileId = tiledMap.getTileAt("tiles", i, j);
            tiledMap.setTileAt("tiles", i, j, tileId + 2);
        }
    }
}

function door_open(door) 
{
    var target = door.target;

    door_openTiles(door, (door.position.x - door.size.x) / 16, (door.position.y - door.size.y) / 16, door.size.x / 8, door.size.y / 8);
    door_openTiles(target, (target.position.x - target.size.x) / 16, (target.position.y - target.size.y) / 16, target.size.x / 8, target.size.y / 8);

    door.isOpen = true;
    target.isOpen = true;

    door.size = door.size.sub(5);
    target.size = target.size.sub(5);

    playSound("prisonDoorOpen.wav");
}

function door_traverse(entity, door)
{
    var target = door.target;
    if (door.position.x < target.position.x)
    {
        entity.position.x = target.position.x + target.size.x + entity.size.x + 2;
        entity.position.y = target.position.y;
    }
    else if (door.position.x > target.position.x)
    {
        entity.position.x = target.position.x - target.size.x - entity.size.x - 2;
        entity.position.y = target.position.y;
    }
    else if (door.position.y < target.position.y) 
    {
        entity.position.y = target.position.y + target.size.y + entity.size.y + 2;
        entity.position.x = target.position.x;
    }
    else if (door.position.y > target.position.y)
    {
        entity.position.y = target.position.y - target.size.y - entity.size.y - 2;
        entity.position.x = target.position.x;
    }
    entity.room = target.room;
    if (entity == player)
    {
        room_show(entity.room);
    }
}

function door_touch(door, fromEntity)
{
    if (fromEntity != player) return; // Only player can open/traverse doors

    if (door.isOpen) 
    {
        fromEntity.spriteAnim.play("idle_" + fromEntity.dir);
        fadeAnim.queue(1, 0.5, Tween.LINEAR, function() 
        {
            // Notify room leave
            for (var i = 0; i < entities.length; ++i)
            {
                var other = entities[i];
                if (other.roomLeaveFn) other.roomLeaveFn(other, fromEntity.room);
            }

            // Traverse door
            door_traverse(fromEntity, door);

            // Notify room enter
            for (var i = 0; i < entities.length; ++i)
            {
                var other = entities[i];
                if (other.roomEnterFn) other.roomEnterFn(other, fromEntity.room);
            }
        });
        fadeAnim.queue(0, 0.5);
        fadeAnim.play();
        return;
    }
    else 
    {
        door_open(door);
    }
}
