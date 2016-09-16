var texture = getTexture("onutLogo.png");

var vertexData = new Float32Array([
    0, 0, 0, 0, 1, 0, 0, 1, //(x, y, u, v, r, g, b, a)
    0, 100, 0, 2, 0, 1, 0, 1,
    100, 0, 1, 0, 0, 0, 1, 1,
    100, 100, 1, 2, 1, 1, 0, 1
]);
var vertexBuffer = VertexBuffer.createStatic(vertexData);

var indexData = new Uint16Array([0, 1, 3, 0, 3, 2]);
var indexBuffer = IndexBuffer.createStatic(indexData);

function render()
{
    Renderer.clear(Color.BLACK);
    Renderer.setupFor2D();

    Renderer.setTexture(texture, 0);

    Renderer.setVertexBuffer(vertexBuffer);
    Renderer.setIndexBuffer(indexBuffer);

    Renderer.drawIndexed(6);
}
