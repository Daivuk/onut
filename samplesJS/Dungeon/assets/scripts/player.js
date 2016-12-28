// Constants
var MOV_SPEED = 64;

function player_init()
{
    var ent_player = tiledMap.getObject("entities", "player");
    var player = {
        position: ent_player.size.div(2).add(ent_player.position),
        spriteAnim: playSpriteAnim("baltAnims.spriteanim", "idle_s"),
        dir: "s",
        size: new Vector2(8, 8),
        stepDelay: 0
    }
    return player;
}

function player_update(dt, entity)
{
    // Find the new direction
    var dir = new Vector2(0, 0);

    if (Input.isDown(Key.XARCADE_LJOY_RIGHT) || Input.isDown(Key.D)) dir.x += 1;
    if (Input.isDown(Key.XARCADE_LJOY_LEFT) || Input.isDown(Key.A)) dir.x += -1;
    if (Input.isDown(Key.XARCADE_LJOY_DOWN) || Input.isDown(Key.S)) dir.y += 1;
    if (Input.isDown(Key.XARCADE_LJOY_UP) || Input.isDown(Key.W)) dir.y += -1;

    if (dir.lengthSquared() == 0)
    {
        entity.spriteAnim.play("idle_" + entity.dir);
        return;
    }
    else 
    {
        dir = dir.normalize();
        if (dir.y > .7) entity.dir = "s";
        else if (dir.x > .7) entity.dir = "e";
        else if (dir.x < -.7) entity.dir = "w";
        else entity.dir = "n";
        entity.spriteAnim.play("run_" + entity.dir);
        entity.stepDelay += dt;
        if (entity.stepDelay >= .3) 
        {
            playSound("step.wav", .5);
            entity.stepDelay = 0;
        }
    }

    // move + collisions
    var previousPosition = entity.position;
    var newPosition = entity.position.add(dir.mul(MOV_SPEED * dt));

    // Check if the new position overlap a door, and trigger it
    for (var i = 0; i < doors.length; ++i) 
    {
        var door = doors[i];
        if (door.isOpen) 
        {
            if (newPosition.x + entity.size.x / 2 >= door.position.x + 4 &&
                newPosition.x - entity.size.x / 2 <= door.position.x + door.size.x - 4 &&
                newPosition.y + entity.size.y / 2 >= door.position.y + 4 &&
                newPosition.y - entity.size.y / 2 <= door.position.y + door.size.y - 4) 
            {
                fadeAnim.queue(1, 0.5, Tween.LINEAR, function() 
                {
                    door_traverse(entity, door);
                    var newRoom = room_getAt(Math.floor(entity.position.x / 16), Math.floor(entity.position.y / 16));
                    room_show(newRoom);
                    camera = new Vector2(newRoom.center);
                });
                fadeAnim.queue(0, 0.5);
                fadeAnim.play(false);
                entity.spriteAnim.play("idle_" + entity.dir);
                return;
            }
        }
        else 
        {
            if (newPosition.x + entity.size.x / 2 >= door.position.x &&
                newPosition.x - entity.size.x / 2 <= door.position.x + door.size.x &&
                newPosition.y + entity.size.y / 2 >= door.position.y &&
                newPosition.y - entity.size.y / 2 <= door.position.y + door.size.y) 
            {
                door_open(door);
            }
        }
    }

    entity.position = tiledMap.collision(previousPosition, newPosition, entity.size);
}
