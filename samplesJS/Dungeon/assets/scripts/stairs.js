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
    
    fadeAnim.queue(1, 2, Tween.LINEAR, function()
    {
        goMainMenu();
    });
    fadeAnim.queue(1, 10);
    fadeAnim.play(Loop.NONE);
}
