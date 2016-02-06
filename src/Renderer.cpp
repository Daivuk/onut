#include "onut.h"
#include "Renderer.h"
#include "Window.h"

#include <fstream>
#include <vector>

#include "_2dvs.cso.h"
#include "_2dps.cso.h"
#include "blurvs.cso.h"
#include "blurhps.cso.h"
#include "blurvps.cso.h"
#include "sepia.cso.h"
#include "crt.cso.h"
#include "cartoon.cso.h"
#include "vignette.cso.h"

namespace onut
{
    Renderer::Renderer(Window& window)
    {
        createDevice(window);
        createRenderTarget();
        createRenderStates();
        loadShaders();
        createUniforms();

        // Set up the description of the static vertex buffer.
        D3D11_BUFFER_DESC vertexBufferDesc;
        vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        vertexBufferDesc.ByteWidth = 12 * 4;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;
        vertexBufferDesc.StructureByteStride = 0;

        // Give the subresource structure a pointer to the vertex data.
        D3D11_SUBRESOURCE_DATA vertexData;
        const float vertices[] = {
            -1, -1, 
            -1, 1, 
            1, -1, 
            1, -1, 
            -1, 1, 
            1, 1
        };
        vertexData.pSysMem = vertices;
        vertexData.SysMemPitch = 0;
        vertexData.SysMemSlicePitch = 0;

        auto ret = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pEffectsVertexBuffer);
        assert(ret == S_OK);
    }

    Renderer::~Renderer()
    {
        delete m_renderTarget;

        if (m_pWorldMatrixBuffer) m_pWorldMatrixBuffer->Release();
        if (m_pViewProj2dBuffer) m_pViewProj2dBuffer->Release();
        if (m_pKernelSizeBuffer) m_pKernelSizeBuffer->Release();
        if (m_pSepiaBuffer) m_pSepiaBuffer->Release();

        if (m_p2DInputLayout) m_p2DInputLayout->Release();
        if (m_p2DPixelShader) m_p2DPixelShader->Release();
        if (m_p2DVertexShader) m_p2DVertexShader->Release();

        if (m_pEffectsVertexShader) m_pEffectsVertexShader->Release();
        if (m_pBlurHPixelShader) m_pBlurHPixelShader->Release();
        if (m_pBlurVPixelShader) m_pBlurVPixelShader->Release();
        if (m_pSepiaPixelShader) m_pSepiaPixelShader->Release();
        if (m_pCRTPixelShader) m_pCRTPixelShader->Release();
        if (m_pCartoonPixelShader) m_pCartoonPixelShader->Release();
        if (m_pVignettePixelShader) m_pVignettePixelShader->Release();
        if (m_pEffectsInputLayout) m_pEffectsInputLayout->Release();
        if (m_pEffectsVertexBuffer) m_pEffectsVertexBuffer->Release();
        if (m_pEffectsSampler) m_pEffectsSampler->Release();

        if (m_pSs2D) m_pSs2D->Release();
        if (m_pBs2D) m_pBs2D->Release();
        if (m_pSr2D) m_pSr2D->Release();
        if (m_pSr2DScissor) m_pSr2DScissor->Release();
        if (m_pDs2D) m_pDs2D->Release();

        if (m_renderTargetView) m_renderTargetView->Release();

        if (m_deviceContext) m_deviceContext->Release();
        if (m_device) m_device->Release();
        if (m_swapChain) m_swapChain->Release();
    }

    void Renderer::createDevice(Window& window)
    {
        // Define our swap chain
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};
        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = window.getHandle();
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.Windowed = true;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        // Create the swap chain, device and device context
        auto result = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 
#if _DEBUG
            0,
#else
            0,
#endif
            nullptr, 0, D3D11_SDK_VERSION,
            &swapChainDesc, &m_swapChain,
            &m_device, nullptr, &m_deviceContext);

        // Check for error
        if (result != S_OK)
        {
            MessageBox(nullptr, L"D3D11CreateDeviceAndSwapChain", L"Error", MB_OK);
            exit(0);
        }
    }

    void Renderer::createRenderTarget()
    {
        ID3D11Texture2D* backBuffer;
        auto result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
        if (result != S_OK)
        {
            MessageBox(nullptr, L"m_swapChain->GetBuffer", L"Error", MB_OK);
            exit(0);
        }
        result = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
        if (result != S_OK)
        {
            MessageBox(nullptr, L"m_device->CreateRenderTargetView", L"Error", MB_OK);
            exit(0);
        }

        backBuffer->GetDesc(&m_backBufferDesc);
        backBuffer->Release();
    }

    void Renderer::onResize(const POINT& newSize)
    {
        resetState();
        m_deviceContext->Flush();
        m_deviceContext->ClearState();

        if (m_renderTargetView) m_renderTargetView->Release();
        m_renderTargetView = nullptr;

        auto ret = m_swapChain->ResizeBuffers(0, newSize.x, newSize.y, DXGI_FORMAT_UNKNOWN, 0);
        createRenderTarget();
    }

    void Renderer::createRenderStates()
    {
        // 2D Depth state
        auto ret = m_device->CreateDepthStencilState(&(D3D11_DEPTH_STENCIL_DESC{
            false,
            D3D11_DEPTH_WRITE_MASK_ZERO,
            D3D11_COMPARISON_LESS,
            false,
            0xFF,
            0xFF,
            {D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_INCR, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS},
            {D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_DECR, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS}
        }), &m_pDs2D);
        assert(ret == S_OK);

        // 2D Rasterizer state
        ret = m_device->CreateRasterizerState(&(D3D11_RASTERIZER_DESC{
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
        }), &m_pSr2D);
        assert(ret == S_OK);

        ret = m_device->CreateRasterizerState(&(D3D11_RASTERIZER_DESC{
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
        }), &m_pSr2DScissor);
        assert(ret == S_OK);

        // 2D Blend state
        ret = m_device->CreateBlendState(&(D3D11_BLEND_DESC{
            FALSE,
            FALSE,
            {{
                    TRUE,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_INV_SRC_ALPHA,
                    D3D11_BLEND_OP_ADD,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_OP_ADD,
                    D3D10_COLOR_WRITE_ENABLE_ALL
                }, {0}, {0}, {0}, {0}, {0}, {0}, {0}}
        }), &m_pBs2D);
        assert(ret == S_OK);

        // 2D Sampler state
        ret = m_device->CreateSamplerState(&(D3D11_SAMPLER_DESC{
            D3D11_FILTER_MIN_MAG_MIP_LINEAR,
            D3D11_TEXTURE_ADDRESS_WRAP,
            D3D11_TEXTURE_ADDRESS_WRAP,
            D3D11_TEXTURE_ADDRESS_WRAP,
            0.f,
            1,
            D3D11_COMPARISON_ALWAYS,
            {0, 0, 0, 0},
            0,
            D3D11_FLOAT32_MAX
        }), &m_pSs2D);
        assert(ret == S_OK);
        ret = m_device->CreateSamplerState(&(D3D11_SAMPLER_DESC{
            D3D11_FILTER_MIN_MAG_MIP_LINEAR,
            D3D11_TEXTURE_ADDRESS_CLAMP,
            D3D11_TEXTURE_ADDRESS_CLAMP,
            D3D11_TEXTURE_ADDRESS_CLAMP,
            0.f,
            1,
            D3D11_COMPARISON_ALWAYS,
            {0, 0, 0, 0},
            0,
            D3D11_FLOAT32_MAX
        }), &m_pEffectsSampler);
        assert(ret == S_OK);
    }

    void Renderer::loadShaders()
    {
        // Create 2D shaders
        {
            auto ret = m_device->CreateVertexShader(_2dvs_cso, sizeof(_2dvs_cso), nullptr, &m_p2DVertexShader);
            assert(ret == S_OK);
            ret = m_device->CreatePixelShader(_2dps_cso, sizeof(_2dps_cso), nullptr, &m_p2DPixelShader);
            assert(ret == S_OK);

            // Create input layout
            D3D11_INPUT_ELEMENT_DESC layout[] = {
                {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
                {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
                {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            };
            ret = m_device->CreateInputLayout(layout, 3, _2dvs_cso, sizeof(_2dvs_cso), &m_p2DInputLayout);
            assert(ret == S_OK);
        }

        // Effects
        {       
            auto ret = m_device->CreateVertexShader(blurvs_cso, sizeof(blurvs_cso), nullptr, &m_pEffectsVertexShader);
            ret = m_device->CreatePixelShader(blurhps_cso, sizeof(blurhps_cso), nullptr, &m_pBlurHPixelShader);
            ret = m_device->CreatePixelShader(blurvps_cso, sizeof(blurvps_cso), nullptr, &m_pBlurVPixelShader);
            ret = m_device->CreatePixelShader(sepia_cso, sizeof(sepia_cso), nullptr, &m_pSepiaPixelShader);
            ret = m_device->CreatePixelShader(crt_cso, sizeof(crt_cso), nullptr, &m_pCRTPixelShader);
            ret = m_device->CreatePixelShader(cartoon_cso, sizeof(cartoon_cso), nullptr, &m_pCartoonPixelShader);
            ret = m_device->CreatePixelShader(vignette_cso, sizeof(vignette_cso), nullptr, &m_pVignettePixelShader);
            //m_pVignettePixelShader = create2DShader("../../../Debug/vignette.cso");

            // Create input layout
            D3D11_INPUT_ELEMENT_DESC layout[] = {
                {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            };
            ret = m_device->CreateInputLayout(layout, 1, blurvs_cso, sizeof(blurvs_cso), &m_pEffectsInputLayout);
            assert(ret == S_OK);
        }
    }

    ID3D11PixelShader* Renderer::create2DShader(const std::string& filename)
    {
        ID3D11PixelShader* pRet = nullptr;

        std::ifstream psFile(filename, std::ios::binary);
        std::vector<char> psData = {std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>()};

        auto ret = m_device->CreatePixelShader(psData.data(), psData.size(), nullptr, &pRet);
        assert(ret == S_OK);

        return pRet;
    }

    void Renderer::createUniforms()
    {
        // 2D view projection matrix
        {
            Matrix viewProj = Matrix::CreateOrthographicOffCenter(0, static_cast<float>(getResolution().x), static_cast<float>(getResolution().y), 0, -999, 999);
            viewProj = viewProj.Transpose();
            D3D11_BUFFER_DESC cbDesc = CD3D11_BUFFER_DESC(sizeof(Matrix), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
            D3D11_SUBRESOURCE_DATA initData{&viewProj, 0, 0};
            auto ret = m_device->CreateBuffer(&cbDesc, &initData, &m_pViewProj2dBuffer);
            assert(ret == S_OK);
        }

        // For effects
        {
            D3D11_BUFFER_DESC cbDesc = CD3D11_BUFFER_DESC(sizeof(Vector4), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
            auto ret = m_device->CreateBuffer(&cbDesc, nullptr, &m_pKernelSizeBuffer);
            assert(ret == S_OK);
        }
        {
            D3D11_BUFFER_DESC cbDesc = CD3D11_BUFFER_DESC(sizeof(Vector4) * 2, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
            auto ret = m_device->CreateBuffer(&cbDesc, nullptr, &m_pSepiaBuffer);
            assert(ret == S_OK);
        }
    }

    void Renderer::beginFrame()
    {
        // Bind render target
        //m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
        if (!m_renderTarget) m_renderTarget = OTexture::createScreenRenderTarget(false);
        m_renderTarget->bindRenderTarget();

        // Set viewport
        auto viewport = CD3D11_VIEWPORT(0.f, 0.f, (float)m_backBufferDesc.Width, (float)m_backBufferDesc.Height);
        m_deviceContext->RSSetViewports(1, &viewport);

        // Reset 2d view
        set2DCamera(Vector2::Zero);
    }

    void Renderer::endFrame()
    {
        m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
        OSB->begin();
        OSB->drawRect(m_renderTarget, ORectFullScreen);
        OSB->end();

        // Swap the buffer!
        m_swapChain->Present(1, 0);
    }

    void Renderer::bindRenderTarget(Texture *pTexture)
    {
        if (pTexture)
        {
            pTexture->bindRenderTarget();
        }
        else
        {
            m_renderTarget->bindRenderTarget();
            //m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
        }
    }

    ID3D11Device* Renderer::getDevice()
    {
        return m_device;
    }

    ID3D11DeviceContext* Renderer::getDeviceContext()
    {
        return m_deviceContext;
    }

    POINT Renderer::getResolution() const
    {
        return{m_backBufferDesc.Width, m_backBufferDesc.Height};
    }

    void Renderer::clear(const Color& color)
    {
        m_renderTarget->clearRenderTarget(color);
        //m_deviceContext->ClearRenderTargetView(m_renderTargetView, &color.x);
    }

    void Renderer::resetState()
    {
        m_renderSetup = eRenderSetup::SETUP_NONE;
    }

    void Renderer::setupFor2D()
    {
        setupFor2D(Matrix::Identity);
    }

    void Renderer::setupFor2D(const Matrix& transform)
    {
        // Bind the matrix
        set2DCamera(m_2dViewProj, transform);
        m_deviceContext->VSSetConstantBuffers(0, 1, &m_pViewProj2dBuffer);

        if (m_renderSetup == eRenderSetup::SETUP_2D) return;
        m_renderSetup = eRenderSetup::SETUP_2D;

        // Set 2d render states
        m_deviceContext->OMSetDepthStencilState(m_pDs2D, 1);
        m_deviceContext->RSSetState(m_pSr2D);
        m_deviceContext->OMSetBlendState(m_pBs2D, NULL, 0xffffffff);
        m_deviceContext->PSSetSamplers(0, 1, &m_pSs2D);

        // Bind the shaders
        m_deviceContext->IASetInputLayout(m_p2DInputLayout);
        m_deviceContext->VSSetShader(m_p2DVertexShader, nullptr, 0);
        m_deviceContext->PSSetShader(m_p2DPixelShader, nullptr, 0);

        m_cameraPos = Vector3::UnitZ;
        m_cameraDir = -Vector3::UnitZ;
        m_cameraUp = -Vector3::UnitY;
    }

    Matrix Renderer::build2DCamera(const Vector2& position, float zoom)
    {
        auto proj = Matrix::CreateOrthographicOffCenter(0, static_cast<float>(getResolution().x), static_cast<float>(getResolution().y), 0, -999, 999);
        auto view = Matrix::CreateTranslation(-position) * Matrix::CreateScale(zoom);
        view.Invert();
        auto viewProj = view * proj;
        viewProj = viewProj.Transpose();

        return viewProj;
    }

    Matrix Renderer::build2DCameraOffCenter(const Vector2& position, float zoom)
    {
        auto proj = Matrix::CreateOrthographicOffCenter(0, static_cast<float>(getResolution().x), static_cast<float>(getResolution().y), 0, -999, 999);
        auto view = Matrix::CreateTranslation(-position) *
            Matrix::CreateScale(zoom) * 
            Matrix::CreateTranslation({static_cast<float>(getResolution().x) * .5f, static_cast<float>(getResolution().y) * .5f, 0.f});
        view.Invert();
        auto viewProj = view * proj;
        viewProj = viewProj.Transpose();

        return viewProj;
    }

    void Renderer::set2DCamera(const Matrix& viewProj, const Matrix& transform)
    {
        D3D11_MAPPED_SUBRESOURCE map;
        m_deviceContext->Map(m_pViewProj2dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        auto finalTransform = viewProj * transform.Transpose();
        memcpy(map.pData, &finalTransform._11, sizeof(finalTransform));
        m_deviceContext->Unmap(m_pViewProj2dBuffer, 0);
    }

    Matrix Renderer::set2DCamera(const Vector2& position, float zoom)
    {
        m_2dViewProj = build2DCamera(position, zoom);
        set2DCamera(m_2dViewProj);
        return m_2dViewProj;
    }

    Matrix Renderer::set2DCameraOffCenter(const Vector2& position, float zoom)
    {
        m_2dViewProj = build2DCameraOffCenter(position, zoom);
        set2DCamera(m_2dViewProj);
        return m_2dViewProj;
    }

    void Renderer::setupFor3D()
    {
        assert(false); // 3D not implemented

        if (m_renderSetup == eRenderSetup::SETUP_3D) return;
        m_renderSetup = eRenderSetup::SETUP_3D;
    }

    void Renderer::setScissor(bool enabled, const Rect& rect)
    {
        m_scissorEnabled = enabled;
        m_scissor = rect;
        if (enabled)
        {
            D3D11_RECT dxRect[1] = {
                {
                    static_cast<LONG>(rect.x),
                    static_cast<LONG>(rect.y), 
                    static_cast<LONG>(rect.x + rect.z),
                    static_cast<LONG>(rect.y + rect.w),
                }
            };
            m_deviceContext->RSSetScissorRects(1, dxRect);
            m_deviceContext->RSSetState(m_pSr2DScissor);
        }
        else
        {
            m_deviceContext->RSSetState(m_pSr2D);
        }
    }

    void Renderer::setKernelSize(const Vector2& kernelSize)
    {
        D3D11_MAPPED_SUBRESOURCE map;
        m_deviceContext->Map(m_pKernelSizeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        Vector4 kernelSize4(kernelSize.x, kernelSize.y, 0, 0);
        memcpy(map.pData, &kernelSize4.x, sizeof(kernelSize4));
        m_deviceContext->Unmap(m_pKernelSizeBuffer, 0);
        m_deviceContext->PSSetConstantBuffers(0, 1, &m_pKernelSizeBuffer);
    }

    void Renderer::setCRT(const Vector2& resolution)
    {
        struct sCRT
        {
            Vector2 res;
            Vector2 pixel;
        };
        sCRT crt = {{resolution.x, resolution.y}, {1.f / resolution.x, 1.f / resolution.y}};

        D3D11_MAPPED_SUBRESOURCE map;
        m_deviceContext->Map(m_pKernelSizeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        memcpy(map.pData, &crt, sizeof(crt));
        m_deviceContext->Unmap(m_pKernelSizeBuffer, 0);
        m_deviceContext->PSSetConstantBuffers(0, 1, &m_pKernelSizeBuffer);
    }

    void Renderer::setCartoon(const Vector3& tone)
    {
        struct sCartoon
        {
            Vector3 tone;
            float padding;
        };
        sCartoon cartoon = {tone};

        D3D11_MAPPED_SUBRESOURCE map;
        m_deviceContext->Map(m_pKernelSizeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        memcpy(map.pData, &cartoon, sizeof(cartoon));
        m_deviceContext->Unmap(m_pKernelSizeBuffer, 0);
        m_deviceContext->PSSetConstantBuffers(0, 1, &m_pKernelSizeBuffer);
    }

    void Renderer::setVignette(const Vector2& kernelSize, float amount)
    {
        struct sVignette
        {
            Vector2 kernelSize;
            float amount;
            float padding;
        };
        sVignette vignette = {kernelSize, amount};

        D3D11_MAPPED_SUBRESOURCE map;
        m_deviceContext->Map(m_pKernelSizeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        memcpy(map.pData, &vignette, sizeof(vignette));
        m_deviceContext->Unmap(m_pKernelSizeBuffer, 0);
        m_deviceContext->PSSetConstantBuffers(0, 1, &m_pKernelSizeBuffer);
    }

    void Renderer::setSepia(const Vector3& tone, float saturation, float sepiaAmount)
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
        m_deviceContext->Map(m_pSepiaBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        memcpy(map.pData, &sepia, sizeof(sepia));
        m_deviceContext->Unmap(m_pSepiaBuffer, 0);
        m_deviceContext->PSSetConstantBuffers(0, 1, &m_pSepiaBuffer);
    }

    void Renderer::drawBlurH()
    {
        // Set 2d render states
        m_deviceContext->OMSetDepthStencilState(m_pDs2D, 1);
        m_deviceContext->RSSetState(m_pSr2D);
        m_deviceContext->OMSetBlendState(m_pBs2D, NULL, 0xffffffff);
        m_deviceContext->PSSetSamplers(0, 1, &m_pEffectsSampler);

        // Bind the shaders
        m_deviceContext->IASetInputLayout(m_pEffectsInputLayout);
        m_deviceContext->VSSetShader(m_pEffectsVertexShader, nullptr, 0);
        m_deviceContext->PSSetShader(m_pBlurHPixelShader, nullptr, 0);

        UINT stride = 2 * 4;
        UINT offset = 0;
        m_deviceContext->IASetVertexBuffers(0, 1, &m_pEffectsVertexBuffer, &stride, &offset);
        m_deviceContext->Draw(6, 0);
    }

    void Renderer::drawBlurV()
    {
        // Set 2d render states
        m_deviceContext->OMSetDepthStencilState(m_pDs2D, 1);
        m_deviceContext->RSSetState(m_pSr2D);
        m_deviceContext->OMSetBlendState(m_pBs2D, NULL, 0xffffffff);
        m_deviceContext->PSSetSamplers(0, 1, &m_pEffectsSampler);

        // Bind the shaders
        m_deviceContext->IASetInputLayout(m_pEffectsInputLayout);
        m_deviceContext->VSSetShader(m_pEffectsVertexShader, nullptr, 0);
        m_deviceContext->PSSetShader(m_pBlurVPixelShader, nullptr, 0);

        UINT stride = 2 * 4;
        UINT offset = 0;
        m_deviceContext->IASetVertexBuffers(0, 1, &m_pEffectsVertexBuffer, &stride, &offset);
        m_deviceContext->Draw(6, 0);
    }

    void Renderer::drawSepia()
    {
        // Set 2d render states
        m_deviceContext->OMSetDepthStencilState(m_pDs2D, 1);
        m_deviceContext->RSSetState(m_pSr2D);
        m_deviceContext->OMSetBlendState(m_pBs2D, NULL, 0xffffffff);
        m_deviceContext->PSSetSamplers(0, 1, &m_pEffectsSampler);

        // Bind the shaders
        m_deviceContext->IASetInputLayout(m_pEffectsInputLayout);
        m_deviceContext->VSSetShader(m_pEffectsVertexShader, nullptr, 0);
        m_deviceContext->PSSetShader(m_pSepiaPixelShader, nullptr, 0);

        UINT stride = 2 * 4;
        UINT offset = 0;
        m_deviceContext->IASetVertexBuffers(0, 1, &m_pEffectsVertexBuffer, &stride, &offset);
        m_deviceContext->Draw(6, 0);
    }

    void Renderer::drawCRT()
    {
        // Set 2d render states
        m_deviceContext->OMSetDepthStencilState(m_pDs2D, 1);
        m_deviceContext->RSSetState(m_pSr2D);
        m_deviceContext->OMSetBlendState(m_pBs2D, NULL, 0xffffffff);
        m_deviceContext->PSSetSamplers(0, 1, &m_pEffectsSampler);

        // Bind the shaders
        m_deviceContext->IASetInputLayout(m_pEffectsInputLayout);
        m_deviceContext->VSSetShader(m_pEffectsVertexShader, nullptr, 0);
        m_deviceContext->PSSetShader(m_pCRTPixelShader, nullptr, 0);

        UINT stride = 2 * 4;
        UINT offset = 0;
        m_deviceContext->IASetVertexBuffers(0, 1, &m_pEffectsVertexBuffer, &stride, &offset);
        m_deviceContext->Draw(6, 0);
    }

    void Renderer::drawCartoon()
    {
        // Set 2d render states
        m_deviceContext->OMSetDepthStencilState(m_pDs2D, 1);
        m_deviceContext->RSSetState(m_pSr2D);
        m_deviceContext->OMSetBlendState(m_pBs2D, NULL, 0xffffffff);
        m_deviceContext->PSSetSamplers(0, 1, &m_pEffectsSampler);

        // Bind the shaders
        m_deviceContext->IASetInputLayout(m_pEffectsInputLayout);
        m_deviceContext->VSSetShader(m_pEffectsVertexShader, nullptr, 0);
        m_deviceContext->PSSetShader(m_pCartoonPixelShader, nullptr, 0);

        UINT stride = 2 * 4;
        UINT offset = 0;
        m_deviceContext->IASetVertexBuffers(0, 1, &m_pEffectsVertexBuffer, &stride, &offset);
        m_deviceContext->Draw(6, 0);
    }

    void Renderer::drawVignette()
    {
        // Set 2d render states
        m_deviceContext->OMSetDepthStencilState(m_pDs2D, 1);
        m_deviceContext->RSSetState(m_pSr2D);
        m_deviceContext->OMSetBlendState(m_pBs2D, NULL, 0xffffffff);
        m_deviceContext->PSSetSamplers(0, 1, &m_pEffectsSampler);

        // Bind the shaders
        m_deviceContext->IASetInputLayout(m_pEffectsInputLayout);
        m_deviceContext->VSSetShader(m_pEffectsVertexShader, nullptr, 0);
        m_deviceContext->PSSetShader(m_pVignettePixelShader, nullptr, 0);

        UINT stride = 2 * 4;
        UINT offset = 0;
        m_deviceContext->IASetVertexBuffers(0, 1, &m_pEffectsVertexBuffer, &stride, &offset);
        m_deviceContext->Draw(6, 0);
    }
}
