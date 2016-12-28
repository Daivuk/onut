var fadeAnim = new NumberAnim(0);
var font = getFont("font.fnt");
var tiledMap = getTiledMap("dungeon.tmx");
var player = player_init();

doors_init();
room_init();
var startRoom = room_getAt(Math.floor(player.position.x / 16), Math.floor(player.position.y / 16));
room_show(startRoom);
var camera = new Vector2(startRoom.center);

function drawEntity(entity)
{
    SpriteBatch.drawSpriteAnim(entity.spriteAnim, entity.position);
}

function update(dt)
{
    if (fadeAnim.isPlaying()) return;
    player_update(dt, player);
}

function render()
{
    Renderer.clear(Color.BLACK);

    var transform = Matrix.createTranslation(new Vector3(-camera.x + 128, -camera.y + 80));
    
    SpriteBatch.begin(transform);
    tiledMap.render();
    drawEntity(player)
    SpriteBatch.end();

    // Fade
    if (fadeAnim.isPlaying())
    {
        SpriteBatch.begin();
        SpriteBatch.drawRect(null, new Rect(0, 0, Renderer.getResolution()), new Color(0, 0, 0, fadeAnim.get()));
        SpriteBatch.end();
    }

    //SpriteBatch.begin();
    //SpriteBatch.drawText(font, JSON.stringify(ent_player));
    //SpriteBatch.end();
}
