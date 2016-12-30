// Constants
var PLAYER_MOV_SPEED = 64;
var PLAYER_DAMAGE = 1;
var PLAYER_DAMAGE_RADIUS = 8;
var PLAYER_DAMAGE_OFFSET = 8;

function player_init(entity)
{
    // Player is smaller
    entity.size = new Vector2(4, 4);

    // Anims
    entity.spriteAnim = playSpriteAnim("baltAnims.spriteanim", "idle_s"),
    entity.attackAnim = createSpriteAnimInstance("swordAttack.spriteanim"),

    // Misc
    entity.dir = "s";
    entity.stepDelay = 0;
    entity.isAttacking = false;
    entity.life = 3;
    entity.bomb = 0;
    entity.gold = 0;

    // callbacks
    entity.updateFn = player_update;
    entity.drawFn = entity_draw;
    entity.drawOverlayFn = player_drawOverlay;
    entity.roomLeaveFn = player_roomLeave;

    // So we can touch things
    entity.isTrigger = true;

    player = entity;
}

function player_roomLeave(entity)
{
    entity.spriteAnim.play("idle_" + entity.dir);
}

function player_doneAttacking(entity)
{
    entity.isAttacking = false;
}

function player_updateControls(entity, dt)
{
    if (Input.isJustDown(Key.XARCADE_LBUTTON_1) || Input.isJustDown(Key.SPACE_BAR))
    {
        entity.isAttacking = true;
        entity.attackAnim.play("attack");
        entity.spriteAnim.play("attack_" + entity.dir);
        playSound("swoosh.wav");
        setTimeout(function() {player_doneAttacking(entity)}, 250);

        // Damage area in front of us
        if (entity.dir == "e") radiusDamage(Entity, entity.position.add(new Vector2(PLAYER_DAMAGE_OFFSET, -4)), PLAYER_DAMAGE_RADIUS, PLAYER_DAMAGE);
        else if (entity.dir == "s") radiusDamage(entity, entity.position.add(new Vector2(0, PLAYER_DAMAGE_OFFSET)), PLAYER_DAMAGE_RADIUS, PLAYER_DAMAGE);
        else if (entity.dir == "w") radiusDamage(Entity, entity.position.add(new Vector2(-PLAYER_DAMAGE_OFFSET, -4)), PLAYER_DAMAGE_RADIUS, PLAYER_DAMAGE);
        else if (entity.dir == "n") radiusDamage(entity, entity.position.add(new Vector2(0, -PLAYER_DAMAGE_OFFSET - 2)), PLAYER_DAMAGE_RADIUS, PLAYER_DAMAGE);
        return;
    }

    if (Input.isJustDown(Key.XARCADE_LBUTTON_2) || Input.isJustDown(Key.B))
    {
        if (entity.bomb > 0)
        {
            --entity.bomb;
            spawnBomb(entity.position.add(new Vector2(0, 1)));
        }
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
    var newPosition = entity.position.add(dir.mul(PLAYER_MOV_SPEED * dt));

    entity.position = tiledMap.collision(previousPosition, newPosition, entity.size);
}

function player_update(entity, dt)
{
    if (!entity.isAttacking)
    {
        player_updateControls(entity, dt);
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
