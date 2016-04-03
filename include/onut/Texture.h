#pragma once
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
    class Texture : public Resource, public std::enable_shared_from_this<Texture>
    {
    public:
        static OTextureRef createFromFile(const std::string& filename, const OContentManagerRef& pContentManager = nullptr, bool generateMipmaps = oGenerateMipmaps);
        static OTextureRef createFromFileData(const uint8_t* pData, uint32_t size, bool generateMipmaps = oGenerateMipmaps);
        static OTextureRef createFromData(const uint8_t* pData, const Point& size, bool generateMipmaps = oGenerateMipmaps);
        static OTextureRef createRenderTarget(const Point& size, bool willBeUsedInEffects = false);
        static OTextureRef createScreenRenderTarget(bool willBeUsedInEffects = false);
        static OTextureRef createDynamic(const Point& size);

        virtual ~Texture();

        const Point& getSize() const;
        Vector2 getSizef() const;
        void bind(int slot = 0);
        bool isRenderTarget() const;
        bool isDynamic() const;

        virtual void clearRenderTarget(const Color& color) = 0;

        // Apply effects. It will only work if the texture is a render target
        virtual void blur(float amount = 16.f) = 0; // Blur radius
        virtual void sepia(const Vector3& tone = Vector3(1.40f, 1.10f, 0.90f), // 0 - 2.55
                           float saturation = 0, // 0 - 1
                           float sepiaAmount = .75f) = 0; // 0 - 1
        virtual void crt() = 0;
        virtual void cartoon(const Vector3& tone = Vector3(2, 5, 1)) = 0; // Values range from 1 to infinite
        virtual void vignette(float amount = .5f) =0 ; // 0 - 1

        virtual void setData(const uint8_t* pData) = 0;
        virtual void resizeTarget(const Point& size) = 0;

    protected:
        Texture() {}

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
    };
}

OTextureRef OGetTexture(const std::string& name);
