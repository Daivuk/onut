function chest_init(entity)
{
    // Make it slightly bigger so the player can trigger it
    entity.size = entity.size.add(1);

    // Containing resources
    entity.inventory = {};
    entity.inventory["gold"] = entity.properties["Chest:Gold"];
    entity.inventory["bomb"] = entity.properties["Chest:Bomb"];

    // Callbacks
    entity.touchFn = chest_touch;
}

function chest_touch(entity, fromEntity)
{
    if (fromEntity != player) return; // Only player can open chests

    var x = Math.floor(entity.position.x / 16);
    var y = Math.floor(entity.position.y / 16);

    var tileId = tiledMap.getTileAt("obj", x, y);
    ++tileId;
    tiledMap.setTileAt("obj", x, y, tileId);

    playSound("chestOpen.wav");

    // Spawn items
    dropLoot(entity);

    entity_kill(entity);
}
