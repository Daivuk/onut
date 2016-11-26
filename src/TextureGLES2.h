#pragma once
#if defined(__unix__)
// Onut
#include <onut/Texture.h>

// Third party
//#include <d3d11.h>

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

    protected:
        TextureGLES2() {}

    private:
        friend Texture;
    };
}

#endif
