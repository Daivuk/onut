// Constants
var MOV_SPEED = 64;

function player_init(entity)
{
    // Center the player on the tile
    entity.position = entity.size.div(2).add(entity.position);

    // Force the size on th eplayer
    entity.size = new Vector2(8, 8);

    // Anims
    entity.spriteAnim = playSpriteAnim("baltAnims.spriteanim", "idle_s"),
    entity.attackAnim = createSpriteAnimInstance("swordAttack.spriteanim"),

    // Misc
    entity.dir = "s";
    entity.stepDelay = 0;
    entity.isAttacking = false;

    // callbacks
    entity.updateFn = player_update;
    entity.drawFn = entity_draw;
    entity.drawOverlayFn = player_drawOverlay;

    player = entity;
}

function player_doneAttacking(entity)
{
    entity.isAttacking = false;
}

function player_updateControls(dt, entity)
{
    if (Input.isJustDown(Key.XARCADE_LBUTTON_1) || Input.isJustDown(Key.SPACE_BAR))
    {
        entity.isAttacking = true;
        entity.attackAnim.play("attack");
        entity.spriteAnim.play("attack_" + entity.dir);
        setTimeout(function() {entity.isAttacking = false}, 250);
        return;
    }

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
                    currentRoom = room_getAt(Math.floor(entity.position.x / 16), Math.floor(entity.position.y / 16));
                    room_show(currentRoom);
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

function player_update(dt, entity)
{
    if (entity.isAttacking)
    {
        // Do a damage radius
    }
    else
    {
        player_updateControls(dt, entity);
    }
}

function player_drawOverlay(entity)
{
    if (!entity.attackAnim.isPlaying()) return;
    if (entity.dir == "e") SpriteBatch.drawSpriteAnim(entity.attackAnim, entity.position);
    else if (entity.dir == "s") SpriteBatch.drawSpriteAnim(entity.attackAnim, entity.position, Color.WHITE, 90);
    else if (entity.dir == "w") SpriteBatch.drawSpriteAnim(entity.attackAnim, entity.position.add(new Vector2(0, -4)), Color.WHITE, 180);
    else if (entity.dir == "n") SpriteBatch.drawSpriteAnim(entity.attackAnim, entity.position.add(new Vector2(0, -2)), Color.WHITE, -90);
}
