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

    // Custom UVs
    SpriteBatch.drawRectWithUVs(nutTexture, Rect(64, 128, 64, 64), Vector4(0, 0, .5, .5));

    // Split the texture in 4 corner (Good for corsairs)
    SpriteBatch.draw4Corner(nutTexture, Rect(128, 64, 160, 160));
    
    // Scale 9, 9 Patch, 9 slice (whatever you guys call it)
    SpriteBatch.drawRectScaled9(frameTexture, Rect(0, 256, 160, 160), Vector4(48, 48, 48, 48));
    SpriteBatch.drawRectScaled9(frameTexture, Rect(160, 256, 256, 160), Vector4(48, 48, 48, 48));
    SpriteBatch.drawRectScaled9RepeatCenters(frameTexture, Rect(416, 256, 256, 160), Vector4(48, 48, 48, 48));

    // Sprites
    SpriteBatch.drawSprite(nutTexture, Vector2(64, 480));
    SpriteBatch.drawSprite(nutTexture, Vector2(192, 480), whiteColor(), spriteAngle);
    SpriteBatch.drawSprite(nutTexture, Vector2(320, 440), whiteColor(), spriteAngle, .5);
    SpriteBatch.drawSprite(null, Vector2(320, 500), whiteColor(), spriteAngle, 32);

    // With a custom matrix
    var customTransform = identityMatrix();
    customTransform = rotateMatrixAroundZ(customTransform, spriteAngle);
    customTransform = scaleMatrix(customTransform, 4, .5, 1);
    customTransform = translateMatrix(customTransform, 520, 120, 0);
    SpriteBatch.drawTransformedSprite(nutTexture, customTransform);

    // Lines
    SpriteBatch.drawBeam(chainTexture, Vector2(448, 480), Vector2(648, 440), 32);
    SpriteBatch.drawBeam(chainTexture, Vector2(448, 520), Vector2(648, 480), 32, whiteColor(), spriteAngle * .1);

    // Blend modes
    SpriteBatch.setBlend(BlendMode.Add);
    SpriteBatch.drawSprite(nutTexture, Vector2(400, 50), whiteColor(), 0, .25);
    SpriteBatch.setBlend(BlendMode.Alpha);
    SpriteBatch.drawSprite(nutTexture, Vector2(450, 50), whiteColor(), 0, .25);
    SpriteBatch.setBlend(BlendMode.ForceWrite);
    SpriteBatch.drawSprite(nutTexture, Vector2(500, 50), whiteColor(), 0, .25);
    SpriteBatch.setBlend(BlendMode.Multiply);
    SpriteBatch.drawSprite(nutTexture, Vector2(550, 50), whiteColor(), 0, .25);
    SpriteBatch.setBlend(BlendMode.Opaque);
    SpriteBatch.drawSprite(nutTexture, Vector2(600, 50), whiteColor(), 0, .25);
    SpriteBatch.setBlend(BlendMode.PreMultiplied);
    SpriteBatch.drawSprite(nutTexture, Vector2(650, 50), whiteColor(), 0, .25);

    // Origin
    SpriteBatch.drawSprite(nutTexture, Vector2(400, 200), whiteColor(), 0, .25, Align.TopLeft);
    SpriteBatch.drawCross(Vector2(400, 200), 10, Color(1, 1, 0, 1));
    SpriteBatch.drawSprite(nutTexture, Vector2(500, 200), whiteColor(), 0, .25, Align.Center);
    SpriteBatch.drawCross(Vector2(500, 200), 10, Color(1, 1, 0, 1));
    SpriteBatch.drawSprite(nutTexture, Vector2(600, 200), whiteColor(), 0, .25, Align.BottomRight);
    SpriteBatch.drawCross(Vector2(600, 200), 10, Color(1, 1, 0, 1));

    // Filtering
    SpriteBatch.setFilter(FilterMode.Nearest);
    SpriteBatch.drawSprite(nutTexture, Vector2(725, 100), whiteColor(), 0, 1.5);
    SpriteBatch.setFilter(FilterMode.Linear);
    SpriteBatch.drawSprite(nutTexture, Vector2(725, 300), whiteColor(), 0, 1.5);

    // End and flush the batch
    SpriteBatch.end();
}
