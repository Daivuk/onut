function vase_init(entity)
{
    // Containing resources
    entity.gold = entity.properties["Vase:Gold"];

    // Damage callback so we can break the vase
    entity.damageFn = vase_damage;
}

function vase_damage(entity, fromEntity, amount)
{
    var x = Math.floor(entity.position.x / 16);
    var y = Math.floor(entity.position.y / 16);

    var tileId = tiledMap.getTileAt("obj", x, y);
    ++tileId;
    tiledMap.setTileAt("obj", x, y, tileId);

    playSound("potBreak.wav");

    // Spawn items
    if (entity.gold)
    {
        for (var i = 0; i < entity.gold; ++i)
        {
            setTimeout(function()
            {
                spawnItem(entity.position, "gold");
                playSound("gold.wav", 1 / (i + 1));
                ++player.gold;
            }, i * 500);
        }
    }
    
    entity_kill(entity);
}
