#ifndef RENDERERGLES2_H_INCLUDED
#define RENDERERGLES2_H_INCLUDED

#if defined(__unix__)
// Onut
#include <onut/Point.h>
#include <onut/Renderer.h>

// Third party
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(RendererGLES2);

namespace onut
{
    class RendererGLES2 final : public Renderer
    {
    public:
        RendererGLES2(const OWindowRef& pWindow);
        ~RendererGLES2();

        void clear(const Color& color = {.25f, .5f, 1, 1}) override;
        void clearDepth() override;

        void beginFrame();
        void endFrame();

        void draw(uint32_t vertexCount) override;
        void drawIndexed(uint32_t indexCount) override;

        Point getTrueResolution() const override;
        void onResize(const Point& newSize);

        EGLDisplay getDisplay() const;
        EGLSurface getSurface() const;
        EGLContext getContext() const;

        void applyRenderStates() override;
        void init(const OWindowRef& pWindow) override;

    private:
        void createDevice(const OWindowRef& pWindow);
        void createRenderTarget();
        void createRenderStates();
        void createUniforms();

        // Device stuff
        EGL_DISPMANX_WINDOW_T m_nativeWindow;
        DISPMANX_DISPLAY_HANDLE_T m_dispman_display;
        DISPMANX_ELEMENT_HANDLE_T m_dispman_element;
        EGLDisplay m_display;
        EGLSurface m_surface;
        EGLContext m_context;

        // Render target
        Point m_resolution;

        // Render states

        // Constant buffers

     //   OTextureRef m_boundTextures[RenderStates::MAX_TEXTURES];
    };
};

#endif // __unix__

#endif
