function chest_init(entity)
{
    // Make it slightly bigger so the player can trigger it
    entity.size = entity.size.add(1);

    // Containing resources
    entity.gold = entity.properties["Chest:Gold"];
    entity.bomb = entity.properties["Chest:Bomb"];

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
    if (entity.bomb)
    {
        for (var i = 0; i < entity.bomb; ++i)
        {
            setTimeout(function()
            {
                spawnItem(entity.position, "bomb");
                ++player.bomb;
            }, (entity.gold + i) * 500);
        }
    }

    entity_kill(entity);
}
