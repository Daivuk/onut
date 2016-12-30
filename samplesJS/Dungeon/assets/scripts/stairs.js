function stairs_init(entity)
{
    entity.size = entity.size.add(-4);

    entity.touchFn = stairs_touch;
}

function stairs_touch(stairs, fromEntity)
{
    if (fromEntity != player) return; // Only player can trigger end level

    for (var i = 0; i < entities.length; ++i)
    {
        var other = entities[i];
        if (other.roomLeaveFn) other.roomLeaveFn(other);
    }
    
    fadeAnim.playSingle(0, 1, 2, Tween.LINEAR, Loop.NONE, function()
    {
        quit();
    });
}
