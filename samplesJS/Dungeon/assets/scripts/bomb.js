function spawnBomb(position)
{
    var entity = {};
    entity.position = new Vector2(position);
    entity.size = new Vector2(4, 4);
    bomb_init(entity);
    entity_add(entity);
}

function bomb_init(entity)
{
    entity.drawFn = entity_draw;

    entity.spriteAnim = playSpriteAnim("bomb.spriteanim", "fuse");

    // Last second flashes faster, then explodes
    setTimeout(function()
    {
        entity.spriteAnim.setSpeed(2);
    }, 2000);

    setTimeout(function()
    {
        radiusDamage(entity, entity.position, 20, 10);
        entity.spriteAnim = playSpriteAnim("explosion.spriteanim", "explosion");
        playSound("explosion.wav");
    }, 3000);

    setTimeout(function()
    {
        entity_kill(entity);
    }, 4000);
}
