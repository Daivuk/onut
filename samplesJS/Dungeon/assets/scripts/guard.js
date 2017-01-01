var GUARD_MOV_SPEED = 32;

function guard_init(entity)
{
    // Guard is smaller
    entity.size = new Vector2(4, 4);

    // Misc
    entity.dir = entity.properties["Guard:Dir"];
    entity.isAttacking = false;
    entity.life = 3;
    entity.inventory = {};
    entity.inventory.bomb = 0;
    entity.inventory.gold = 0;
    entity.startPosition = entity.position;
    entity.startDir = entity.dir;
    entity.hitFlashAnim = new ColorAnim(Color.WHITE);
    entity.pushBackVel = Vector2.ZERO;
    entity.attackCoolDown = false;

    // Anims
    entity.spriteAnim = playSpriteAnim(entity.properties["SpriteAnimComponent:SpriteAnim"], "idle_" + entity.dir),
    entity.attackAnim = createSpriteAnimInstance("swordAttack.spriteanim"),

    // callbacks
    entity.updateFn = null;
    entity.drawFn = null;
    entity.drawOverlayFn = null;
    entity.roomEnterFn = guard_roomEnter;
    entity.roomLeaveFn = null;
    entity.damageFn = null;

    updateEntities.push(entity);
    drawEntities.push(entity);
    drawOverlayEntities.push(entity);
    damageEntities.push(entity);
}

function guard_damage(entity, fromEntity, amount)
{
    if (entity.hitFlashAnim.isPlaying()) return; // Can't hit while in flashing mode
    entity.life = Math.max(0, entity.life - amount);
    if (entity.life == 0)
    {
        // Ded
        entity.spriteAnim = playSpriteAnim("puff.spriteanim", "puff");
        entity.drawFn = null;
        entity.roomEnterFn = null;
        entity.roomLeaveFn = null;
        entity.damageFn = null;
        entity.updateFn = null;
            
        playSound("puff.wav");

        setTimeout(function()
        {
            entity_kill(entity);
        }, 1000);
    }
    else
    {
        playSound("hit.wav");
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

function guard_roomEnter(entity, room)
{
    if (entity.room == room && entity.life > 0)
    {
        entity.updateFn = guard_update;
        entity.drawFn = guard_draw;
        entity.drawOverlayFn = guard_drawOverlay;
        entity.roomEnterFn = null;
        entity.roomLeaveFn = guard_roomLeave;
        entity.damageFn = guard_damage;
        entity.position = entity.startPosition;
        entity.dir = entity.startDir;
    }
}

function guard_roomLeave(entity, room)
{
    if (entity.room == room)
    {
        entity.updateFn = null;
        entity.drawFn = null;
        entity.drawOverlayFn = null;
        entity.roomEnterFn = guard_roomEnter;
        entity.roomLeaveFn = null;
        entity.damageFn = null;
    }
}

function guard_draw(entity)
{
    SpriteBatch.drawSpriteAnim(entity.spriteAnim, entity.position, entity.hitFlashAnim.get());
}

function guard_update(entity, dt)
{
    var previousPosition = entity.position;
    var newPosition = entity.position.add(entity.pushBackVel.mul(dt));

    var dis = Vector2.distanceSquared(player.position, entity.position);
    var closeEnough = dis <= 12 * 12;

    if (!entity.isAttacking && !closeEnough)
    {
        var dir = new Vector2(0, 0);

        // Walk toward the player with some basic logic
        // When the guard is far away, it will prioritize Y before X, inverse when close
        if (entity.position.y + 96 < player.position.y || entity.position.y - 96 > player.position.x)
        {
            if (entity.position.y + 8 < player.position.y || entity.position.y - 8 > player.position.y)
            {
                if (player.position.y < entity.position.y + 8) dir.y = -1;
                else if (player.position.y > entity.position.y - 8) dir.y = 1;
            }
            else
            {
                if (player.position.x < entity.position.x + 8) dir.x = -1;
                else if (player.position.x > entity.position.x - 8) dir.x = 1;
            }
        }
        else
        {
            if (entity.position.x + 8 < player.position.x || entity.position.x - 8 > player.position.x)
            {
                if (player.position.x < entity.position.x + 8) dir.x = -1;
                else if (player.position.x > entity.position.x - 8) dir.x = 1;
            }
            else
            {
                if (player.position.y < entity.position.y + 8) dir.y = -1;
                else if (player.position.y > entity.position.y - 8) dir.y = 1;
            }
        }

        if (dir.lengthSquared() == 0)
        {
            entity.spriteAnim.play("idle_" + entity.dir);
            return;
        }
        else 
        {
            if (dir.y > .7) entity.dir = "s";
            else if (dir.x > .7) entity.dir = "e";
            else if (dir.x < -.7) entity.dir = "w";
            else entity.dir = "n";
            entity.spriteAnim.play("run_" + entity.dir);
        }

        newPosition = newPosition.add(dir.mul(GUARD_MOV_SPEED * dt));
    }

    entity.position = tiledMap.collision(previousPosition, newPosition, entity.size);

    // If close enough to the player, start an attack
    if (!entity.isAttacking && !entity.attackCoolDown && closeEnough)
    {
        // Pick attack direction
        var dir = player.position.sub(entity.position);
        dir = dir.normalize();
        if (dir.y > .7) entity.dir = "s";
        else if (dir.x > .7) entity.dir = "e";
        else if (dir.x < -.7) entity.dir = "w";
        else entity.dir = "n";

        entity.isAttacking = true;
        entity.attackCoolDown = true;

        entity.spriteAnim.play("idle_" + entity.dir);
        setTimeout(function() 
        {
            if (entity.life == 0) return;
            
            entity.attackAnim.play("attack");
            entity.spriteAnim.play("attack_" + entity.dir);
            playSound("swoosh.wav");
            setTimeout(function() {entity.isAttacking = false;}, 250);
            setTimeout(function() {entity.attackCoolDown = false;}, 1000);

            // Damage area in front of us
            if (entity.dir == "e") radiusDamage(entity, entity.position.add(new Vector2(PLAYER_DAMAGE_OFFSET, -4)), PLAYER_DAMAGE_RADIUS, PLAYER_DAMAGE);
            else if (entity.dir == "s") radiusDamage(entity, entity.position.add(new Vector2(0, PLAYER_DAMAGE_OFFSET)), PLAYER_DAMAGE_RADIUS, PLAYER_DAMAGE);
            else if (entity.dir == "w") radiusDamage(entity, entity.position.add(new Vector2(-PLAYER_DAMAGE_OFFSET, -4)), PLAYER_DAMAGE_RADIUS, PLAYER_DAMAGE);
            else if (entity.dir == "n") radiusDamage(entity, entity.position.add(new Vector2(0, -PLAYER_DAMAGE_OFFSET - 2)), PLAYER_DAMAGE_RADIUS, PLAYER_DAMAGE);
        }, 500)
    }
}

function guard_drawOverlay(entity)
{
    if (entity.life == 0)
    {
        SpriteBatch.drawSpriteAnim(entity.spriteAnim, entity.position);
    }
    else
    {
        if (!entity.attackAnim.isPlaying()) return;
        if (entity.dir == "e") SpriteBatch.drawSpriteAnim(entity.attackAnim, entity.position);
        else if (entity.dir == "s") SpriteBatch.drawSpriteAnim(entity.attackAnim, entity.position, Color.WHITE, 90);
        else if (entity.dir == "w") SpriteBatch.drawSpriteAnim(entity.attackAnim, entity.position.add(new Vector2(0, -4)), Color.WHITE, 180);
        else if (entity.dir == "n") SpriteBatch.drawSpriteAnim(entity.attackAnim, entity.position.add(new Vector2(0, -2)), Color.WHITE, -90);
    }
}
