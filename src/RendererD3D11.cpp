#if defined(WIN32)
// Onut
#include <onut/IndexBuffer.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Texture.h>
#include <onut/VertexBuffer.h>
#include <onut/Window.h>

// Private
#include "IndexBufferD3D11.h"
#include "RendererD3D11.h"
#include "ShaderD3D11.h"
#include "TextureD3D11.h"
#include "VertexBufferD3D11.h"

// STL
#include <fstream>
#include <vector>

namespace onut
{
    ORendererRef Renderer::create(const OWindowRef& pWindow)
    {
        return OMake<RendererD3D11>(pWindow);
    }

    RendererD3D11::RendererD3D11(const OWindowRef& pWindow)
    {
        memset(m_pBlendStates, 0, sizeof(m_pBlendStates));
        memset(m_pSamplerStates, 0, sizeof(m_pSamplerStates));
        memset(m_pRasterizerStates, 0, sizeof(m_pRasterizerStates));
        memset(m_pDepthStencilStates, 0, sizeof(m_pDepthStencilStates));
    }

    void RendererD3D11::init(const OWindowRef& pWindow)
    {
        createDevice(pWindow);
        createRenderTarget();
        createRenderStates();
        createUniforms();

        Renderer::init(pWindow);
    }

    RendererD3D11::~RendererD3D11()
    {
        if (m_pViewProj2dBuffer) m_pViewProj2dBuffer->Release();
        if (m_pKernelSizeBuffer) m_pKernelSizeBuffer->Release();
        if (m_pSepiaBuffer) m_pSepiaBuffer->Release();

        if (m_pRenderTargetView) m_pRenderTargetView->Release();

        // The new stuff
        for (auto pBlendState : m_pBlendStates)
        {
            if (pBlendState) pBlendState->Release();
        }
        for (auto pSamplerState : m_pSamplerStates)
        {
            if (pSamplerState) pSamplerState->Release();
        }
        for (auto pRasterizerState : m_pRasterizerStates)
        {
            if (pRasterizerState) pRasterizerState->Release();
        }
        for (auto pDepthStencilStates : m_pDepthStencilStates)
        {
            if (pDepthStencilStates) pDepthStencilStates->Release();
        }

        if (m_pDeviceContext) m_pDeviceContext->Release();
        if (m_pDevice) m_pDevice->Release();
        if (m_pSwapChain) m_pSwapChain->Release();
    }

    void RendererD3D11::createDevice(const OWindowRef& pWindow)
    {
        // Define our swap chain
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};
        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = pWindow->getHandle();
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.Windowed = true;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        // Create the swap chain, device and device context
        auto result = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 
#if _DEBUG
            /*D3D11_CREATE_DEVICE_DEBUG*/0,
#else
            0,
#endif
            nullptr, 0, D3D11_SDK_VERSION,
            &swapChainDesc, &m_pSwapChain,
            &m_pDevice, nullptr, &m_pDeviceContext);

        // Check for error
        if (result != S_OK)
        {
            MessageBox(nullptr, L"D3D11CreateDeviceAndSwapChain", L"Error", MB_OK);
            exit(0);
        }
    }

    void RendererD3D11::createRenderTarget()
    {
        ID3D11Texture2D* backBuffer;
        auto result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
        if (result != S_OK)
        {
            MessageBox(nullptr, L"m_pSwapChain->GetBuffer", L"Error", MB_OK);
            exit(0);
        }
        result = m_pDevice->CreateRenderTargetView(backBuffer, nullptr, &m_pRenderTargetView);
        if (result != S_OK)
        {
            MessageBox(nullptr, L"m_pDevice->CreateRenderTargetView", L"Error", MB_OK);
            exit(0);
        }

        backBuffer->GetDesc(&m_backBufferDesc);
        backBuffer->Release();
    }

    void RendererD3D11::onResize(const Point& newSize)
    {
        m_pDeviceContext->Flush();
        m_pDeviceContext->ClearState();

        if (m_pRenderTargetView) m_pRenderTargetView->Release();
        m_pRenderTargetView = nullptr;

        auto ret = m_pSwapChain->ResizeBuffers(0,
                                               static_cast<UINT>(newSize.x),
                                               static_cast<UINT>(newSize.y),
                                               DXGI_FORMAT_UNKNOWN, 0);
        createRenderTarget();
    }

    void RendererD3D11::createRenderStates()
    {
        HRESULT ret;

        // Blend modes
        {
            D3D11_BLEND_DESC blendDesc;
            memset(&blendDesc, 0, sizeof(blendDesc));
            for (int i = 0; i < static_cast<int>(BlendMode::COUNT); ++i)
            {
                switch (static_cast<BlendMode>(i))
                {
                    case BlendMode::Opaque:
                        blendDesc.RenderTarget[0].BlendEnable = FALSE;
                        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
                        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
                        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
                        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
                        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
                        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
                        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
                        break;
                    case BlendMode::Alpha:
                        blendDesc.RenderTarget[0].BlendEnable = TRUE;
                        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
                        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
                        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
                        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
                        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
                        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
                        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
                        break;
                    case BlendMode::Add:
                        blendDesc.RenderTarget[0].BlendEnable = TRUE;
                        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
                        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
                        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
                        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
                        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
                        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
                        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
                        break;
                    case BlendMode::PreMultiplied:
                        blendDesc.RenderTarget[0].BlendEnable = TRUE;
                        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
                        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
                        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
                        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
                        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
                        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
                        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
                        break;
                    case BlendMode::Multiply:
                        blendDesc.RenderTarget[0].BlendEnable = TRUE;
                        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_COLOR;
                        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
                        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
                        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
                        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
                        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
                        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
                        break;
                    case BlendMode::ForceWrite:
                        blendDesc.RenderTarget[0].BlendEnable = TRUE;
                        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
                        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
                        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
                        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
                        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
                        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
                        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
                        break;
                }
                ret = m_pDevice->CreateBlendState(&blendDesc, &m_pBlendStates[i]);
                assert(ret == S_OK);
            }
        }

        // Sampler states
        {
            D3D11_SAMPLER_DESC samplerState;
            memset(&samplerState, 0, sizeof(samplerState));
            samplerState.MaxAnisotropy = 1;
            samplerState.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
            samplerState.MaxLOD = D3D11_FLOAT32_MAX;
            for (int i = 0; i < static_cast<int>(sample::Filtering::COUNT); ++i)
            {
                switch (static_cast<sample::Filtering>(i))
                {
                    case sample::Filtering::Linear:
                        samplerState.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
                        break;
                    case sample::Filtering::Nearest:
                        samplerState.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
                        break;
                }

                for (int j = 0; j < static_cast<int>(sample::AddressMode::COUNT); ++j)
                {
                    switch (static_cast<sample::AddressMode>(j))
                    {
                        case sample::AddressMode::Wrap:
                            samplerState.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
                            samplerState.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
                            samplerState.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
                            break;
                        case sample::AddressMode::Clamp:
                            samplerState.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
                            samplerState.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
                            samplerState.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
                            break;
                    }

                    ret = m_pDevice->CreateSamplerState(&samplerState, &m_pSamplerStates[i * static_cast<int>(sample::AddressMode::COUNT) + j]);
                    assert(ret == S_OK);
                }
            }
        }

        // Rasterizer state
        {
            D3D11_RASTERIZER_DESC rasterizerDesc;
            rasterizerDesc = D3D11_RASTERIZER_DESC{
                D3D11_FILL_SOLID,
                D3D11_CULL_NONE,
                false,
                0,
                0.f,
                0.f,
                false,
                false,
                false,
                false
            };
            ret = m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerStates[0]); // No Scissor
            assert(ret == S_OK);
            rasterizerDesc = D3D11_RASTERIZER_DESC{
                D3D11_FILL_SOLID,
                D3D11_CULL_NONE,
                false,
                0,
                0.f,
                0.f,
                false,
                true,
                false,
                false
            };
            ret = m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerStates[1]); // Scissor
            assert(ret == S_OK);
        }

        // Depth stencil state
        {
            // Depth states
            D3D11_DEPTH_STENCIL_DESC depthStencilState = D3D11_DEPTH_STENCIL_DESC{
                false,
                D3D11_DEPTH_WRITE_MASK_ZERO,
                D3D11_COMPARISON_LESS,
                false,
                0xFF,
                0xFF,
                {D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_INCR, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS},
                {D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_DECR, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS}
            };
            auto ret = m_pDevice->CreateDepthStencilState(&depthStencilState, &m_pDepthStencilStates[0]);
            assert(ret == S_OK);
        }
    }

    void RendererD3D11::createUniforms()
    {
        // 2D view projection matrix
        {
            Matrix viewProj = Matrix::CreateOrthographicOffCenter(0, static_cast<float>(getResolution().x), static_cast<float>(getResolution().y), 0, -999, 999);
            viewProj = viewProj.Transpose();
            D3D11_BUFFER_DESC cbDesc = CD3D11_BUFFER_DESC(sizeof(Matrix), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
            D3D11_SUBRESOURCE_DATA initData{&viewProj, 0, 0};
            auto ret = m_pDevice->CreateBuffer(&cbDesc, &initData, &m_pViewProj2dBuffer);
            assert(ret == S_OK);
        }

        // For effects
        {
            D3D11_BUFFER_DESC cbDesc = CD3D11_BUFFER_DESC(sizeof(Vector4), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
            auto ret = m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pKernelSizeBuffer);
            assert(ret == S_OK);
        }
        {
            D3D11_BUFFER_DESC cbDesc = CD3D11_BUFFER_DESC(sizeof(Vector4) * 2, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
            auto ret = m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pSepiaBuffer);
            assert(ret == S_OK);
        }
    }

    void RendererD3D11::beginFrame()
    {
        // Bind render target
        renderStates.reset();

        // Set viewport
        renderStates.viewport = iRect{0, 0, static_cast<int>(m_backBufferDesc.Width), static_cast<int>(m_backBufferDesc.Height)};

        // Reset 2d view
        set2DCamera(Vector2::Zero);
    }

    void RendererD3D11::endFrame()
    {
        // Swap the buffer!
        if (oSettings->getIsEditorMode())
        {
            m_pSwapChain->Present(0, 0);
        }
        else
        {
            m_pSwapChain->Present(1, 0);
        }
    }

    ID3D11Device* RendererD3D11::getDevice() const
    {
        return m_pDevice;
    }

    ID3D11DeviceContext* RendererD3D11::getDeviceContext() const
    {
        return m_pDeviceContext;
    }

    Point RendererD3D11::getResolution() const
    {
        return Point(static_cast<int>(m_backBufferDesc.Width), static_cast<int>(m_backBufferDesc.Height));
    }

    void RendererD3D11::clear(const Color& color)
    {
        ID3D11RenderTargetView* pRenderTargetView = m_pRenderTargetView;
        if (renderStates.renderTarget.get())
        {
            auto pRenderTargetD3D11 = ODynamicCast<OTextureD3D11>(renderStates.renderTarget.get());
            pRenderTargetView = pRenderTargetD3D11->getD3DRenderTargetView();
        }
        m_pDeviceContext->ClearRenderTargetView(pRenderTargetView, &color.x);
    }

    void RendererD3D11::setKernelSize(const Vector2& kernelSize)
    {
        D3D11_MAPPED_SUBRESOURCE map;
        m_pDeviceContext->Map(m_pKernelSizeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        Vector4 kernelSize4(kernelSize.x, kernelSize.y, 0, 0);
        memcpy(map.pData, &kernelSize4.x, sizeof(kernelSize4));
        m_pDeviceContext->Unmap(m_pKernelSizeBuffer, 0);
        m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pKernelSizeBuffer);
    }

    void RendererD3D11::setCRT(const Vector2& resolution)
    {
        struct sCRT
        {
            Vector2 res;
            Vector2 pixel;
        };
        sCRT crt = {{resolution.x, resolution.y}, {1.f / resolution.x, 1.f / resolution.y}};

        D3D11_MAPPED_SUBRESOURCE map;
        m_pDeviceContext->Map(m_pKernelSizeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        memcpy(map.pData, &crt, sizeof(crt));
        m_pDeviceContext->Unmap(m_pKernelSizeBuffer, 0);
        m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pKernelSizeBuffer);
    }

    void RendererD3D11::setCartoon(const Vector3& tone)
    {
        struct sCartoon
        {
            Vector3 tone;
            float padding;
        };
        sCartoon cartoon = {tone};

        D3D11_MAPPED_SUBRESOURCE map;
        m_pDeviceContext->Map(m_pKernelSizeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        memcpy(map.pData, &cartoon, sizeof(cartoon));
        m_pDeviceContext->Unmap(m_pKernelSizeBuffer, 0);
        m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pKernelSizeBuffer);
    }

    void RendererD3D11::setVignette(const Vector2& kernelSize, float amount)
    {
        struct sVignette
        {
            Vector2 kernelSize;
            float amount;
            float padding;
        };
        sVignette vignette = {kernelSize, amount};

        D3D11_MAPPED_SUBRESOURCE map;
        m_pDeviceContext->Map(m_pKernelSizeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        memcpy(map.pData, &vignette, sizeof(vignette));
        m_pDeviceContext->Unmap(m_pKernelSizeBuffer, 0);
        m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pKernelSizeBuffer);
    }

    void RendererD3D11::setSepia(const Vector3& tone, float saturation, float sepiaAmount)
    {
        struct sSepia
        {
            Vector3 tone;
            float desaturation;
            float sepia;
            Vector3 padding;
        };
        sSepia sepia = {tone, 1.f - saturation, sepiaAmount};

        D3D11_MAPPED_SUBRESOURCE map;
        m_pDeviceContext->Map(m_pSepiaBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        memcpy(map.pData, &sepia, sizeof(sepia));
        m_pDeviceContext->Unmap(m_pSepiaBuffer, 0);
        m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pSepiaBuffer);
    }

    void RendererD3D11::draw(uint32_t vertexCount)
    {
        applyRenderStates();
        m_pDeviceContext->Draw(static_cast<UINT>(vertexCount), 0);
    }

    void RendererD3D11::drawIndexed(uint32_t indexCount)
    {
        applyRenderStates();
        m_pDeviceContext->DrawIndexed(static_cast<UINT>(indexCount), 0, 0);
    }

    void RendererD3D11::applyRenderStates()
    {
        // Render target
        if (renderStates.renderTarget.isDirty())
        {
            auto& pRenderTarget = renderStates.renderTarget.get();
            ID3D11RenderTargetView* pRenderTargetView = m_pRenderTargetView;
            if (pRenderTarget)
            {
                auto pRenderTargetD3D11 = ODynamicCast<OTextureD3D11>(pRenderTarget);
                pRenderTargetView = pRenderTargetD3D11->getD3DRenderTargetView();
                for (int i = 0; i < RenderStates::MAX_TEXTURES; ++i)
                {
                    if (m_boundTextures[i] == pRenderTarget)
                    {
                        ID3D11ShaderResourceView* pResourceView = nullptr;
                        m_pDeviceContext->PSSetShaderResources(static_cast<UINT>(i), 1, &pResourceView);
                    }
                }
            }
            m_pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
            renderStates.renderTarget.resetDirty();
            renderStates.viewport.forceDirty();
        }

        // Textures
        for (int i = 0; i < RenderStates::MAX_TEXTURES; ++i)
        {
            auto& pTextureState = renderStates.textures[i];
            if (pTextureState.isDirty())
            {
                ID3D11ShaderResourceView* pResourceView = nullptr;
                m_boundTextures[i] = pTextureState.get();
                if (pTextureState.get() != nullptr)
                {
                    auto pRenderTargetD3D11 = ODynamicCast<OTextureD3D11>(pTextureState.get());
                    pResourceView = pRenderTargetD3D11->getD3DResourceView();
                }
                m_pDeviceContext->PSSetShaderResources(static_cast<UINT>(i), 1, &pResourceView);
                pTextureState.resetDirty();
            }
        }

        // Blend
        if (renderStates.blendMode.isDirty())
        {
            m_pDeviceContext->OMSetBlendState(m_pBlendStates[static_cast<int>(renderStates.blendMode.get())], NULL, 0xffffffff);
            renderStates.blendMode.resetDirty();
        }

        // Sampler state
        if (renderStates.sampleFiltering.isDirty() ||
            renderStates.sampleAddressMode.isDirty())
        {
            m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerStates[
                static_cast<int>(renderStates.sampleFiltering.get()) * static_cast<int>(sample::AddressMode::COUNT) + 
                static_cast<int>(renderStates.sampleAddressMode.get())]);
            renderStates.sampleFiltering.resetDirty();
            renderStates.sampleAddressMode.resetDirty();
        }

        // Viewport
        if (renderStates.viewport.isDirty())
        {
            auto& rect = renderStates.viewport.get();
            auto d3d11Viewport = CD3D11_VIEWPORT(
                static_cast<float>(rect.left),
                static_cast<float>(rect.top),
                static_cast<float>(rect.right - rect.left),
                static_cast<float>(rect.bottom - rect.top));
            m_pDeviceContext->RSSetViewports(1, &d3d11Viewport);
            renderStates.viewport.resetDirty();
            renderStates.scissor.forceDirty();
        }

        // Scissor enabled
        if (renderStates.scissorEnabled.isDirty())
        {
            if (renderStates.scissorEnabled.get())
            {
                m_pDeviceContext->RSSetState(m_pRasterizerStates[1]);
            }
            else
            {
                m_pDeviceContext->RSSetState(m_pRasterizerStates[0]);
            }
            renderStates.scissorEnabled.resetDirty();
        }

        // Scissor
        if (renderStates.scissorEnabled.get() &&
            renderStates.scissor.isDirty())
        {
            auto& rect = renderStates.scissor.get();
            D3D11_RECT dxRect[1] = {
                {
                    static_cast<LONG>(rect.left),
                    static_cast<LONG>(rect.top),
                    static_cast<LONG>(rect.right),
                    static_cast<LONG>(rect.bottom),
                }
            };
            m_pDeviceContext->RSSetScissorRects(1, dxRect);
            renderStates.scissor.resetDirty();
        }

        // Transform matrix
        if (renderStates.viewProjection.isDirty() ||
            renderStates.world.isDirty())
        {
            auto world = renderStates.world.get().Transpose();
            auto finalTransform = renderStates.viewProjection.get() * world;

            D3D11_MAPPED_SUBRESOURCE map;
            m_pDeviceContext->Map(m_pViewProj2dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
            memcpy(map.pData, &finalTransform._11, sizeof(finalTransform));
            m_pDeviceContext->Unmap(m_pViewProj2dBuffer, 0);
            m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pViewProj2dBuffer);

            renderStates.viewProjection.resetDirty();
            renderStates.world.resetDirty();
        }

        // Depth
        if (renderStates.depthEnabled.isDirty())
        {
            m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilStates[0], 1);
            renderStates.depthEnabled.resetDirty();
        }

        // Primitive mode
        if (renderStates.primitiveMode.isDirty())
        {
            switch (renderStates.primitiveMode.get())
            {
                case PrimitiveMode::PointList:
                    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
                    break;
                case PrimitiveMode::LineList:
                    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
                    break;
                case PrimitiveMode::LineStrip:
                    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
                    break;
                case PrimitiveMode::TriangleList:
                    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                    break;
                case PrimitiveMode::TriangleStrip:
                    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
                    break;
            }
            renderStates.primitiveMode.resetDirty();
        }

        // Shaders
        if (renderStates.vertexShader.isDirty())
        {
            auto pShaderD3D11 = std::dynamic_pointer_cast<OShaderD3D11>(renderStates.vertexShader.get());
            m_pDeviceContext->VSSetShader(pShaderD3D11->getVertexShader(), nullptr, 0);
            m_pDeviceContext->IASetInputLayout(pShaderD3D11->getInputLayout());
            renderStates.vertexShader.resetDirty();
        }
        if (renderStates.pixelShader.isDirty())
        {
            auto pShaderD3D11 = std::dynamic_pointer_cast<OShaderD3D11>(renderStates.pixelShader.get());
            m_pDeviceContext->PSSetShader(pShaderD3D11->getPixelShader(), nullptr, 0);
            renderStates.pixelShader.resetDirty();
        }

        // Vertex/Index buffers
        if (renderStates.vertexBuffer.isDirty())
        {
            if (renderStates.vertexBuffer.get())
            {
                auto pVertexBufferD3D11 = ODynamicCast<OVertexBufferD3D11>(renderStates.vertexBuffer.get());
                auto pD3DBuffer = pVertexBufferD3D11->getBuffer();
                UINT stride = static_cast<UINT>(renderStates.vertexShader.get()->getVertexSize());
                UINT offset = 0;
                m_pDeviceContext->IASetVertexBuffers(0, 1, &pD3DBuffer, &stride, &offset);
            }
        }
        if (renderStates.indexBuffer.isDirty())
        {
            if (renderStates.indexBuffer.get())
            {
                auto pIndexBufferD3D11 = ODynamicCast<OIndexBufferD3D11>(renderStates.indexBuffer.get());
                auto pD3DBuffer = pIndexBufferD3D11->getBuffer();
                m_pDeviceContext->IASetIndexBuffer(pD3DBuffer, DXGI_FORMAT_R16_UINT, 0);
            }
        }
    }
}

#endif
