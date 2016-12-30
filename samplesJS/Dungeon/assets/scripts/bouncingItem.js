function bouncingItem_init(entity, type)
{
    entity.z = new NumberAnim();
    entity.drawFn = bouncingItem_draw;
    entity.z.queue(16, .5, Tween.EASE_OUT);
    entity.z.queue(0, 1, Tween.BOUNCE_OUT);
    entity.z.play(false);
    
    switch (type)
    {
        case "gold": 
            entity.spriteAnim = playSpriteAnim("coin.spriteanim", "spin");
            break;
        case "bomb": 
            entity.spriteAnim = playSpriteAnim("bomb.spriteanim", "idle");
            break;
    }

    // Delete after 3 sec
    setTimeout(function()
    {
        entity.drawFn = null;
        entities.splice(entities.indexOf(entity), 1);
        drawEntities.splice(drawEntities.indexOf(entity), 1);
    }, 3000);
}

function bouncingItem_draw(entity)
{
    SpriteBatch.drawSpriteAnim(entity.spriteAnim, new Vector2(entity.position.x, entity.position.y - entity.z.get()));
}
