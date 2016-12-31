function vase_init(entity)
{
    // Containing resources
    entity.inventory = {}
    entity.inventory["gold"] = entity.properties["Vase:Gold"];

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
    dropLoot(entity);
    
    entity_kill(entity);
}
