function vase_init(entity)
{
    // Containing resources
    entity.gold = entity.properties["Vase:Gold"];

    // Damage callback so we can break the vase
    entity.damageFn = vase_damage;
}

function vase_damage(entity, amount)
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
            spawnItem(entity.position, "gold");
        }
        playSound("gold.wav");
    }

    // Unregister to listening to damage
    entity.damageFn = null;
}
