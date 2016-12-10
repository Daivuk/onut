#ifndef TEXTUREGLES2_H_INCLUDED
#define TEXTUREGLES2_H_INCLUDED

#if defined(__unix__)
// Onut
#include <onut/SampleMode.h>
#include <onut/Texture.h>

// Third party
#include <GLES/gl.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(TextureGLES2);

extern bool oGenerateMipmaps;

namespace onut
{
    class TextureGLES2 final : public Texture
    {
    public:
        ~TextureGLES2();

        void clearRenderTarget(const Color& color) override;

        // Apply effects. It will only work if the texture is a render target
        void blur(float amount = 16.f) override; // Blur radius
        void sepia(const Vector3& tone = Vector3(1.40f, 1.10f, 0.90f), // 0 - 2.55
                           float saturation = 0, // 0 - 1
                           float sepiaAmount = .75f) override; // 0 - 1
        void crt() override;
        void cartoon(const Vector3& tone = Vector3(2, 5, 1)) override; // Values range from 1 to infinite
        void vignette(float amount = .5f) override; // 0 - 1

        void setData(const uint8_t* pData) override;
        void resizeTarget(const Point& size) override;
        
        GLuint getHandle() const;
        GLuint getFramebuffer() const;
        
        // Renderer need to keep track of the sample states per texture in OpenGL
        sample::Filtering filtering = sample::Filtering::Linear;
        sample::AddressMode addressMode = sample::AddressMode::Wrap;

    protected:
        TextureGLES2() {}

    private:
        friend Texture;
        
        GLuint m_handle = 0;
        GLuint m_frameBuffer = 0;
    };
}

#endif

#endif
