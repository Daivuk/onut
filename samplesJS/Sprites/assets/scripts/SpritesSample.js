var spriteAngle = 0.0;
var nutTexture = Texture("onutLogo.png");
var frameTexture = Texture("frameSmall.png");
var chainTexture = Texture("chain.png");

function update(dt) {
    spriteAngle += dt * 45;
}

function render() {
    // Clear
    Renderer.clear(ColorHexRGB(0x1d232d));

    // Begin a batch
    SpriteBatch.begin();

    // Opaque
    SpriteBatch.drawRect(null, Rect(0, 0, 64, 64));
    SpriteBatch.drawRect(null, Rect(64, 0, 64, 64), Color(1, 0, 0, 1)); // Red
    SpriteBatch.drawInclinedRect(null, Rect(192, 0, 64, 64), -.5); // Inclined

    // Textured
    SpriteBatch.drawRect(nutTexture, Rect(0, 64, 64, 64));
    SpriteBatch.drawRect(nutTexture, Rect(64, 64, 64, 64), Color(1, 0, 0, 1)); // Tinted

    // Per vertex coloring
    SpriteBatch.drawRectWithColors(null, Rect(0, 128, 64, 64), Color(1, 0, 0, 1), Color(1, 1, 0, 1), Color(0, 1, 0, 1), Color(0, 0, 1, 1));

    // End and flush the batch
    SpriteBatch.end();
}
