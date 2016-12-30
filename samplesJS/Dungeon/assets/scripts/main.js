var fadeAnim = new NumberAnim(0);
var font = getFont("font.fnt");
var tiledMap = getTiledMap("dungeon.tmx");
var player = {};

// Generate rooms
room_init();

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
    "door": door_init
};
for (var i = 0; i < objCount; ++i) 
{
    var entity = tiledMap.getObject("entities", i);
    var fn = objInitFns[entity.type];

    // Call init function based on type
    if (fn) fn(entity);

    // Add entity to various lists
    entities.push(entity);
    if (entity.updateFn) updateEntities.push(entity);
    if (entity.drawFn) drawEntities.push(entity);
    if (entity.drawOverlayFn) drawOverlayEntities.push(entity);
    if (entity.damageFn) damageEntities.push(entity);
}

doors_init();

var currentRoom = room_getAt(Math.floor(player.position.x / 16), Math.floor(player.position.y / 16));
room_show(currentRoom);
var camera = room_calculateCameraPos(currentRoom);

function entity_draw(entity)
{
    SpriteBatch.drawSpriteAnim(entity.spriteAnim, entity.position);
}

function update(dt)
{
    if (!fadeAnim.isPlaying())
    {
        for (var i = 0; i < drawEntities.length; ++i)
        {
            var entity = drawEntities[i];
            entity.updateFn(dt, entity);
        }
    }
    camera = room_calculateCameraPos(currentRoom, player.position);
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
        entity.drawFn(entity);
    }
    for (var i = 0; i < drawOverlayEntities.length; ++i)
    {
        var entity = drawOverlayEntities[i];
        entity.drawOverlayFn(entity);
    }
    SpriteBatch.end();

    // Fade
    if (fadeAnim.isPlaying())
    {
        SpriteBatch.begin();
        SpriteBatch.drawRect(null, new Rect(0, 0, Renderer.getResolution()), new Color(0, 0, 0, fadeAnim.get()));
        SpriteBatch.end();
    }

    //SpriteBatch.begin();
    //SpriteBatch.drawText(font, "Hello World");
    //SpriteBatch.end();
}
