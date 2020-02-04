var vertexAnim = 0;
var pixelAnim = 0;

var texture0 = getTexture("texture0.png");
var texture1 = getTexture("texture1.png");

var vertexShader = getShader("sample.vs");
var pixelShader = getShader("sample.ps");
var mrtPS = getShader("mrt.ps");

var mrt = [
    Texture.createRenderTarget(new Vector2(128, 128)),
    Texture.createRenderTarget(new Vector2(128, 128)),
    Texture.createRenderTarget(new Vector2(128, 128)),
    Texture.createRenderTarget(new Vector2(128, 128))
]

function update(dt) {
    vertexAnim += dt;
    pixelAnim += dt * .5;
}

function render() {
    var hres = Renderer.getResolution().div(2);

    // Clear
    Renderer.clear(Color.fromHexRGB(0x1d232d));

    SpriteBatch.begin();

    // Set custom shaders
    Renderer.pushVertexShader(vertexShader);
    Renderer.setPixelShader(pixelShader);

    // Bind both textures directly to the renderer
    Renderer.setTexture(texture0, 0);
    Renderer.setTexture(texture1, 1);

    // Set uniforms
    vertexShader.setNumber("anim", vertexAnim);
    pixelShader.setNumber("anim", pixelAnim);

    // Draw sprite
    SpriteBatch.drawSprite(texture0, new Vector2(hres.x - 150, hres.y));

    SpriteBatch.end();

    // MRT
    for (var i = 0; i < 4; ++i) Renderer.pushRenderTarget(mrt[i], i);
    SpriteBatch.begin();
    Renderer.setPixelShader(mrtPS);
    SpriteBatch.drawRect(texture0, new Rect(0, 0, 128, 128));
    SpriteBatch.end();
    for (var i = 0; i < 4; ++i) Renderer.popRenderTarget(i);

    SpriteBatch.begin();
    SpriteBatch.drawSprite(mrt[0], new Vector2(hres.x + 150, hres.y - 64));
    SpriteBatch.drawSprite(mrt[1], new Vector2(hres.x + 150+128, hres.y - 64));
    SpriteBatch.drawSprite(mrt[2], new Vector2(hres.x + 150, hres.y + 64));
    SpriteBatch.drawSprite(mrt[3], new Vector2(hres.x + 150+128, hres.y + 64));
    SpriteBatch.end();
}
