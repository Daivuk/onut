#ifndef RENDERERGL_H_INCLUDED
#define RENDERERGL_H_INCLUDED

// Onut
#include <onut/Point.h>
#include <onut/Renderer.h>

// Third party
#include "gl_includes.h"
#if defined(WIN32)
#include <Windows.h>
#endif

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(RendererGL);

namespace onut
{
    class RendererGL final : public Renderer
    {
    public:
        RendererGL(const OWindowRef& pWindow);
        ~RendererGL();

        void clear(const Color& color = {.25f, .5f, 1, 1}) override;
        void clearDepth() override;

        void beginFrame() override;
        void endFrame() override;

        void draw(uint32_t vertexCount) override;
        void drawIndexed(uint32_t indexCount) override;

        Point getTrueResolution() const override;
        void onResize(const Point& newSize) override;

        void applyRenderStates() override;
        void init(const OWindowRef& pWindow) override;

    private:
        void createDevice(const OWindowRef& pWindow);
        void createRenderTarget();
        void createRenderStates();
        void createUniforms();

        // Device stuff
#if defined(WIN32)
        HGLRC m_hRC = nullptr;  // Permanent Rendering Context
        HDC m_hDC = nullptr;  // Private GDI Device Context
        HWND m_hWnd = nullptr; // Holds Our Window Handle
#else
        SDL_Window* m_pSDLWindow = nullptr;
        SDL_GLContext m_glContext;
#endif

        int m_lastVertexAttribCount = 0;

        // Render target
        Point m_resolution;

        // Render states

        // Constant buffers

     //   OTextureRef m_boundTextures[RenderStates::MAX_TEXTURES];
    };
};

#endif
