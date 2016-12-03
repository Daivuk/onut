// Oak Nut include
#include <onut/Renderer.h>
#include <onut/Settings.h>

#include <GLES/gl.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

void initSettings()
{
    oSettings->setGameName("Empty Sample");
}

struct sVertex
{
    float x, y;
    float u, v;
    float r, g, b, a;
};

sVertex vertices[4] = {
    {0, 0, 0, 0, 1, 0, 0, 1},
    {0, 100, 0, 1, 0, 1, 0, 1},
    {100, 100, 1, 1, 0, 0, 1, 1},
    {100, 0, 1, 0, 1, 1, 0, 1}
};

uint16_t indices[6] = {
    0, 1, 2, 0, 2, 3
};

GLuint vbo;
GLuint ibo;
GLuint texture2x2;

GLuint createTextureFromData(uint8_t *pData, int w, int h)
{
    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return texture;
}

void init()
{
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
    
    // Create a test 2x2 texture
    uint32_t textureData[4] = {
        0xFFFFFFFF,
        0xFF888888,
        0xFF888888,
        0xFFFFFFFF
    };
    texture2x2 = createTextureFromData((uint8_t*)textureData, 2, 2);
}

void update()
{
}

void render()
{
    // Clear
    oRenderer->clear(OColorHex(1d232d));
    
    // Setup states
 /*   glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glViewport(0, 0, OScreenWf, OScreenHf);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float proj[16] = {
        2.f / OScreenWf, 0.f,                0.f,                0.f,
        0.f,           -2.f / OScreenHf,     0.f,                0.f,
        0.f,            0.f,               -0.000500500493f,    0.f,
       -1.f,            1.f,                0.5f,               1.f};
    glMultMatrixf(proj);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();*/
    
    // Draw immediate
  /*  glVertexPointer(2, GL_FLOAT, sizeof(sVertex), vertices);
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(sVertex), (float*)vertices + 2);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glColorPointer(4, GL_FLOAT, sizeof(sVertex), (float*)vertices + 4);
    glEnableClientState(GL_COLOR_ARRAY);
    glDrawArrays(GL_TRIANGLES, 0, 3);*/
    
    // Draw VBO
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture2x2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glVertexPointer(2, GL_FLOAT, sizeof(sVertex), NULL);
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(sVertex), (float*)(sizeof(GL_FLOAT) * 2));
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glColorPointer(4, GL_FLOAT, sizeof(sVertex), (float*)(sizeof(GL_FLOAT) * 4));
    glEnableClientState(GL_COLOR_ARRAY);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
}

void postRender()
{
}
