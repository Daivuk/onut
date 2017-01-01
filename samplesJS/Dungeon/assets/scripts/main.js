// Globals
var font = getFont("font.fnt");
var fadeAnim = new NumberAnim(0);;
var tiledMap = null;
var player = {};
var camera = new Vector2();
var ui = null;

// For optimization, we have different lists wether an entity is updatable/drawable/etc
var entities = [];
var updateEntities = [];
var drawEntities = [];
var drawOverlayEntities = [];
var damageEntities = [];
var touchEntities = [];
var triggerEntities = [];

var gameState = "mainMenu";

goMainMenu();

function goMainMenu()
{
    gameState = "mainMenu";
    setUINavigation(true);
    ui = loadUI("mainMenu.json");
  //  ui.setVisible(true);
    findUI("btnPlay").setOnClick(restartLevel);
    findUI("btnQuit").setOnClick(quit);
}

function restartLevel()
{
    gameState = "play";
  //  ui.setVisible(false);
    unloadUI("mainMenu.json");

    fadeAnim = new NumberAnim(0);
    tiledMap = getFreshTiledMap("dungeon.tmx");
    player = {};
    camera = new Vector2();
        
    entities = [];
    updateEntities = [];
    drawEntities = [];
    drawOverlayEntities = [];
    damageEntities = [];
    touchEntities = [];
    triggerEntities = [];

    // Generate rooms
    rooms_init();

    // Parse map entities
    var objCount = tiledMap.getObjectCount("entities");
    var objInitFns = {
        "start_location": player_init,
        "door": door_init,
        "vase": vase_init,
        "chest": chest_init,
        "crumble": crumble_init,
        "stairs": stairs_init,
        "guard": guard_init,
        "bat": bat_init
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

        entity_add(entity);
    }

    // Show the start room
    room_show(player.room);
}

// Add entity
function entity_add(entity)
{
    entities.push(entity);

    // Add entity to various lists
    if (entity.updateFn) updateEntities.push(entity);
    if (entity.drawFn) drawEntities.push(entity);
    if (entity.drawOverlayFn) drawOverlayEntities.push(entity);
    if (entity.damageFn) damageEntities.push(entity);
    if (entity.touchFn) touchEntities.push(entity);
    if (entity.isTrigger) triggerEntities.push(entity);
}

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
            entity.damageFn(entity, fromEntity, damage);
        }
    }
}

function entity_kill(entity)
{
    var hasUpdate = false;
    var hasDraw = false;
    var hasDrawOverlay = false;
    var hasDamage = false;
    var hasTouch = false;
    var isTrigger = entity.isTrigger;

    if (entity.updateFn) {hasUpdate = true; entity.updateFn = null;}
    if (entity.drawFn) {hasDraw = true; entity.drawFn = null;}
    if (entity.drawOverlayFn) {hasDrawOverlay = true; entity.drawOverlayFn = null;}
    if (entity.damageFn) {hasDamage = true; entity.damageFn = null;}
    if (entity.touchFn) {hasTouch = true; entity.touchFn = null;}
    if (isTrigger) entity.isTrigger = false;

    defer(function()
    {
        entities.splice(entities.indexOf(entity), 1);
        if (isTrigger) triggerEntities.splice(triggerEntities.indexOf(entity), 1);
        if (hasUpdate) updateEntities.splice(updateEntities.indexOf(entity), 1);
        if (hasDraw) drawEntities.splice(drawEntities.indexOf(entity), 1);
        if (hasDrawOverlay) drawOverlayEntities.splice(drawOverlayEntities.indexOf(entity), 1);
        if (hasDamage) damageEntities.splice(damageEntities.indexOf(entity), 1);
        if (hasTouch) touchEntities.splice(touchEntities.indexOf(entity), 1);
    });
}

function update(dt)
{
    if (gameState == "mainMenu") return;

    // Update entities
    if (!fadeAnim.isPlaying())
    {
        // Touch events first
        for (var i = 0; i < touchEntities.length; ++i)
        {
            var touchEntity = touchEntities[i];
            if (!touchEntity.touchFn) continue;
            for (var j = 0; j < triggerEntities.length; ++j)
            {
                var triggerEntity = triggerEntities[j];
                if (touchEntity.position.x + touchEntity.size.x >= triggerEntity.position.x - triggerEntity.size.x &&
                    touchEntity.position.x - touchEntity.size.x <= triggerEntity.position.x + triggerEntity.size.x &&
                    touchEntity.position.y + touchEntity.size.y >= triggerEntity.position.y - triggerEntity.size.y &&
                    touchEntity.position.y - touchEntity.size.y <= triggerEntity.position.y + triggerEntity.size.y)
                {
                    touchEntity.touchFn(touchEntity, triggerEntity);
                }
            }
        }
    }

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
    if (gameState == "mainMenu") return;

    Renderer.clear(Color.BLACK);

    var transform = Matrix.createTranslation(new Vector3(-camera.x + 128, -camera.y + 80));
    
    SpriteBatch.begin(transform);
    tiledMap.render();
    // We always draw upper entities first so they render in the right order
    drawEntities.sort(function(a, b){return a.position.y < b.position.y ? -1 : 1});
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

    // Draw in screen space now
    SpriteBatch.begin();

    // Fade
    if (fadeAnim.isPlaying())
    {
        SpriteBatch.drawRect(null, new Rect(0, 0, Renderer.getResolution()), new Color(0, 0, 0, fadeAnim.get()));
    }

    // Player's life
    for (var i = 0; i < player.life; ++i)
    {
        SpriteBatch.drawSprite(getTexture("heart.PNG"), new Vector2(8 + i * 8, 8));
    }

    // Player's gold
    for (var i = 0; i < player.inventory.gold; ++i)
    {
        SpriteBatch.drawSprite(getTexture("coinIcon.PNG"), new Vector2(Renderer.getResolution().x - 8 - i * 2, 8));
    }

    // Player's bombs
    for (var i = 0; i < player.inventory.bomb; ++i)
    {
        SpriteBatch.drawSprite(getTexture("bombIcon.PNG"), new Vector2(Renderer.getResolution().x - 8 - i * 8, 8 + 12));
    }

    SpriteBatch.end();
}
