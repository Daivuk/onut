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
