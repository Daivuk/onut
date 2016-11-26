#pragma once
#if defined(__unix__)
// Onut
#include <onut/Point.h>
#include <onut/Renderer.h>

// Third party
#include <GLES/gl.h>
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

        // For effects
        void setKernelSize(const Vector2& kernelSize);
        void setSepia(const Vector3& tone = Vector3(1.40f, 1.10f, 0.90f), // 0 - 2.55
                      float saturation = .25f, // 0 - 1
                      float sepiaAmount = .75f); // 0 - 1
        void setCRT(const Vector2& kernelSize);
        void setCartoon(const Vector3& tone);
        void setVignette(const Vector2& kernelSize, float amount = .5f);

        void applyRenderStates() override;
        void init(const OWindowRef& pWindow) override;

    private:
        void createDevice(const OWindowRef& pWindow);
        void createRenderTarget();
        void createRenderStates();
        void createUniforms();

        // Device stuff
        EGLDisplay m_display;
        EGLSurface m_surface;
        EGLContext m_context;
        EGL_DISPMANX_WINDOW_T m_nativeWindow;

        // Render target
        Point m_resolution;

        // Render states

        // Constant buffers

     //   OTextureRef m_boundTextures[RenderStates::MAX_TEXTURES];
    };
};

#endif // __unix__
