var spriteAngle = 0.0;
var nutTexture = Texture.createFromFile("onutLogo.png");
var frameTexture = Texture.createFromFile("frameSmall.png");
var chainTexture = Texture.createFromFile("chain.png");

function update(dt) {
    spriteAngle += dt * 45;
}

function render() {
    // Clear
    Renderer.clear(Color.fromHexRGB(0x1d232d));

    // Begin a batch
    SpriteBatch.begin();

    // Opaque
    SpriteBatch.drawRect(null, new Rect(0, 0, 64, 64));
    SpriteBatch.drawRect(null, new Rect(64, 0, 64, 64), new Color(1, 0, 0, 1)); // Red
    SpriteBatch.drawInclinedRect(null, new Rect(192, 0, 64, 64), -.5); // Inclined

    // Textured
    SpriteBatch.drawRect(nutTexture, new Rect(0, 64, 64, 64));
    SpriteBatch.drawRect(nutTexture, new Rect(64, 64, 64, 64), new Color(1, 0, 0, 1)); // Tinted

    // Per vertex coloring
    SpriteBatch.drawRectWithColors(null, new Rect(0, 128, 64, 64), new Color(1, 0, 0, 1), new Color(1, 1, 0, 1), new Color(0, 1, 0, 1), new Color(0, 0, 1, 1));

    // Custom UVs
    SpriteBatch.drawRectWithUVs(nutTexture, new Rect(64, 128, 64, 64), new Vector4(0, 0, .5, .5));

    // Split the texture in 4 corner (Good for corsairs)
    SpriteBatch.draw4Corner(nutTexture, new Rect(128, 64, 160, 160));
    
    // Scale 9, 9 Patch, 9 slice (whatever you guys call it)
    SpriteBatch.drawRectScaled9(frameTexture, new Rect(0, 256, 160, 160), new Vector4(48, 48, 48, 48));
    SpriteBatch.drawRectScaled9(frameTexture, new Rect(160, 256, 256, 160), new Vector4(48, 48, 48, 48));
    SpriteBatch.drawRectScaled9RepeatCenters(frameTexture, new Rect(416, 256, 256, 160), new Vector4(48, 48, 48, 48));

    // Sprites
    SpriteBatch.drawSprite(nutTexture, new Vector2(64, 480));
    SpriteBatch.drawSprite(nutTexture, new Vector2(192, 480), Color.WHITE, spriteAngle);
    SpriteBatch.drawSprite(nutTexture, new Vector2(320, 440), Color.WHITE, spriteAngle, .5);
    SpriteBatch.drawSprite(null, new Vector2(320, 500), Color.WHITE, spriteAngle, 32);

    // With a custom matrix
    var customTransform = Matrix.createRotationZ(spriteAngle).mul(Matrix.createScale(new Vector3(4, .5, 1))).mul(Matrix.createTranslation(new Vector3(520, 120, 0)));
    SpriteBatch.drawTransformedSprite(nutTexture, customTransform);

    // Lines
    SpriteBatch.drawBeam(chainTexture, new Vector2(448, 480), new Vector2(648, 440), 32);
    SpriteBatch.drawBeam(chainTexture, new Vector2(448, 520), new Vector2(648, 480), 32, Color.WHITE, spriteAngle * .1);

    // Blend modes
    SpriteBatch.setBlend(BlendMode.ADD);
    SpriteBatch.drawSprite(nutTexture, new Vector2(400, 50), Color.WHITE, 0, .25);
    SpriteBatch.setBlend(BlendMode.ALPHA);
    SpriteBatch.drawSprite(nutTexture, new Vector2(450, 50), Color.WHITE, 0, .25);
    SpriteBatch.setBlend(BlendMode.FORCE_WRITE);
    SpriteBatch.drawSprite(nutTexture, new Vector2(500, 50), Color.WHITE, 0, .25);
    SpriteBatch.setBlend(BlendMode.MULTIPLY);
    SpriteBatch.drawSprite(nutTexture, new Vector2(550, 50), Color.WHITE, 0, .25);
    SpriteBatch.setBlend(BlendMode.OPAQUE);
    SpriteBatch.drawSprite(nutTexture, new Vector2(600, 50), Color.WHITE, 0, .25);
    SpriteBatch.setBlend(BlendMode.PREMULTIPLIED);
    SpriteBatch.drawSprite(nutTexture, new Vector2(650, 50), Color.WHITE, 0, .25);

    // Origin
    SpriteBatch.drawSprite(nutTexture, new Vector2(400, 200), Color.WHITE, 0, .25, Vector2.TOP_LEFT);
    SpriteBatch.drawCross(new Vector2(400, 200), 10, new Color(1, 1, 0, 1));
    SpriteBatch.drawSprite(nutTexture, new Vector2(500, 200), Color.WHITE, 0, .25, Vector2.CENTER);
    SpriteBatch.drawCross(new Vector2(500, 200), 10, new Color(1, 1, 0, 1));
    SpriteBatch.drawSprite(nutTexture, new Vector2(600, 200), Color.WHITE, 0, .25, Vector2.BOTTOM_RIGHT);
    SpriteBatch.drawCross(new Vector2(600, 200), 10, new Color(1, 1, 0, 1));

    // Filtering
    SpriteBatch.setFilter(FilterMode.NEAREST);
    SpriteBatch.drawSprite(nutTexture, new Vector2(725, 100), Color.WHITE, 0, 1.5);
    SpriteBatch.setFilter(FilterMode.LINEAR);
    SpriteBatch.drawSprite(nutTexture, new Vector2(725, 300), Color.WHITE, 0, 1.5);

    // End and flush the batch
    SpriteBatch.end();
}
