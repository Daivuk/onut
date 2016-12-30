var fadeAnim = new NumberAnim(0);
var font = getFont("font.fnt");
var tiledMap = getTiledMap("dungeon.tmx");
var player = player_init();

doors_init();
room_init();
var currentRoom = room_getAt(Math.floor(player.position.x / 16), Math.floor(player.position.y / 16));
room_show(currentRoom);

function drawEntity(entity)
{
    SpriteBatch.drawSpriteAnim(entity.spriteAnim, entity.position);
}

function update(dt)
{
    if (!fadeAnim.isPlaying())
    {
        player_update(dt, player);
    }
    camera = room_calculateCameraPos(currentRoom, player.position);
}

function render()
{
    Renderer.clear(Color.BLACK);

    var transform = Matrix.createTranslation(new Vector3(-camera.x + 128, -camera.y + 80));
    
    SpriteBatch.begin(transform);
    tiledMap.render();
    drawEntity(player);
    player_drawSpecials(player);
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
