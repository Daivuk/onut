#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

// Onut
#include <onut/Maths.h>
#include <onut/Point.h>
#include <onut/Resource.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Texture);
OForwardDeclare(ContentManager);

extern bool oGenerateMipmaps;

namespace onut
{
    enum class RenderTargetFormat
    {
        R8, // normalized in range [0, 1]
        RG8, // normalized in range [0, 1]
        RGBA8, // normalized in range [0, 1]
        R16, // normalized in range [0, 1]
        RG16, // normalized in range [0, 1]
        RGBA16, // normalized in range [0, 1]
        R32, // float
        RG32, // float
        RGBA32, // float
        RGB10A2 // normalized in range [0, 1]
    };

    class Texture : public Resource, public std::enable_shared_from_this<Texture>
    {
    public:
        static OTextureRef createFromFile(const std::string& filename, const OContentManagerRef& pContentManager = nullptr, bool generateMipmaps = oGenerateMipmaps);
        static OTextureRef createFromFileData(const uint8_t* pData, uint32_t size, bool generateMipmaps = oGenerateMipmaps);
        static OTextureRef createFromData(const uint8_t* pData, const Point& size, bool generateMipmaps = oGenerateMipmaps);
        static OTextureRef createRenderTarget(const Point& size, bool willBeUsedInEffects = false, RenderTargetFormat format = onut::RenderTargetFormat::RGBA8);
        static OTextureRef createScreenRenderTarget(bool willBeUsedInEffects = false, RenderTargetFormat format = onut::RenderTargetFormat::RGBA8);
        static OTextureRef createDynamic(const Point& size);

        virtual ~Texture();

        const Point& getSize() const;
        Vector2 getSizef() const;
        void bind(int slot = 0);
        bool isRenderTarget() const;
        bool isDynamic() const;

        virtual void clearRenderTarget(const Color& color) = 0;

        // Apply effects. It will only work if the texture is a render target
        virtual void blur(float amountX = 16.f, float amountY = -1.0f) = 0; // Blur radius
        virtual void sepia(const Vector3& tone = Vector3(1.40f, 1.10f, 0.90f), // 0 - 2.55
                           float saturation = 0, // 0 - 1
                           float sepiaAmount = .75f) = 0; // 0 - 1
        virtual void crt() = 0;
        virtual void cartoon(const Vector3& tone = Vector3(2, 5, 1)) = 0; // Values range from 1 to infinite
        virtual void vignette(float amount = .5f) =0 ; // 0 - 1

        virtual void setData(const uint8_t* pData) = 0;
        virtual void resizeTarget(const Point& size) = 0;

    protected:
        Texture() :
            m_type{}
        {}

        enum class Type
        {
            Static,
            Dynamic,
            RenderTarget,
            ScreenRenderTarget
        };

        Point m_size;
        Type m_type;
        bool m_isScreenRenderTarget = false;
        RenderTargetFormat m_format = onut::RenderTargetFormat::RGBA8;
    };
}

OTextureRef OGetTexture(const std::string& name);

#define OFormatR8 onut::RenderTargetFormat::R8
#define OFormatRG8 onut::RenderTargetFormat::RG8
#define OFormatRGBA8 onut::RenderTargetFormat::RGBA8
#define OFormatR16 onut::RenderTargetFormat::R16
#define OFormatRG16 onut::RenderTargetFormat::RG16
#define OFormatRGBA16 onut::RenderTargetFormat::RGBA16
#define OFormatR32 onut::RenderTargetFormat::R32
#define OFormatRG32 onut::RenderTargetFormat::RG32
#define OFormatRGBA32 onut::RenderTargetFormat::RGBA32
#define OFormatRGB10A2 onut::RenderTargetFormat::RGB10A2

#endif
