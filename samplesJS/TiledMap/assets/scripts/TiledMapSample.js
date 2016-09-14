var tiledMap = getTiledMap("sample.tmx");

var cliftSideId = tiledMap.getTileAt(2, 4, 0);
tiledMap.setTileAt(0, 0, 0, cliftSideId);
tiledMap.setTileAt(0, 1, 0, cliftSideId);
tiledMap.setTileAt(0, 2, 0, cliftSideId);
tiledMap.setTileAt(0, 3, 0, cliftSideId);

function render() {
    tiledMap.render();
}
