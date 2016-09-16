var player = new VideoPlayer();

player.setSource("https://download.blender.org/durian/trailer/sintel_trailer-720p.mp4");
player.play();

function update()
{
    player.update();
}

function render()
{
    // Clear
    Renderer.clear(Color.BLACK);

    var fullScreenRect = new Rect(0, 0, Renderer.getResolution());
    var texture = player.getRenderTarget();
    var drawRect = fullScreenRect.fitSize(texture.getSize());

    SpriteBatch.begin();
    SpriteBatch.drawRect(texture, drawRect);
    SpriteBatch.end();
}
