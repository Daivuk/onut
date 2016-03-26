// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/onut.h>
#include <onut/PrimitiveBatch.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Texture.h>

void init();
void update();
void render();

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Primitives Sample");
    ORun(init, update, render);
}

void init()
{
}

void update()
{
}

void render()
{
    oRenderer->clear({0, 0, 0, 1});

    // Lines
    oPrimitiveBatch->begin(OPrimitiveLineStrip);
    oPrimitiveBatch->draw(Vector2(10, 10), Color(1, 0, 0, 1));
    oPrimitiveBatch->draw(Vector2(100, 100), Color(1, 1, 0, 1));
    oPrimitiveBatch->draw(Vector2(75, 200), Color(0, 1, 0, 1));
    oPrimitiveBatch->draw(Vector2(25, 50), Color(0, 0, 1, 1));
    oPrimitiveBatch->end();

    // Points
    oPrimitiveBatch->begin(OPrimitivePointList);
    oPrimitiveBatch->draw(Vector2(110, 10), Color(1, 0, 0, 1));
    oPrimitiveBatch->draw(Vector2(110, 50), Color(1, 1, 0, 1));
    oPrimitiveBatch->draw(Vector2(150, 50), Color(0, 1, 0, 1));
    oPrimitiveBatch->draw(Vector2(150, 10), Color(0, 0, 1, 1));
    oPrimitiveBatch->end();

    // Triangles
    oPrimitiveBatch->begin(OPrimitiveTriangleList);
    oPrimitiveBatch->draw(Vector2(250, 10), Color(1, 0, 0, 1));
    oPrimitiveBatch->draw(Vector2(200, 50), Color(1, 1, 0, 1));
    oPrimitiveBatch->draw(Vector2(300, 50), Color(0, 1, 0, 1));
    oPrimitiveBatch->draw(Vector2(250, 110), Color(0, 0, 1, 1));
    oPrimitiveBatch->draw(Vector2(200, 150), Color(1, 0, 1, 1));
    oPrimitiveBatch->draw(Vector2(300, 150), Color(0, 1, 1, 1));
    oPrimitiveBatch->end();

    // Textured
    oPrimitiveBatch->begin(OPrimitiveTriangleList, OGetTexture("onutLogo.png"));
    oPrimitiveBatch->draw(Vector2(350, 10), Color::White, Vector2(.5f, 0));
    oPrimitiveBatch->draw(Vector2(300, 100), Color::White, Vector2(0, 1));
    oPrimitiveBatch->draw(Vector2(400, 100), Color::White, Vector2(1, 1));
    oPrimitiveBatch->end();
}
