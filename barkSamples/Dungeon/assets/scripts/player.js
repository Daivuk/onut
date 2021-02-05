// Props create variables that are serializable and visible to the editor.
_props = {
    speed: { type: "number", default: 32 },
    stepSound: { type: "Sound", default: "step.wav" }
};

/** @param {Entity} e */
function _onCreate(e, c)
{
    // Defining variables directly on the component means they won't be
    // serialized or visible in the editor.
    c.dir = "s";
}

/** @param {Entity} e @param {number} dt */
function _onUpdate(e, c, dt)
{
    var move = new Vector2(0, 0);

    if (Input.isDown(Key.D)) move.x += 1;
    if (Input.isDown(Key.A)) move.x -= 1;
    if (Input.isDown(Key.W)) move.y -= 1;
    if (Input.isDown(Key.S)) move.y += 1;

    if (move.lengthSquared() != 0)
    {
        move = move.normalize();

        var p = e.getPosition();
        p.x += move.x * 64 * dt;
        p.y += move.y * 64 * dt;
        e.setPosition(p);

        if (move.y > .8) c.dir = 's'
        else if (move.y < -.8) c.dir = 'n'
        else if (move.x < 0) c.dir = 'w'
        else if (move.x > 0) c.dir = 'e'
        
        e.spriteAnimRenderer.anim = "run_" + c.dir;
    }
    else
    {
        e.spriteAnimRenderer.anim = "idle_" + c.dir;
    }
}
