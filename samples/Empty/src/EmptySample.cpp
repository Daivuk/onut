// Oak Nut include
#include <onut/Renderer.h>
#include <onut/Settings.h>

#if defined(__unix__)
#include <GLES/gl.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <onut/Texture.h>
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

GLuint vbo;
GLuint ibo;
OTextureRef pTexture1;
OTextureRef pTexture2;
#endif

void init()
{
#if defined(__unix__)
    // Build VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sVertex) * 4, vertices, GL_DYNAMIC_DRAW);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sVertex) * 4, vertices);
    
    // Build IBO
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, indices, GL_STATIC_DRAW);
    //glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint16_t) * 6, indices);
    
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
    oRenderer->renderStates.sampleFiltering = onut::sample::Filtering::Nearest;
    oRenderer->renderStates.world = Matrix::CreateTranslation(0, 0, 0);
    oRenderer->renderStates.textures[0] = pTexture1;
    oRenderer->applyRenderStates();
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glVertexPointer(2, GL_FLOAT, sizeof(sVertex), NULL);
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(sVertex), (float*)(sizeof(GL_FLOAT) * 2));
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glColorPointer(4, GL_FLOAT, sizeof(sVertex), (float*)(sizeof(GL_FLOAT) * 4));
    glEnableClientState(GL_COLOR_ARRAY);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
    
    oRenderer->renderStates.sampleFiltering = onut::sample::Filtering::Linear;
    oRenderer->renderStates.world = Matrix::CreateTranslation(300, 0, 0);
    oRenderer->renderStates.textures[0] = pTexture2;
    oRenderer->applyRenderStates();
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glVertexPointer(2, GL_FLOAT, sizeof(sVertex), NULL);
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(sVertex), (float*)(sizeof(GL_FLOAT) * 2));
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glColorPointer(4, GL_FLOAT, sizeof(sVertex), (float*)(sizeof(GL_FLOAT) * 4));
    glEnableClientState(GL_COLOR_ARRAY);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
#endif
}

void postRender()
{
}
