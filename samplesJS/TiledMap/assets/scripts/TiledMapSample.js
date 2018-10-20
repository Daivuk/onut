var tiledMap = getTiledMap("sample.tmx");
var ZOOMED_TRANSFORM = Matrix.create2DTranslationZoom(new Vector3(512, 384, 0), 2);
var tileSize = tiledMap.getTileSize();
var halfTileSize = tileSize * .5;
var shader = getShader("sample.ps");

var path1 = tiledMap.getPath(13, 27, 6, 12, PathType.CROSS_CORNERS);
var path2 = tiledMap.getPath(13, 27, 6, 12, PathType.STRAIGHT);
var path3 = tiledMap.getPath(13, 27, 6, 12, PathType.ALLOW_DIAGONAL);

var OPTION_NORMAL = 0;
var OPTION_ZOOMED = 1;
var OPTION_SHADER = 2;
var option = OPTION_NORMAL;

function drawPath(path, color, transform)
{
    PrimitiveBatch.begin(PrimitiveMode.LINE_STRIP, null, transform);
    for (var i = 0; i < path.length; ++i)
    {
        var pos = path[i];
        PrimitiveBatch.draw(pos.mul(tileSize).add(halfTileSize), color);
    }
    PrimitiveBatch.end();
}

function update(dt)
{

}

function render() 
{
    var transform = Matrix.IDENTITY;

    // Render full map
    if (option == OPTION_ZOOMED)
    {
        transform = ZOOMED_TRANSFORM;

        // By wraping a sprite batch around it, we can apply transform
        SpriteBatch.begin(transform);
        tiledMap.render();
        SpriteBatch.end();
    }
    else if (option == OPTION_SHADER)
    {
        // By wraping a sprite batch around it, we can apply shaders
        SpriteBatch.begin(transform, shader);
        tiledMap.renderLayer(0);
        SpriteBatch.end();

        tiledMap.renderLayer(1);
        tiledMap.renderLayer(2);
        tiledMap.renderLayer(3);
    }
    else
    {
        // Simple map render, no need to activate the sprite batch
        tiledMap.render();
    }

    // Draw paths
    drawPath(path1, new Color(1, 0, 0), transform);
    drawPath(path2, new Color(0, 1, 0), transform);
    drawPath(path3, new Color(0, 0, 1), transform);
}

function renderUI()
{
    if (GUI.begin("Options"))
    {
        option = GUI.radioButton("Normal", option, 0);
        option = GUI.radioButton("Zoomed", option, 1);
        option = GUI.radioButton("Shaders to bottom layer", option, 2);
    }
    GUI.end();
}
