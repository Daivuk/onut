function dropLoot(entity)
{
    if (!entity.inventory) return;

    // Loop inventory
    var keys = Object.keys(entity.inventory);
    var delay = 0;
    for (var i = 0; i < keys.length; ++i)
    {
        var key = keys[i];
        var count = entity.inventory[key];
        for (var i = 0; i < count; ++i)
        {
            setTimeout(function()
            {
                spawnItem(entity.position, key);
                playSound(key + ".wav");
                ++player.inventory[key];
            }, delay);
            delay += 200;
        }
    }

    // It's empty now
    entity.inventory = {};
}

function spawnItem(position, type)
{
    var entity = {};
    entity.position = new Vector2(position);
    entity.size = new Vector2(0, 0);
    bouncingItem_init(entity, type);
    entity_add(entity);
}

function bouncingItem_init(entity, type)
{
    entity.drawFn = bouncingItem_draw;

    entity.z = new NumberAnim();
    entity.z.queue(16, .25, Tween.EASE_OUT);
    entity.z.queue(0, .75, Tween.BOUNCE_OUT);
    entity.z.play(false);

    // Randomize a bit his position
    entity.position.x += Math.random() * 12 - 6;
    entity.position.y += Math.random() * 12 - 6;
    
    switch (type)
    {
        case "gold": 
            entity.spriteAnim = playSpriteAnim("coin.spriteanim", "spin");
            break;
        case "bomb": 
            entity.spriteAnim = playSpriteAnim("bomb.spriteanim", "idle");
            break;
    }

    // Delete after 2 sec
    setTimeout(function()
    {
        entity_kill(entity);
    }, 2000);
}

function bouncingItem_draw(entity)
{
    SpriteBatch.drawSpriteAnim(entity.spriteAnim, new Vector2(entity.position.x, entity.position.y - entity.z.get()));
}
