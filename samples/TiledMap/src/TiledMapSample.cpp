// Oak Nut include
#include <onut/PrimitiveBatch.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/TiledMap.h>

OTiledMapRef pTiledMap;
OTiledMap::Path path1;
OTiledMap::Path path2;
OTiledMap::Path path3;
OTiledMap::Path path4;

void initSettings()
{
    oSettings->setGameName("Tiled Map Sample");
    oSettings->setResolution(Point(1024, 768));
}

void init()
{
    pTiledMap = OGetTiledMap("sample.tmx");
    pTiledMap->generateCollisions("collisions");
    path1 = pTiledMap->getPath(Point(13, 27), Point(6, 12), OTiledMap::PATH_ALLOW_DIAGONAL | OTiledMap::PATH_CROSS_CORNERS);
    path2 = pTiledMap->getPath(Point(13, 27), Point(6, 12), 0);
    path3 = pTiledMap->getPath(Point(13, 27), Point(6, 12), OTiledMap::PATH_ALLOW_DIAGONAL);
    path4 = pTiledMap->getPath(Point(13, 27), Point(1, 30), OTiledMap::PATH_ALLOW_DIAGONAL | OTiledMap::PATH_CROSS_CORNERS); // Impossible, will get closest
}

void update()
{
}

void drawPath(const OTiledMap::Path& path, const Matrix& transform, const Color& color)
{
    auto tileSize = (float)pTiledMap->getTileSize();
    auto halfTileSize = tileSize * .5f;

    oPrimitiveBatch->begin(OPrimitiveLineStrip, nullptr, transform);
    for (const auto& p : path)
    {
        oPrimitiveBatch->draw(Vector2((float)p.x * tileSize + halfTileSize, (float)p.y * tileSize + halfTileSize), color);
    }
    oPrimitiveBatch->end();
}

void render()
{
    auto transform = Matrix::Create2DTranslationZoom(OScreenf, { 512, 384 });

    pTiledMap->setTransform(transform);
    pTiledMap->render();

    drawPath(path1, transform, Color(1, 0, 0));
    drawPath(path2, transform, Color(0, 1, 0));
    drawPath(path3, transform, Color(0, 0, 1));
    drawPath(path4, transform, Color(1, 0, 1));
}

void postRender()
{
}

void renderUI()
{
}
