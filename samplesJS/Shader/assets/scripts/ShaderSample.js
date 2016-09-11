var vertexAnim = 0;
var pixelAnim = 0;

var texture0 = getTexture("texture0.png");
var texture1 = getTexture("texture1.png");

var vertexShader = getShader("sample.vs");
var pixelShader = getShader("sample.ps");

function update(dt) {
    vertexAnim += dt;
    pixelAnim += dt * .5;
}

function render() {
    // Clear
    Renderer.clear(Color.fromHexRGB(0x1d232d));

    SpriteBatch.begin();

    // Set custom shaders
    Renderer.setVertexShader(vertexShader);
    Renderer.setPixelShader(pixelShader);

    // Bind both textures directly to the renderer
    Renderer.setTexture(texture0, 0);
    Renderer.setTexture(texture1, 1);

    // Set uniforms
    vertexShader.setNumber("anim", vertexAnim);
    pixelShader.setNumber("anim", pixelAnim);

    // Draw sprite
    SpriteBatch.drawSprite(texture0, Renderer.getResolution().div(2));

    SpriteBatch.end();
}
