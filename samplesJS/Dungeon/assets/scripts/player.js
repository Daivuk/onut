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
    entity.inventory = {};
    entity.inventory.bomb = 0;
    entity.inventory.gold = 0;
    entity.hitFlashAnim = new ColorAnim(Color.WHITE);
    entity.pushBackVel = Vector2.ZERO;

    // callbacks
    entity.updateFn = player_update;
    entity.drawFn = player_draw;
    entity.drawOverlayFn = player_drawOverlay;
    entity.damageFn = player_damage;

    // So we can touch things
    entity.isTrigger = true;

    player = entity;
}

function player_doneAttacking(entity)
{
    entity.isAttacking = false;
}

function player_update(entity, dt)
{    
    var previousPosition = entity.position;
    var newPosition = entity.position.add(entity.pushBackVel.mul(dt));

    if (!entity.isAttacking)
    {
        if (Input.isJustDown(Key.XARCADE_LBUTTON_1) || Input.isJustDown(Key.SPACE_BAR))
        {
            entity.isAttacking = true;
            entity.attackAnim.play("attack");
            entity.spriteAnim.play("attack_" + entity.dir);
            playSound("swoosh.wav");
            setTimeout(function() {player_doneAttacking(entity)}, 250);

            // Damage area in front of us
            if (entity.dir == "e") radiusDamage(entity, entity.position.add(new Vector2(PLAYER_DAMAGE_OFFSET, -4)), PLAYER_DAMAGE_RADIUS, PLAYER_DAMAGE);
            else if (entity.dir == "s") radiusDamage(entity, entity.position.add(new Vector2(0, PLAYER_DAMAGE_OFFSET)), PLAYER_DAMAGE_RADIUS, PLAYER_DAMAGE);
            else if (entity.dir == "w") radiusDamage(entity, entity.position.add(new Vector2(-PLAYER_DAMAGE_OFFSET, -4)), PLAYER_DAMAGE_RADIUS, PLAYER_DAMAGE);
            else if (entity.dir == "n") radiusDamage(entity, entity.position.add(new Vector2(0, -PLAYER_DAMAGE_OFFSET - 2)), PLAYER_DAMAGE_RADIUS, PLAYER_DAMAGE);
            return;
        }

        if (Input.isJustDown(Key.XARCADE_LBUTTON_2) || Input.isJustDown(Key.B))
        {
            if (entity.inventory.bomb > 0)
            {
                --entity.inventory.bomb;
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

            // move
            newPosition = newPosition.add(dir.mul(PLAYER_MOV_SPEED * dt));
        }
    }

    // Collisions
    entity.position = tiledMap.collision(previousPosition, newPosition, entity.size);
}

function player_drawOverlay(entity)
{
    if (!entity.attackAnim.isPlaying()) return;
    if (entity.dir == "e") SpriteBatch.drawSpriteAnim(entity.attackAnim, entity.position);
    else if (entity.dir == "s") SpriteBatch.drawSpriteAnim(entity.attackAnim, entity.position, Color.WHITE, 90);
    else if (entity.dir == "w") SpriteBatch.drawSpriteAnim(entity.attackAnim, entity.position.add(new Vector2(0, -4)), Color.WHITE, 180);
    else if (entity.dir == "n") SpriteBatch.drawSpriteAnim(entity.attackAnim, entity.position.add(new Vector2(0, -2)), Color.WHITE, -90);
}

function player_draw(entity)
{
    SpriteBatch.drawSpriteAnim(entity.spriteAnim, entity.position, entity.hitFlashAnim.get());
}

function player_damage(entity, fromEntity, amount)
{
    if (entity.hitFlashAnim.isPlaying() || entity.life == 0) return; // Can't hit while in flashing mode
    entity.life = Math.max(0, entity.life - amount);
    if (entity.life == 0)
    {
        // Ded
        entity.spriteAnim.play("die");

        entity.updateFn = null;
            
        playSound("gameover.wav");

        fadeAnim.queue(0, 1);
        fadeAnim.queue(1, 1, Tween.LINEAR, function()
        {
            goMainMenu();
        });
        fadeAnim.queue(1, 10);
        fadeAnim.play(Loop.NONE);
    }
    else
    {
        // Push back and flash
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
