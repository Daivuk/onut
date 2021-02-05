/** @param {Entity} e */
function _onUpdate(e, c, dt)
{
    var player = findEntity("Player");
    if (player)
    {
        e.setPosition(player.getPosition());
    }
}
