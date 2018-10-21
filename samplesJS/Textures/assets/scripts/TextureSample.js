var font = Font.createFromFile("font.fnt");

// From file
var textureFromFile = Texture.createFromFile("onutLogo.png");

// Create a render target
var renderTarget = Texture.createRenderTarget(new Vector2(256, 256));

// From raw data
var rawData = new Uint32Array(2 * 2);
rawData[0] = 0xFFFF00FF;
rawData[1] = 0xFF969696;
rawData[2] = 0xFF969696;
rawData[3] = 0xFFFF00FF;
textureFromData = Texture.createFromData(rawData, new Vector2(2, 2));

// dynamic
var dynamic = Texture.createDynamic(new Vector2(1, 128));
var dynamicData = new ArrayBuffer(128 * 4);
var dynamicAnim = 0;

function update(dt)
{
    dynamicAnim += dt * 85;
    while (dynamicAnim >= 256) dynamicAnim -= 256;
}

function render()
{
    // Clear
    Renderer.clear(Color.fromHexRGB(0x1d232d));

    // Draw stuff to our render target
    Renderer.pushRenderTarget(renderTarget);
    Renderer.pushViewport(new Rect(0, 0, 256, 256));
    SpriteBatch.begin();
    SpriteBatch.drawRect(textureFromFile, new Rect(0, 0, 128, 128));
    SpriteBatch.drawRect(textureFromFile, new Rect(128, 0, 128, 128));
    SpriteBatch.drawRect(textureFromFile, new Rect(0, 128, 128, 128));
    SpriteBatch.drawRect(textureFromFile, new Rect(128, 128, 128, 128));
    SpriteBatch.end();
    Renderer.popViewport();
    Renderer.popRenderTarget();

    // Update our dynamic texture.
    for (var y = 0; y < 128; ++y)
    {
        var k = y * 4;
        dynamicData[k + 0] = y - dynamicAnim;
        dynamicData[k + 1] = y + dynamicAnim;
        dynamicData[k + 2] = y;
        dynamicData[k + 3] = 255;
    }
    dynamic.setData(dynamicData);

    // Draw out resulted textures
    Renderer.setFilterMode(FilterMode.NEAREST);
    SpriteBatch.begin(Matrix.createTranslation(new Vector3(80 + 64, 212, 0)));

    SpriteBatch.drawRect(textureFromFile, new Rect(0, 0, 128, 128));
    SpriteBatch.drawRect(textureFromData, new Rect(128, 0, 128, 128));
    SpriteBatch.drawRect(renderTarget, new Rect(256, 0, 128, 128));
    SpriteBatch.drawRect(dynamic, new Rect(384, 0, 128, 128));
    
    SpriteBatch.drawText(font, "From File", new Vector2(64, 140), Vector2.CENTER);
    SpriteBatch.drawText(font, "From Data", new Vector2(64 + 128, 140), Vector2.CENTER);
    SpriteBatch.drawText(font, "Render Target", new Vector2(64 + 256, 140), Vector2.CENTER);
    SpriteBatch.drawText(font, "Dynamic", new Vector2(64 + 384, 140), Vector2.CENTER);
    
    SpriteBatch.end();
}
