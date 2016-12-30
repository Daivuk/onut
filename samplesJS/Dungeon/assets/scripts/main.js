// Globals
var fadeAnim = new NumberAnim(0);
var font = getFont("font.fnt");
var tiledMap = getTiledMap("dungeon.tmx");
var player = {};
var currentRoom = {};
var camera = new Vector2();

// Generate rooms
rooms_init();

// For optimization, we have different lists wether an entity is updatable/drawable/etc
var entities = [];
var updateEntities = [];
var drawEntities = [];
var drawOverlayEntities = [];
var damageEntities = [];

// Parse map entities
var objCount = tiledMap.getObjectCount("entities");
var objInitFns = {
    "start_location": player_init,
    "door": door_init,
    "vase": vase_init
};
for (var i = 0; i < objCount; ++i) 
{
    var entity = tiledMap.getObject("entities", i);
    var initFn = objInitFns[entity.type];

    // Center the entity on the tile
    entity.position = entity.size.div(2).add(entity.position);

    // Halve size
    entity.size = entity.size.div(2);

    // Call init function based on type
    if (initFn) initFn(entity);

    // Find the room in which the entity is
    entity.room = room_getAt(Math.floor(entity.position.x / 16), Math.floor(entity.position.y / 16));

    // Add entity to various lists
    entities.push(entity);
    if (entity.updateFn) updateEntities.push(entity);
    if (entity.drawFn) drawEntities.push(entity);
    if (entity.drawOverlayFn) drawOverlayEntities.push(entity);
    if (entity.damageFn) damageEntities.push(entity);
}

// Show the start room
room_show(player.room);

function entity_draw(entity)
{
    SpriteBatch.drawSpriteAnim(entity.spriteAnim, entity.position);
}

function radiusDamage(fromEntity, position, radius, damage)
{
    for (var i = 0; i < damageEntities.length; ++i)
    {
        var entity = damageEntities[i];
        if (!entity.damageFn) continue;
        if (entity == fromEntity) continue;
        if (entity.position.x + entity.size.x >= position.x - radius &&
            entity.position.x - entity.size.x <= position.x + radius &&
            entity.position.y + entity.size.y >= position.y - radius &&
            entity.position.y - entity.size.y <= position.y + radius)
        {
            entity.damageFn(entity, damage);
        }
    }
}

function spawnItem(position, type)
{
    var entity = {};
    entity.position = position;
    bouncingItem_init(entity, type);
    entities.push(entity);
    drawEntities.push(entity);
}

function update(dt)
{
    if (!fadeAnim.isPlaying())
    {
        for (var i = 0; i < drawEntities.length; ++i)
        {
            var entity = drawEntities[i];
            if (entity.updateFn) entity.updateFn(entity, dt);
        }
    }
    camera = room_calculateCameraPos(player.room, player.position);
}

function render()
{
    Renderer.clear(Color.BLACK);

    var transform = Matrix.createTranslation(new Vector3(-camera.x + 128, -camera.y + 80));
    
    SpriteBatch.begin(transform);
    tiledMap.render();
    // We always draw upper entities first so they render in the right order
    drawEntities.sort(function(a, b){return a.position.y < b.position.y});
    for (var i = 0; i < drawEntities.length; ++i)
    {
        var entity = drawEntities[i];
        if (entity.drawFn) entity.drawFn(entity);
    }
    for (var i = 0; i < drawOverlayEntities.length; ++i)
    {
        var entity = drawOverlayEntities[i];
        if (entity.drawOverlayFn) entity.drawOverlayFn(entity);
    }
 /*   for (var i = 0; i < entities.length; ++i)
    {
        var entity = entities[i];
        SpriteBatch.drawRect(null, new Rect(entity.position.x - entity.size.x, entity.position.y - entity.size.y, entity.size.x * 2, entity.size.y * 2), new Color(1, .25, .25, .25));
    }*/
    SpriteBatch.end();

    // Fade
    if (fadeAnim.isPlaying())
    {
        SpriteBatch.begin();
        SpriteBatch.drawRect(null, new Rect(0, 0, Renderer.getResolution()), new Color(0, 0, 0, fadeAnim.get()));
        SpriteBatch.end();
    }
}
