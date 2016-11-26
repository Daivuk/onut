#if defined(__unix__)
// Onut
#include <onut/ContentManager.h>
#include <onut/Files.h>
#include <onut/Settings.h>

// Private
#include "RendererGLES2.h"
#include "TextureGLES2.h"

// Third party
#include <lodepng/LodePNG.h>

// STL
#include <cassert>
#include <vector>

namespace onut
{
    OTextureRef Texture::createRenderTarget(const Point& size, bool willUseFX)
    {
        auto pRet = std::shared_ptr<TextureGLES2>(new TextureGLES2());
        pRet->m_size = size;
        assert(false);
        pRet->m_type = Type::RenderTarget;
        return pRet;
    }

    OTextureRef Texture::createScreenRenderTarget(bool willBeUsedInEffects)
    {
        Point res = oRenderer->getTrueResolution();
        if (oSettings->getIsRetroMode())
        {
            res = oSettings->getRetroResolution();
        }

        auto pRet = createRenderTarget(res, willBeUsedInEffects);
        if (pRet)
        {
            pRet->m_isScreenRenderTarget = true;
        }
        pRet->m_type = Type::ScreenRenderTarget;
        return pRet;
    }

    OTextureRef Texture::createDynamic(const Point& size)
    {
        auto pRet = std::shared_ptr<TextureGLES2>(new TextureGLES2());
        assert(false);
        pRet->m_type = Type::Dynamic;
        return pRet;
    }

    OTextureRef Texture::createFromFile(const std::string& filename, const OContentManagerRef& pContentManager, bool generateMipmaps)
    {
        std::vector<uint8_t> image; //the raw pixels (holy crap that must be slow)
        unsigned int w, h;
        auto ret = lodepng::decode(image, w, h, filename);
        assert(!ret);
        Point size{static_cast<int>(w), static_cast<int>(h)};

        // Pre multiplied
        uint8_t* pImageData = &(image[0]);
        auto len = size.x * size.y;
        for (decltype(len) i = 0; i < len; ++i, pImageData += 4)
        {
            pImageData[0] = pImageData[0] * pImageData[3] / 255;
            pImageData[1] = pImageData[1] * pImageData[3] / 255;
            pImageData[2] = pImageData[2] * pImageData[3] / 255;
        }

        auto pRet = createFromData(image.data(), size, generateMipmaps);
        pRet->setName(onut::getFilename(filename));
        pRet->m_type = Type::Static;
        return pRet;
    }

    OTextureRef Texture::createFromFileData(const uint8_t* pData, uint32_t dataSize, bool generateMipmaps)
    {
        std::vector<uint8_t> image; //the raw pixels (holy crap that must be slow)
        unsigned int w, h;
        lodepng::State state;
        auto ret = lodepng::decode(image, w, h, state, pData, dataSize);
        assert(!ret);
        Point size{static_cast<int>(w), static_cast<int>(h)};

        // Pre multiplied
        uint8_t* pImageData = image.data();
        auto len = size.x * size.y;
        for (int i = 0; i < len; ++i, pImageData += 4)
        {
            pImageData[0] = pImageData[0] * pImageData[3] / 255;
            pImageData[1] = pImageData[1] * pImageData[3] / 255;
            pImageData[2] = pImageData[2] * pImageData[3] / 255;
        }

        return createFromData(image.data(), size, generateMipmaps);
    }

    OTextureRef Texture::createFromData(const uint8_t* pData, const Point& size, bool generateMipmaps)
    {
        auto pRet = std::shared_ptr<TextureGLES2>(new TextureGLES2());
        assert(false);
        pRet->m_type = Type::Static;
        return pRet;
    }

    void TextureGLES2::setData(const uint8_t* pData)
    {
        assert(isDynamic()); // Only dynamic texture can be set data
        assert(false);
    }

    TextureGLES2::~TextureGLES2()
    {
    }

    void TextureGLES2::resizeTarget(const Point& size)
    {
        if (m_size == size) return;

        m_size = size;

        // Release
        assert(false);
    }

    void TextureGLES2::clearRenderTarget(const Color& color)
    {
        assert(false);
    }

    void TextureGLES2::blur(float amount)
    {
        assert(false);
    }

    void TextureGLES2::sepia(const Vector3& tone, float saturation, float sepiaAmount)
    {
        assert(false);
    }

    void TextureGLES2::crt()
    {
        assert(false);
    }

    void TextureGLES2::cartoon(const Vector3& tone)
    {
        assert(false);
    }

    void TextureGLES2::vignette(float amount)
    {
        assert(false);
    }
}

#endif