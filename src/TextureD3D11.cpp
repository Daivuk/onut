#if defined(WIN32)
// Onut
#include <onut/ContentManager.h>
#include <onut/Files.h>
#include <onut/Settings.h>

// Private
#include "RendererD3D11.h"
#include "TextureD3D11.h"

// Third party
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <json/json.h>

// STL
#include <cassert>
#include <fstream>
#include <vector>

#define MS_COUNT 4
#define MS_QUALITY 0

namespace onut
{
    OTextureRef Texture::createRenderTarget(const Point& size, bool willUseFX, RenderTargetFormat format)
    {
        auto pRet = std::shared_ptr<TextureD3D11>(new TextureD3D11());
        pRet->m_size = size;
        pRet->m_format = format;

        // If anti-aliasing is enabled, create an extra texture to store resolved data
        pRet->multiSampled = oSettings->getAntiAliasing();
        if (pRet->multiSampled)
        {
            auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
            auto pDevice = pRendererD3D11->getDevice();

            D3D11_TEXTURE2D_DESC textureDesc = {0};
            HRESULT result;
            D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
            D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
            memset(&renderTargetViewDesc, 0, sizeof(renderTargetViewDesc));
            memset(&shaderResourceViewDesc, 0, sizeof(shaderResourceViewDesc));

            textureDesc.Width = pRet->m_size.x;
            textureDesc.Height = pRet->m_size.y;
            textureDesc.MipLevels = 1;
            textureDesc.ArraySize = 1;
            textureDesc.Format = pRet->getDXGIFormat();
            textureDesc.SampleDesc.Count = 1;
            textureDesc.SampleDesc.Quality = 0;
            textureDesc.Usage = D3D11_USAGE_DEFAULT;
            textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
            textureDesc.CPUAccessFlags = 0;
            textureDesc.MiscFlags = 0;

            // Create the render target texture.
            result = pDevice->CreateTexture2D(&textureDesc, NULL, &pRet->m_pResolvedTexture);
            if (result != S_OK)
            {
                assert(false && "Failed CreateTexture2D");
                return nullptr;
            }

            // Setup the description of the shader resource view.
            shaderResourceViewDesc.Format = textureDesc.Format;
            shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
            shaderResourceViewDesc.Texture2D.MipLevels = 1;

            // Create the shader resource view.
            result = pDevice->CreateShaderResourceView(pRet->m_pResolvedTexture, &shaderResourceViewDesc, &pRet->m_pResolvedTextureView);
            if (result != S_OK)
            {
                assert(false && "Failed CreateShaderResourceView");
                return nullptr;
            }
        }

        pRet->createRenderTargetViews(pRet->m_pTexture, pRet->m_pTextureView, pRet->m_pRenderTargetView);
        if (willUseFX)
        {
            pRet->createRenderTargetViews(pRet->m_pTextureFX, pRet->m_pTextureViewFX, pRet->m_pRenderTargetViewFX);
        }

        pRet->m_type = Type::RenderTarget;
        return pRet;
    }

    OTextureRef Texture::createScreenRenderTarget(bool willBeUsedInEffects, RenderTargetFormat format)
    {
        Point res = oRenderer->getTrueResolution();
        if (oSettings->getIsRetroMode())
        {
            res = oSettings->getRetroResolution();
        }

        auto pRet = createRenderTarget(res, willBeUsedInEffects, format);
        if (pRet)
        {
            pRet->m_isScreenRenderTarget = true;
        }
        pRet->m_type = Type::ScreenRenderTarget;
        return pRet;
    }

    OTextureRef Texture::createDynamic(const Point& size)
    {
        auto pRet = std::shared_ptr<TextureD3D11>(new TextureD3D11());

        ID3D11Texture2D* pTexture = NULL;
        ID3D11ShaderResourceView* pTextureView = NULL;

        D3D11_TEXTURE2D_DESC desc;
        desc.Width = static_cast<UINT>(size.x);
        desc.Height = static_cast<UINT>(size.y);
        desc.MipLevels = desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;

        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDevice = pRendererD3D11->getDevice();
        auto ret = pDevice->CreateTexture2D(&desc, NULL, &pTexture);
        assert(ret == S_OK);
        ret = pDevice->CreateShaderResourceView(pTexture, NULL, &pTextureView);
        assert(ret == S_OK);

        pRet->m_size = size;
        pRet->m_pTextureView = pTextureView;
        pRet->m_pTexture = pTexture;
        pRet->m_type = Type::Dynamic;
        return pRet;
    }

    OTextureRef Texture::createFromFile(const std::string& filename, const OContentManagerRef& pContentManager, bool generateMipmaps)
    {
        bool premultiplied = true;
                
        std::string assetFilename = pContentManager->findResourceFile(filename);
        if (assetFilename.empty())
        {
            assetFilename = filename;
        }

        // Load config json.font (optional)
        if (onut::getExtension(assetFilename) == "TEXTURE")
        {
            std::ifstream fic(assetFilename);
            Json::Value json;
            fic >> json;
            fic.close();

            if (json["name"].isString())
            {
                assetFilename = pContentManager->findResourceFile(json["name"].asString());
            }
            else
            {
                // Assume fnt
                assetFilename = pContentManager->findResourceFile(onut::getFilenameWithoutExtension(assetFilename) + ".fnt");
            }
            if (json["premultiplied"].isBool())
            {
                premultiplied = json["premultiplied"].asBool();
            }
        }

        int w, h, n;
        auto image = stbi_load(assetFilename.c_str(), &w, &h, &n, 4);
        if (!image) return nullptr;
        Point size{w, h};

        // Pre multiplied
        if (premultiplied)
        {
            uint8_t* pImageData = image;
            auto len = size.x * size.y;
            for (decltype(len) i = 0; i < len; ++i, pImageData += 4)
            {
                pImageData[0] = pImageData[0] * pImageData[3] / 255;
                pImageData[1] = pImageData[1] * pImageData[3] / 255;
                pImageData[2] = pImageData[2] * pImageData[3] / 255;
            }
        }

        auto pRet = createFromData(image, size, generateMipmaps);
        stbi_image_free(image);
        pRet->setName(onut::getFilename(filename));
        pRet->m_type = Type::Static;
        return pRet;
    }

    OTextureRef Texture::createFromFileData(const uint8_t* pData, uint32_t dataSize, bool generateMipmaps)
    {
        int w, h, n;
        auto image = stbi_load_from_memory(pData, (int)dataSize, &w, &h, &n, 4);
        if (!image) return nullptr;
        Point size{static_cast<int>(w), static_cast<int>(h)};

        // Pre multiplied
        uint8_t* pImageData = image;
        auto len = size.x * size.y;
        for (int i = 0; i < len; ++i, pImageData += 4)
        {
            pImageData[0] = pImageData[0] * pImageData[3] / 255;
            pImageData[1] = pImageData[1] * pImageData[3] / 255;
            pImageData[2] = pImageData[2] * pImageData[3] / 255;
        }

        auto pRet = createFromData(image, size, generateMipmaps);
        stbi_image_free(image);
        return pRet;
    }

    OTextureRef Texture::createFromData(const uint8_t* pData, const Point& size, bool generateMipmaps)
    {
        auto pRet = std::shared_ptr<TextureD3D11>(new TextureD3D11());

#if defined(WIN32)
        ID3D11Texture2D* pTexture = NULL;
        ID3D11ShaderResourceView* pTextureView = NULL;

        // Manually generate mip levels
        bool allowMipMaps = true;
        int w2 = 1;
        int h2 = 1;
        while (w2 < size.x) w2 *= 2;
        if (size.x != w2) allowMipMaps = false;
        while (h2 < size.y) h2 *= 2;
        if (size.y != h2) allowMipMaps = false;
        unsigned char* pMipMaps = NULL;
        int mipLevels = 1;
        D3D11_SUBRESOURCE_DATA* mipsData = NULL;
        allowMipMaps = allowMipMaps && generateMipmaps;
        if (allowMipMaps)
        {
            int biggest = std::max<>(w2, h2);
            int w2t = w2;
            int h2t = h2;
            int totalSize = w2t * h2t * 4;
            while (!(w2t == 1 && h2t == 1))
            {
                ++mipLevels;
                w2t /= 2;
                if (w2t < 1) w2t = 1;
                h2t /= 2;
                if (h2t < 1) h2t = 1;
                totalSize += w2t * h2t * 4;
            }
            pMipMaps = new byte[totalSize];
            memcpy(pMipMaps, pData, size.x * size.y * 4);

            mipsData = new D3D11_SUBRESOURCE_DATA[mipLevels];

            w2t = w2;
            h2t = h2;
            totalSize = 0;
            int mipTarget = mipLevels;
            mipLevels = 0;
            byte* prev;
            byte* cur;
            while (mipLevels != mipTarget)
            {
                prev = pMipMaps + totalSize;
                mipsData[mipLevels].pSysMem = prev;
                mipsData[mipLevels].SysMemPitch = w2t * 4;
                mipsData[mipLevels].SysMemSlicePitch = 0;
                totalSize += w2t * h2t * 4;
                cur = pMipMaps + totalSize;
                w2t /= 2;
                if (w2t < 1) w2t = 1;
                h2t /= 2;
                if (h2t < 1) h2t = 1;
                ++mipLevels;
                if (mipLevels == mipTarget) break;
                int accum;

                // Generate the mips
                int multX = w2 / w2t;
                int multY = h2 / h2t;
                for (int y = 0; y < h2t; ++y)
                {
                    for (int x = 0; x < w2t; ++x)
                    {
                        for (int k = 0; k < 4; ++k)
                        {
                            accum = 0;
                            accum += prev[(y * multY * w2 + x * multX) * 4 + k];
                            accum += prev[(y * multY * w2 + (x + multX / 2) * multX) * 4 + k];
                            accum += prev[((y + multY / 2) * multY * w2 + x * multX) * 4 + k];
                            accum += prev[((y + multY / 2) * multY * w2 + (x + multX / 2) * multX) * 4 + k];
                            cur[(y * w2t + x) * 4 + k] = accum / 4;
                        }
                    }
                }

                w2 = w2t;
                h2 = h2t;
            }
        }

        D3D11_TEXTURE2D_DESC desc;
        desc.Width = static_cast<UINT>(size.x);
        desc.Height = static_cast<UINT>(size.y);
        desc.MipLevels = mipLevels;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = (pMipMaps) ? pMipMaps : pData;
        data.SysMemPitch = size.x * 4;
        data.SysMemSlicePitch = 0;

        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDevice = pRendererD3D11->getDevice();
        auto ret = pDevice->CreateTexture2D(&desc, (mipsData) ? mipsData : &data, &pTexture);
        assert(ret == S_OK);
        ret = pDevice->CreateShaderResourceView(pTexture, NULL, &pTextureView);
        assert(ret == S_OK);

        pTexture->Release();
        if (pMipMaps) delete[] pMipMaps;
        if (mipsData) delete[] mipsData;

        pRet->m_size = size;
        pRet->m_pTextureView = pTextureView;
#else
#error
#endif

        pRet->m_type = Type::Static;
        return pRet;
    }

    OTextureRef Texture::createFromDataWithFormat(const uint8_t* pData, const Point& size, RenderTargetFormat format, bool generateMipmaps)
    {
        auto pRet = std::shared_ptr<TextureD3D11>(new TextureD3D11());

        ID3D11Texture2D* pTexture = NULL;
        ID3D11ShaderResourceView* pTextureView = NULL;

        pRet->m_format = format;

        D3D11_TEXTURE2D_DESC desc;
        desc.Width = static_cast<UINT>(size.x);
        desc.Height = static_cast<UINT>(size.y);
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = pRet->getDXGIFormat();
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = pData;
        data.SysMemPitch = size.x * pRet->getPitch();
        data.SysMemSlicePitch = 0;

        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDevice = pRendererD3D11->getDevice();
        auto ret = pDevice->CreateTexture2D(&desc, &data, &pTexture);
        assert(ret == S_OK);
        ret = pDevice->CreateShaderResourceView(pTexture, NULL, &pTextureView);
        assert(ret == S_OK);

        pTexture->Release();

        pRet->m_size = size;
        pRet->m_pTextureView = pTextureView;

        pRet->m_type = Type::Static;
        return pRet;
    }

    void TextureD3D11::setData(const uint8_t* pData)
    {
        assert(isDynamic()); // Only dynamic texture can be set data

        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDeviceContext = pRendererD3D11->getDeviceContext();

        D3D11_MAPPED_SUBRESOURCE data;
        pDeviceContext->Map(m_pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
        for (int y = 0; y < m_size.y; ++y)
        {
            memcpy(reinterpret_cast<uint8_t*>(data.pData) + y * data.RowPitch, pData + y * m_size.x * 4, m_size.x * 4);
        }
        pDeviceContext->Unmap(m_pTexture, 0);
    }

    TextureD3D11::~TextureD3D11()
    {
        if (m_pTextureView) m_pTextureView->Release();
        if (m_pTexture) m_pTexture->Release();
        if (m_pRenderTargetView) m_pRenderTargetView->Release();
        if (m_pTextureViewFX) m_pTextureViewFX->Release();
        if (m_pTextureFX) m_pTextureFX->Release();
        if (m_pRenderTargetViewFX) m_pRenderTargetViewFX->Release();
    }

    void TextureD3D11::resizeTarget(const Point& size)
    {
        if (m_size == size) return;

        m_size = size;

        // Release
        if (m_pTexture) m_pTexture->Release();
        if (m_pTextureView) m_pTextureView->Release();
        if (m_pRenderTargetView)
        {
            m_pRenderTargetView->Release();
            m_pTexture = nullptr;
            m_pTextureView = nullptr;
            m_pRenderTargetView = nullptr;
            createRenderTargetViews(m_pTexture, m_pTextureView, m_pRenderTargetView);
        }

        if (m_pTextureFX) m_pTextureFX->Release();
        if (m_pTextureViewFX) m_pTextureViewFX->Release();
        if (m_pRenderTargetViewFX)
        {
            m_pRenderTargetViewFX->Release();
            m_pTextureFX = nullptr;
            m_pTextureViewFX = nullptr;
            m_pRenderTargetViewFX = nullptr;
            createRenderTargetViews(m_pTextureFX, m_pTextureViewFX, m_pRenderTargetViewFX);
        }
    }

    ID3D11RenderTargetView* TextureD3D11::getD3DRenderTargetView()
    {
        if (m_pRenderTargetView)
        {
            if (m_isScreenRenderTarget)
            {
                Point res = oRenderer->getTrueResolution();
                if (oSettings->getIsRetroMode())
                {
                    res = oSettings->getRetroResolution();
                }
                if (m_size != res)
                {
                    m_size = res;

                    // Release
                    if (m_pTexture) m_pTexture->Release();
                    m_pTexture = nullptr;
                    if (m_pTextureView) m_pTextureView->Release();
                    m_pTextureView = nullptr;
                    if (m_pRenderTargetView)
                    {
                        m_pRenderTargetView->Release();
                        m_pRenderTargetView = nullptr;
                        createRenderTargetViews(m_pTexture, m_pTextureView, m_pRenderTargetView);
                    }

                    if (m_pTextureFX) m_pTextureFX->Release();
                    m_pTextureFX = nullptr;
                    if (m_pTextureViewFX) m_pTextureViewFX->Release();
                    m_pTextureViewFX = nullptr;
                    if (m_pRenderTargetViewFX)
                    {
                        m_pRenderTargetViewFX->Release();
                        createRenderTargetViews(m_pTextureFX, m_pTextureViewFX, m_pRenderTargetViewFX);
                    }
                    m_pRenderTargetViewFX = nullptr;
                }
            }
        }

        return m_pRenderTargetView;
    }

    void TextureD3D11::clearRenderTarget(const Color& color)
    {
        auto pRendererD3D11 = ODynamicCast<ORendererD3D11>(oRenderer);
        pRendererD3D11->getDeviceContext()->ClearRenderTargetView(m_pRenderTargetView, &color.r);
        needResolve = true;
    }

    void TextureD3D11::blur(float amountX, float amountY)
    {
        if (!m_pRenderTargetView) return; // Not a render target
        if (!m_pRenderTargetViewFX)
        {
            createRenderTargetViews(m_pTextureFX, m_pTextureViewFX, m_pRenderTargetViewFX);
        }

        oRenderer->renderStates.viewport.push({0, 0, m_size.x, m_size.y});
        oRenderer->renderStates.renderTargets[0].push(shared_from_this());
        oRenderer->renderStates.textures[0].push(shared_from_this());

        int i = 0;
        if (amountY == -1.0f) amountY = amountX;
        float amount = std::max(amountX, amountY);
        float ratioX = amountX / amount;
        float ratioY = amountY / amount;
        while (amount > 0.f)
        {
            oRenderer->setKernelSize({
                1.f / static_cast<float>(m_size.x) * ((float)i + amountX) / 6,
                1.f / static_cast<float>(m_size.y) * ((float)i + amountY) / 6
            });
            amountX -= 6.f * ratioX;
            amountY -= 6.f * ratioY;
            amount -= 6.f;
            
            needResolve = true;
            resolve();

            oRenderer->renderStates.renderTargets[0].forceDirty();
            oRenderer->renderStates.textures[0].forceDirty();
            std::swap(m_pRenderTargetView, m_pRenderTargetViewFX);
            clearRenderTarget(Color::Transparent);
            oRenderer->drawBlurH();
            std::swap(m_pTextureView, m_pTextureViewFX);
            std::swap(m_pTexture, m_pTextureFX);

            needResolve = true;
            resolve();

            oRenderer->renderStates.renderTargets[0].forceDirty();
            oRenderer->renderStates.textures[0].forceDirty();
            std::swap(m_pRenderTargetView, m_pRenderTargetViewFX);
            clearRenderTarget(Color::Transparent);
            oRenderer->drawBlurV();
            std::swap(m_pTextureView, m_pTextureViewFX);
            std::swap(m_pTexture, m_pTextureFX);

            i += 1;
        }

        oRenderer->renderStates.viewport.pop();
        oRenderer->renderStates.renderTargets[0].pop();
        oRenderer->renderStates.textures[0].pop();
    }

    void TextureD3D11::sepia(const Vector3& tone, float saturation, float sepiaAmount)
    {
        if (!m_pRenderTargetView) return; // Not a render target
        if (!m_pRenderTargetViewFX)
        {
            createRenderTargetViews(m_pTextureFX, m_pTextureViewFX, m_pRenderTargetViewFX);
        }

        oRenderer->renderStates.viewport.push({0, 0, m_size.x, m_size.y});
        oRenderer->renderStates.renderTargets[0].push(shared_from_this());
        oRenderer->renderStates.renderTargets[0].forceDirty();
        oRenderer->renderStates.textures[0].push(shared_from_this());

        std::swap(m_pRenderTargetView, m_pRenderTargetViewFX);
        needResolve = true;
        clearRenderTarget(Color::Transparent);

        oRenderer->setSepia(tone, saturation, sepiaAmount);
        oRenderer->drawSepia();

        std::swap(m_pTexture, m_pTextureFX);
        std::swap(m_pTextureView, m_pTextureViewFX);

        oRenderer->renderStates.viewport.pop();
        oRenderer->renderStates.renderTargets[0].pop();
        oRenderer->renderStates.renderTargets[0].forceDirty();
        oRenderer->renderStates.textures[0].pop();
    }

    void TextureD3D11::crt()
    {
        if (!m_pRenderTargetView) return; // Not a render target
        if (!m_pRenderTargetViewFX)
        {
            createRenderTargetViews(m_pTextureFX, m_pTextureViewFX, m_pRenderTargetViewFX);
        }

        //needResolve = true;
        //resolve();

        oRenderer->renderStates.viewport.push({0, 0, m_size.x, m_size.y});
        oRenderer->renderStates.renderTargets[0].push(shared_from_this());
        oRenderer->renderStates.renderTargets[0].forceDirty();
        oRenderer->renderStates.textures[0].push(shared_from_this());

        std::swap(m_pRenderTargetView, m_pRenderTargetViewFX);
        needResolve = true;
        clearRenderTarget(Color::Transparent);

        oRenderer->setCRT(getSizef());
        oRenderer->drawCRT();

        std::swap(m_pTexture, m_pTextureFX);
        std::swap(m_pTextureView, m_pTextureViewFX);

        oRenderer->renderStates.viewport.pop();
        oRenderer->renderStates.renderTargets[0].pop();
        oRenderer->renderStates.renderTargets[0].forceDirty();
        oRenderer->renderStates.textures[0].pop();
    }

    void TextureD3D11::cartoon(const Vector3& tone)
    {
        if (!m_pRenderTargetView) return; // Not a render target
        if (!m_pRenderTargetViewFX)
        {
            createRenderTargetViews(m_pTextureFX, m_pTextureViewFX, m_pRenderTargetViewFX);
        }

        oRenderer->renderStates.viewport.push({0, 0, m_size.x, m_size.y});
        oRenderer->renderStates.renderTargets[0].push(shared_from_this());
        oRenderer->renderStates.renderTargets[0].forceDirty();
        oRenderer->renderStates.textures[0].push(shared_from_this());

        std::swap(m_pRenderTargetView, m_pRenderTargetViewFX);
        needResolve = true;
        clearRenderTarget(Color::Transparent);

        oRenderer->setCartoon(tone);
        oRenderer->drawCartoon();

        std::swap(m_pTexture, m_pTextureFX);
        std::swap(m_pTextureView, m_pTextureViewFX);

        oRenderer->renderStates.viewport.pop();
        oRenderer->renderStates.renderTargets[0].pop();
        oRenderer->renderStates.renderTargets[0].forceDirty();
        oRenderer->renderStates.textures[0].pop();
    }

    void TextureD3D11::vignette(float amount)
    {
        if (!m_pRenderTargetView) return; // Not a render target
        if (!m_pRenderTargetViewFX)
        {
            createRenderTargetViews(m_pTextureFX, m_pTextureViewFX, m_pRenderTargetViewFX);
        }

        oRenderer->renderStates.viewport.push({0, 0, m_size.x, m_size.y});
        oRenderer->renderStates.renderTargets[0].push(shared_from_this());
        oRenderer->renderStates.renderTargets[0].forceDirty();
        oRenderer->renderStates.textures[0].push(shared_from_this());

        std::swap(m_pRenderTargetView, m_pRenderTargetViewFX);
        needResolve = true;
        clearRenderTarget(Color::Transparent);

        oRenderer->setVignette({
            1.f / static_cast<float>(m_size.x),
            1.f / static_cast<float>(m_size.y)
        }, amount);
        oRenderer->drawVignette();

        std::swap(m_pTexture, m_pTextureFX);
        std::swap(m_pTextureView, m_pTextureViewFX);

        oRenderer->renderStates.viewport.pop();
        oRenderer->renderStates.renderTargets[0].pop();
        oRenderer->renderStates.renderTargets[0].forceDirty();
        oRenderer->renderStates.textures[0].pop();
    }

    UINT TextureD3D11::getPitch()
    {
        switch (m_format)
        {
            case RenderTargetFormat::R8: return 1;
            case RenderTargetFormat::RG8: return 2;
            case RenderTargetFormat::RGBA8: return 4;
            case RenderTargetFormat::R16: return 2;
            case RenderTargetFormat::RG16: return 4;
            case RenderTargetFormat::RGBA16: return 8;
            case RenderTargetFormat::R32: return 4;
            case RenderTargetFormat::RG32: return 8;
            case RenderTargetFormat::RGBA32: return 16;
            case RenderTargetFormat::RGB10A2: return 4;
            case RenderTargetFormat::I8: return 1;
            default: return 4;
        };
    }

    DXGI_FORMAT TextureD3D11::getDXGIFormat()
    {
        switch (m_format)
        {
            case RenderTargetFormat::R8: return DXGI_FORMAT_R8_UNORM;
            case RenderTargetFormat::RG8: return DXGI_FORMAT_R8G8_UNORM;
            case RenderTargetFormat::RGBA8: return DXGI_FORMAT_R8G8B8A8_UNORM;
            case RenderTargetFormat::R16: return DXGI_FORMAT_R16_UNORM;
            case RenderTargetFormat::RG16: return DXGI_FORMAT_R16G16_UNORM;
            case RenderTargetFormat::RGBA16: return DXGI_FORMAT_R16G16B16A16_UNORM;
            case RenderTargetFormat::R32: return DXGI_FORMAT_R32_FLOAT;
            case RenderTargetFormat::RG32: return DXGI_FORMAT_R32G32_FLOAT;
            case RenderTargetFormat::RGBA32: return DXGI_FORMAT_R32G32B32A32_FLOAT;
            case RenderTargetFormat::RGB10A2: return DXGI_FORMAT_R10G10B10A2_UNORM;
            case RenderTargetFormat::I8: return DXGI_FORMAT_R8_UINT;
            default: return DXGI_FORMAT_R8G8B8A8_UNORM;
        };
    }

    void TextureD3D11::createRenderTargetViews(ID3D11Texture2D*& pTexture, ID3D11ShaderResourceView*& pTextureView, ID3D11RenderTargetView*& pRenderTargetView)
    {
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDevice = pRendererD3D11->getDevice();

        D3D11_TEXTURE2D_DESC textureDesc = {0};
        HRESULT result;
        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
        D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
        memset(&renderTargetViewDesc, 0, sizeof(renderTargetViewDesc));
        memset(&shaderResourceViewDesc, 0, sizeof(shaderResourceViewDesc));

        // Create Multisampled texture.
        textureDesc.Width = m_size.x;
        textureDesc.Height = m_size.y;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = getDXGIFormat();
        if (multiSampled)
        {
            textureDesc.SampleDesc.Count = MS_COUNT;
            textureDesc.SampleDesc.Quality = MS_QUALITY;
        }
        else
        {
            textureDesc.SampleDesc.Count = 1;
            textureDesc.SampleDesc.Quality = 0;
        }
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        // Create the render target texture.
        result = pDevice->CreateTexture2D(&textureDesc, NULL, &pTexture);
        if (result != S_OK)
        {
            assert(false && "Failed CreateTexture2D");
            return;
        }

        // Setup the description of the render target view.
        renderTargetViewDesc.Format = textureDesc.Format;
        if (multiSampled)
        {
            renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
        }
        else
        {
            renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        }
        renderTargetViewDesc.Texture2D.MipSlice = 0;

        // Create the render target view.
        result = pDevice->CreateRenderTargetView(pTexture, &renderTargetViewDesc, &pRenderTargetView);
        if (result != S_OK)
        {
            assert(false && "Failed CreateRenderTargetView");
            return;
        }

        // Only create a shader view if non-antialiasing, otherwise we will use resolved view.
        if (!multiSampled)
        {
            shaderResourceViewDesc.Format = textureDesc.Format;
            shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
            shaderResourceViewDesc.Texture2D.MipLevels = 1;

            // Create the shader resource view.
            result = pDevice->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &pTextureView);
            if (result != S_OK)
            {
                assert(false && "Failed CreateShaderResourceView");
                return;
            }
        }
    }

    void TextureD3D11::resolve()
    {
        if (needResolve)
        {
            needResolve = false;
            if (multiSampled)
            {
                auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
                auto pDeviceContext = pRendererD3D11->getDeviceContext();
                pDeviceContext->ResolveSubresource(m_pResolvedTexture, 0, m_pTexture, 0, getDXGIFormat());
            }
        }
    }

    ID3D11ShaderResourceView* TextureD3D11::getD3DResourceView()
    {
        if (multiSampled)
        {
            resolve();
            return m_pResolvedTextureView;
        }
        else
        {
            return m_pTextureView;
        }
    }
}

#endif
