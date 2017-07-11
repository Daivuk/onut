// Onut
#include <onut/ContentManager.h>
#include <onut/Files.h>
#include <onut/Settings.h>

// Private
#include "RendererGL.h"
#include "TextureGL.h"

// Third party
#include <lodepng/LodePNG.h>

// STL
#include <cassert>
#include <vector>

namespace onut
{
    OTextureRef Texture::createRenderTarget(const Point& size, bool willUseFX)
    {
        auto pRet = std::shared_ptr<TextureGL>(new TextureGL());
        pRet->m_size = size;
        pRet->m_type = Type::RenderTarget;
        
        GLuint frameBuffer;
        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        
        GLuint handle;
        glGenTextures(1, &handle);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, handle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, handle, 0);
        
        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        
        pRet->m_frameBuffer = frameBuffer;
        pRet->m_handle = handle;
        
        // Because opengl uses a global state and its dumb as fuck
        oRenderer->renderStates.renderTarget.forceDirty();
        oRenderer->renderStates.textures[0].forceDirty();

        if (willUseFX)
        {
            pRet->createFrameBuffer(pRet->m_handleFx, pRet->m_frameBufferFx);
        }
        
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
        auto pRet = std::shared_ptr<TextureGL>(new TextureGL());
        
        GLuint handle;
        glGenTextures(1, &handle);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, handle);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        // Because opengl uses a global state and its dumb as fuck
        oRenderer->renderStates.textures[0].forceDirty();

        pRet->m_type = Type::Dynamic;
        pRet->m_size = size;
        pRet->m_handle = handle;
        
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
        auto pRet = std::shared_ptr<TextureGL>(new TextureGL());
        pRet->m_isDirty = true;
        pRet->m_dirtyData = std::vector<uint8_t>(pData, pData + (size.x * size.y * 4));
        
        // Because opengl uses a global state and its dumb as fuck
        oRenderer->renderStates.textures[0].forceDirty();

        pRet->m_type = Type::Static;
        pRet->m_size = size;
        
        return pRet;
    }

    void TextureGL::setData(const uint8_t* pData)
    {
        assert(isDynamic()); // Only dynamic texture can be set data (But this can actually work in OpenGL)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_handle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
        
        // Because opengl uses a global state and its dumb as fuck
        oRenderer->renderStates.textures[0].forceDirty();
    }

    TextureGL::~TextureGL()
    {
        if (m_handle)
        {
            glDeleteTextures(1, &m_handle);
        }
        if (m_frameBuffer)
        {
            glDeleteFramebuffers(1, &m_frameBuffer);
        }
        if (m_handleFx)
        {
            glDeleteTextures(1, &m_handleFx);
        }
        if (m_frameBufferFx)
        {
            glDeleteFramebuffers(1, &m_frameBufferFx);
        }
    }

    void TextureGL::resizeTarget(const Point& size)
    {
        if (m_size == size) return;

        m_size = size;

        // Release
        assert(false);
    }

    void TextureGL::clearRenderTarget(const Color& color)
    {
        oRenderer->clear(color);
    }

    void TextureGL::createFrameBuffer(GLuint& otherHandle, GLuint& otherFrameBuffer)
    {
        auto pRet = std::shared_ptr<TextureGL>(new TextureGL());

        GLuint frameBuffer;
        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

        GLuint handle;
        glGenTextures(1, &handle);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, handle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, handle, 0);

        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        otherFrameBuffer = frameBuffer;
        otherHandle = handle;

        //// Because opengl uses a global state and its dumb as fuck
        //oRenderer->renderStates.renderTarget.forceDirty();
        //oRenderer->renderStates.textures[0].forceDirty();
    }

    void TextureGL::blur(float amount)
    {
        if (!m_frameBuffer) return; // Not a render target
        if (!m_frameBufferFx)
        {
            createFrameBuffer(m_handleFx, m_frameBufferFx);
        }

        oRenderer->renderStates.viewport.push({ 0, 0, m_size.x, m_size.y });
        oRenderer->renderStates.renderTarget.push(shared_from_this());
        oRenderer->renderStates.textures[0].push(shared_from_this());

        int i = 0;
        while (amount > 0.f)
        {
            oRenderer->setKernelSize({
                1.f / static_cast<float>(m_size.x) * ((float)i + amount) / 6,
                1.f / static_cast<float>(m_size.y) * ((float)i + amount) / 6
            });
            amount -= 6.f;

            oRenderer->renderStates.renderTarget.forceDirty();
            oRenderer->renderStates.textures[0].forceDirty();
            std::swap(m_frameBuffer, m_frameBufferFx);
            clearRenderTarget(Color::Transparent);
            oRenderer->drawBlurH();
            std::swap(m_frameBuffer, m_frameBufferFx);

            oRenderer->renderStates.renderTarget.forceDirty();
            oRenderer->renderStates.textures[0].forceDirty();
            std::swap(m_handle, m_handleFx);
            clearRenderTarget(Color::Transparent);
            oRenderer->drawBlurV();
            std::swap(m_handle, m_handleFx);

            i += 1;
        }

        oRenderer->renderStates.viewport.pop();
        oRenderer->renderStates.renderTarget.pop();
        oRenderer->renderStates.textures[0].pop();
    }
    
    void TextureGL::sepia(const Vector3& tone, float saturation, float sepiaAmount)
    {
        if (!m_frameBuffer) return; // Not a render target
        if (!m_frameBufferFx)
        {
            createFrameBuffer(m_handleFx, m_frameBufferFx);
        }

        oRenderer->renderStates.viewport.push({ 0, 0, m_size.x, m_size.y });
        oRenderer->renderStates.renderTarget.push(shared_from_this());
        oRenderer->renderStates.renderTarget.forceDirty();
        oRenderer->renderStates.textures[0].push(shared_from_this());

        std::swap(m_frameBuffer, m_frameBufferFx);
        clearRenderTarget(Color::Transparent);

        oRenderer->setSepia(tone, saturation, sepiaAmount);
        oRenderer->drawSepia();
        std::swap(m_handle, m_handleFx);

        oRenderer->renderStates.viewport.pop();
        oRenderer->renderStates.renderTarget.pop();
        oRenderer->renderStates.renderTarget.forceDirty();
        oRenderer->renderStates.textures[0].pop();
    }

    void TextureGL::crt()
    {
        if (!m_frameBuffer) return; // Not a render target
        if (!m_frameBufferFx)
        {
            createFrameBuffer(m_handleFx, m_frameBufferFx);
        }

        oRenderer->renderStates.viewport.push({ 0, 0, m_size.x, m_size.y });
        oRenderer->renderStates.renderTarget.push(shared_from_this());
        oRenderer->renderStates.renderTarget.forceDirty();
        oRenderer->renderStates.textures[0].push(shared_from_this());

        std::swap(m_frameBuffer, m_frameBufferFx);
        clearRenderTarget(Color::Transparent);

        oRenderer->setCRT(getSizef());
        oRenderer->drawCRT();
        std::swap(m_handle, m_handleFx);

        oRenderer->renderStates.viewport.pop();
        oRenderer->renderStates.renderTarget.pop();
        oRenderer->renderStates.renderTarget.forceDirty();
        oRenderer->renderStates.textures[0].pop();
    }

    void TextureGL::cartoon(const Vector3& tone)
    {
        if (!m_frameBuffer) return; // Not a render target
        if (!m_frameBufferFx)
        {
            createFrameBuffer(m_handleFx, m_frameBufferFx);
        }

        oRenderer->renderStates.viewport.push({ 0, 0, m_size.x, m_size.y });
        oRenderer->renderStates.renderTarget.push(shared_from_this());
        oRenderer->renderStates.renderTarget.forceDirty();
        oRenderer->renderStates.textures[0].push(shared_from_this());

        std::swap(m_frameBuffer, m_frameBufferFx);
        clearRenderTarget(Color::Transparent);

        oRenderer->setCartoon(tone);
        oRenderer->drawCartoon();
        std::swap(m_handle, m_handleFx);

        oRenderer->renderStates.viewport.pop();
        oRenderer->renderStates.renderTarget.pop();
        oRenderer->renderStates.renderTarget.forceDirty();
        oRenderer->renderStates.textures[0].pop();
    }

    void TextureGL::vignette(float amount)
    {
        if (!m_frameBuffer) return; // Not a render target
        if (!m_frameBufferFx)
        {
            createFrameBuffer(m_handleFx, m_frameBufferFx);
        }

        oRenderer->renderStates.viewport.push({ 0, 0, m_size.x, m_size.y });
        oRenderer->renderStates.renderTarget.push(shared_from_this());
        oRenderer->renderStates.renderTarget.forceDirty();
        oRenderer->renderStates.textures[0].push(shared_from_this());

        std::swap(m_frameBuffer, m_frameBufferFx);
        clearRenderTarget(Color::Transparent);

        oRenderer->setVignette({
            1.f / static_cast<float>(m_size.x),
            1.f / static_cast<float>(m_size.y)
        }, amount);
        oRenderer->drawVignette();
        std::swap(m_handle, m_handleFx);

        oRenderer->renderStates.viewport.pop();
        oRenderer->renderStates.renderTarget.pop();
        oRenderer->renderStates.renderTarget.forceDirty();
        oRenderer->renderStates.textures[0].pop();
    }

    GLuint TextureGL::getHandle() const
    {
        if (m_isDirty)
        {
            m_isDirty = false;

            GLuint handle;
            glGenTextures(1, &handle);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, handle);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_dirtyData.data());
            m_dirtyData.clear();

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            m_handle = handle;
        }
        return m_handle;
    }

    GLuint TextureGL::getFramebuffer() const
    {
        return m_frameBuffer;
    }
}
