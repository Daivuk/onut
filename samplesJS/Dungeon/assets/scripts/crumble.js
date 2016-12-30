function crumble_init(entity)
{
    door_init(entity); // Inherit basic door behavior

    // Touch callback
    entity.touchFn = crumble_touch;
    entity.damageFn = crumble_damage;
}

function crumble_touch(entity, fromEntity)
{
    if (entity.isOpen) door_touch(entity, fromEntity);
}

function crumble_damage(entity, fromEntity, amount)
{
    if (amount != 10 || entity.isOpen) return; // 10 is the bomb damage signature

    door_open(entity);
}
