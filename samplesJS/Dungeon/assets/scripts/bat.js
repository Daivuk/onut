var BAT_MOV_SPEED = 24;

function bat_init(entity)
{
    // Bat is smaller
    entity.size = new Vector2(4, 4);

    // Misc
    entity.isAttacking = false;
    entity.life = 2;
    entity.inventory = {};
    entity.inventory.bomb = 0;
    entity.inventory.gold = 0;
    entity.startPosition = entity.position;
    entity.hitFlashAnim = new ColorAnim(Color.WHITE);
    entity.pushBackVel = Vector2.ZERO;
    entity.attackCoolDown = false;

    // Anims
    entity.spriteAnim = playSpriteAnim(entity.properties["SpriteAnimComponent:SpriteAnim"], entity.properties["SpriteAnimComponent:DefaultAnim"]),

    // callbacks
    entity.updateFn = null;
    entity.drawFn = null;
    entity.roomEnterFn = bat_roomEnter;
    entity.roomLeaveFn = null;
    entity.damageFn = null;

    updateEntities.push(entity);
    drawEntities.push(entity);
    damageEntities.push(entity);
}

function bat_draw(entity)
{
    SpriteBatch.drawSpriteAnim(entity.spriteAnim, entity.position, entity.hitFlashAnim.get());
}

function bat_damage(entity, fromEntity, amount)
{
    if (entity.hitFlashAnim.isPlaying()) return; // Can't hit while in flashing mode
    entity.life = Math.max(0, entity.life - amount);
    playSound("flesh.wav");
    if (entity.life == 0)
    {
        // Ded
        entity.spriteAnim.play("die");
        entity.damageFn = null;
        entity.updateFn = null;

        // We never kill the entity, we leave the body on the ground
    }
    else
    {
        var flashColor = new Color(1, 1, 1, 0);
        entity.hitFlashAnim.playKeyFrames(
            flashColor, [
                {value:Color.WHITE, duration:.1, tween:Tween.NONE},
                {value:flashColor, duration:.1, tween:Tween.NONE},
                {value:Color.WHITE, duration:.1, tween:Tween.NONE},
                {value:flashColor, duration:.1, tween:Tween.NONE},
                {value:Color.WHITE, duration:.1, tween:Tween.NONE},
                {value:flashColor, duration:.1, tween:Tween.NONE},
                {value:Color.WHITE, duration:.1, tween:Tween.NONE},
            ]);
        entity.pushBackVel = entity.position.sub(fromEntity.position);
        entity.pushBackVel = entity.pushBackVel.normalize();
        entity.pushBackVel = entity.pushBackVel.mul(128);
        setTimeout(function()
        {
            entity.pushBackVel = Vector2.ZERO;
        }, 200);
    }
}

function bat_roomEnter(entity, room)
{
    if (entity.room == room)
    {
        if (entity.life > 0)
        {
            entity.updateFn = bat_update;
            entity.damageFn = bat_damage;
            entity.position = entity.startPosition;
            playSound("bat_wake.wav");
        }
        entity.roomEnterFn = null;
        entity.roomLeaveFn = bat_roomLeave;
        entity.drawFn = bat_draw;
    }
}

function bat_roomLeave(entity, room)
{
    if (entity.room == room)
    {
        entity.updateFn = null;
        entity.drawFn = null;
        entity.roomEnterFn = bat_roomEnter;
        entity.roomLeaveFn = null;
        entity.damageFn = null;
    }
}

function bat_update(entity, dt)
{
    var previousPosition = entity.position;
    var newPosition = entity.position.add(entity.pushBackVel.mul(dt));

    var dis = Vector2.distanceSquared(player.position, entity.position);
    var closeEnough = dis <= 8 * 8;

    if (!entity.isAttacking && !closeEnough)
    {
        // Bats just fly straigth line
        dir = player.position.sub(entity.position);
        dir = dir.normalize();
        newPosition = newPosition.add(dir.mul(BAT_MOV_SPEED * dt));
    }

    entity.position = tiledMap.collision(previousPosition, newPosition, entity.size);

    // If close enough to the player, start an attack
    if (!entity.isAttacking && !entity.attackCoolDown && closeEnough)
    {
        // Pick attack direction
        var dir = player.position.sub(entity.position);
        dir = dir.normalize();

        entity.isAttacking = true;
        entity.attackCoolDown = true;
        entity.spriteAnim.play("attack");
        playSound("bat_attack.wav");
        setTimeout(function() {entity.isAttacking = false;}, 250);
        setTimeout(function() {entity.attackCoolDown = false;}, 1000);

        // Damage area around us
        radiusDamage(entity, entity.position, 8, 1);
    }
}

