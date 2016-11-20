// Oak Nut include
#include <onut/Settings.h>
#include <onut/TiledMap.h>

OTiledMapRef pTiledMap;

void initSettings()
{
    oSettings->setGameName("Tiled Map Sample");
    oSettings->setIsRetroMode(true);
    oSettings->setIsResizableWindow(true);
}

void init()
{
    pTiledMap = OGetTiledMap("sample.tmx");
}

void update()
{
}

void render()
{
    pTiledMap->setTransform(Matrix::CreateTranslation(-375.0f, -250.0f, 0.0f));
    pTiledMap->render();
}

void postRender()
{
}
