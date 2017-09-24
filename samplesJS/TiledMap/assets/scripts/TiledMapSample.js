var tiledMap = getTiledMap("sample.tmx");
var transform = Matrix.create2DTranslationZoom(new Vector3(512, 384, 0));
var tileSize = tiledMap.getTileSize();
var halfTileSize = tileSize * .5;

var path1 = tiledMap.getPath(13, 27, 6, 12, PathType.CROSS_CORNERS);
var path2 = tiledMap.getPath(13, 27, 6, 12, PathType.STRAIGHT);
var path3 = tiledMap.getPath(13, 27, 6, 12, PathType.ALLOW_DIAGONAL);

function drawPath(path, color)
{
    PrimitiveBatch.begin(PrimitiveMode.LINE_STRIP, null, transform);
    for (var i = 0; i < path.length; ++i)
    {
        var pos = path[i];
        PrimitiveBatch.draw(pos.mul(tileSize).add(halfTileSize), color);
    }
    PrimitiveBatch.end();
}

function render() 
{
    SpriteBatch.begin(transform);
    tiledMap.render();
    SpriteBatch.end();

    drawPath(path1, new Color(1, 0, 0));
    drawPath(path2, new Color(0, 1, 0));
    drawPath(path3, new Color(0, 0, 1));
}
