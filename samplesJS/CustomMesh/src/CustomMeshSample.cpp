// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/IndexBuffer.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Texture.h>
#include <onut/VertexBuffer.h>

void init();
void update();
void render();

OVertexBufferRef pVertexBuffer;
OIndexBufferRef pIndexBuffer;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Custom Mesh Sample");
    ORun(init, update, render);
}

void init()
{
    // Create our vertex buffer
    struct Vertex
    {
        Vector2 position;
        Vector2 uv;
        Color color;
    };
    Vertex vertices[4] = {
        {Vector2(0, 0), Vector2(0, 0), Color(1, 0, 0, 1)},
        {Vector2(0, 100), Vector2(0, 1), Color(0, 1, 0, 1)},
        {Vector2(100, 0), Vector2(1, 0), Color(0, 0, 1, 1)},
        {Vector2(100, 100), Vector2(1, 1), Color(1, 1, 0, 1)}
    };
    pVertexBuffer = OVertexBuffer::createStatic(vertices, sizeof(vertices));

    // Create our index buffer
    uint16_t indices[6] = {0, 1, 3, 0, 3, 2};
    pIndexBuffer = OIndexBuffer::createStatic(indices, sizeof(indices));
}

void update()
{
}

void render()
{
    oRenderer->clear({0, 0, 0, 1});
    oRenderer->setupFor2D();

    oRenderer->renderStates.textures[0] = OGetTexture("onutLogo.png");
    oRenderer->renderStates.vertexBuffer = pVertexBuffer;
    oRenderer->renderStates.indexBuffer = pIndexBuffer;

    oRenderer->drawIndexed(6);
}
