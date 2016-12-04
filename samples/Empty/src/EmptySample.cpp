// Oak Nut include
#include <onut/Renderer.h>
#include <onut/Settings.h>

#if defined(__unix__)
#include <GLES/gl.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <onut/Texture.h>
#include <onut/VertexBuffer.h>
#include <onut/IndexBuffer.h>
#include <onut/SpriteBatch.h>
#endif

void initSettings()
{
    oSettings->setGameName("Empty Sample");
}

#if defined(__unix__)
struct sVertex
{
    float x, y;
    float u, v;
    float r, g, b, a;
};

sVertex vertices[4] = {
    {0, 0, 0, 0, 1, 1, 1, 1},
    {0, 100, 0, 1, 1, 1, 1, 1},
    {100, 100, 1, 1, 1, 1, 1, 1},
    {100, 0, 1, 0, 1, 1, 1, 1}
};

uint16_t indices[6] = {
    0, 1, 2, 0, 2, 3
};

OTextureRef pTexture1;
OTextureRef pTexture2;
OIndexBufferRef ibo;
OVertexBufferRef vbo;
#endif

void init()
{
#if defined(__unix__)
    // Build VBO
 /*   glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sVertex) * 4, vertices, GL_DYNAMIC_DRAW);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sVertex) * 4, vertices);
    
    // Build IBO
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, indices, GL_STATIC_DRAW);
    //glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint16_t) * 6, indices);*/
    
    ibo = OIndexBuffer::createStatic(indices, sizeof(uint16_t) * 6);
    vbo = OVertexBuffer::createStatic(vertices, sizeof(sVertex) * 4);
    
    // Create test 2x2 textures
    uint32_t textureData1[4] = {
        0xFFFFFFFF,
        0xFF888888,
        0xFF888888,
        0xFFFFFFFF
    };
    pTexture1 = OTexture::createFromData((uint8_t*)textureData1, Point(2, 2), false);
    uint32_t textureData2[4] = {
        0xFFFF00FF,
        0xFF888888,
        0xFF888888,
        0xFFFF00FF
    };
    pTexture2 = OTexture::createFromData((uint8_t*)textureData2, Point(2, 2), false);
#endif
}

void update()
{
}

void render()
{
    // Clear
    oRenderer->clear(OColorHex(1d232d));
    
#if defined(__unix__)
    oSpriteBatch->begin();
    oSpriteBatch->changeFiltering(onut::sample::Filtering::Linear);
    oSpriteBatch->drawRect(pTexture1, Rect(0, 0, 100, 100), Color(1, 1, 1, 1));
    oSpriteBatch->changeFiltering(onut::sample::Filtering::Nearest);
    oSpriteBatch->drawRect(pTexture2, Rect(300, 0, 100, 100), Color(1, 1, 1, 1));
    oSpriteBatch->end();
#endif
}

void postRender()
{
}
