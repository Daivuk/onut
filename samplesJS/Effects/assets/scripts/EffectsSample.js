var landscape = getTexture("landscape.png");
var font = getFont("font.fnt");

var blured = Texture.createRenderTarget(256);
var sepia = Texture.createRenderTarget(256);
var crt = Texture.createRenderTarget(256);
var cartoon = Texture.createRenderTarget(256);
var vignette = Texture.createRenderTarget(256);

function drawLandscapeToRenderTarget(renderTarget)
{
    var size = renderTarget.getSize();
    Renderer.pushRenderTarget(renderTarget);
    SpriteBatch.begin();
    SpriteBatch.drawRect(landscape, new Rect(0, 0, size));
    SpriteBatch.end();
    Renderer.popRenderTarget();
}

function render() {
    // Clear
    Renderer.clear(Color.fromHexRGB(0x1d232d));
    
    // Draw landscape to our render targets
    drawLandscapeToRenderTarget(blured);
    drawLandscapeToRenderTarget(sepia);
    drawLandscapeToRenderTarget(crt);
    drawLandscapeToRenderTarget(cartoon);
    drawLandscapeToRenderTarget(vignette);
    
    // Apply effects
    blured.blur();
    sepia.sepia();
    crt.crt();
    cartoon.cartoon();
    vignette.vignette();

    // Draw out resulted textures
    SpriteBatch.begin();

    SpriteBatch.drawRect(landscape, new Rect(0, 0, 256, 256));
    SpriteBatch.drawRect(blured, new Rect(256, 0, 256, 256));
    SpriteBatch.drawRect(sepia, new Rect(512, 0, 256, 256));
    SpriteBatch.drawRect(crt, new Rect(0, 288, 256, 256));
    SpriteBatch.drawRect(cartoon, new Rect(256, 288, 256, 256));
    SpriteBatch.drawRect(vignette, new Rect(512, 288, 256, 256));

    SpriteBatch.drawText(font, "Original", new Vector2(128 + 0, 256 + 8), Vector2.CENTER);
    SpriteBatch.drawText(font, "Blur", new Vector2(128 + 256, 256 + 8), Vector2.CENTER);
    SpriteBatch.drawText(font, "Sepia", new Vector2(128 + 512, 256 + 8), Vector2.CENTER);
    SpriteBatch.drawText(font, "CRT", new Vector2(128 + 0, 288 + 256 + 8), Vector2.CENTER);
    SpriteBatch.drawText(font, "Cartoon", new Vector2(128 + 256, 288 + 256 + 8), Vector2.CENTER);
    SpriteBatch.drawText(font, "Vignette", new Vector2(128 + 512, 288 + 256 + 8), Vector2.CENTER);

    SpriteBatch.end();
}

