#include "onut.h"
#include "Renderer.h"
#include "Window.h"

#include <fstream>
#include <vector>

namespace onut
{
    Renderer::Renderer(Window& window)
    {
        createDevice(window);
        createRenderTarget();
        createRenderStates();
        loadShaders();
        createUniforms();
    }

    Renderer::~Renderer()
    {
#ifdef EASY_GRAPHIX
        egDestroyDevice(&m_device);
#else
        if (m_pWorldMatrixBuffer) m_pWorldMatrixBuffer->Release();
        if (m_pViewProj2dBuffer) m_pViewProj2dBuffer->Release();

        if (m_p2DInputLayout) m_p2DInputLayout->Release();
        if (m_p2DPixelShader) m_p2DPixelShader->Release();
        if (m_p2DVertexShader) m_p2DVertexShader->Release();

        if (m_pSs2D) m_pSs2D->Release();
        if (m_pBs2D) m_pBs2D->Release();
        if (m_pSr2D) m_pSr2D->Release();
        if (m_pSr2DScissor) m_pSr2DScissor->Release();
        if (m_pDs2D) m_pDs2D->Release();

        if (m_renderTargetView) m_renderTargetView->Release();

        if (m_deviceContext) m_deviceContext->Release();
        if (m_device) m_device->Release();
        if (m_swapChain) m_swapChain->Release();
#endif;
    }

    void Renderer::createDevice(Window& window)
    {
#ifdef EASY_GRAPHIX
        m_device = egCreateDevice(window.getHandle());

        // Check for error
        if (!m_device)
        {
            MessageBox(nullptr, L"egCreateDevice", L"Error", MB_OK);
            exit(0);
        }
#else
        // Define our swap chain
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};
        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = window.getHandle();
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.Windowed = true;

        // Create the swap chain, device and device context
        auto result = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 
#if _DEBUG
            D3D11_CREATE_DEVICE_DEBUG,
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
#endif /* !EASY_GRAPHIX */
    }

    void Renderer::createRenderTarget()
    {
#ifndef EASY_GRAPHIX
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
#endif /* !EASY_GRAPHIX */
    }

    void Renderer::onResize()
    {
#ifdef EASY_GRAPHIX
        egResize();
#else /* EASY_GRAPHIX */
        m_renderTargetView->Release();
        m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
        createRenderTarget();
#endif /* !EASY_GRAPHIX */
    }

    void Renderer::createRenderStates()
    {
#ifdef EASY_GRAPHIX
        egDisable(EG_ALL);
        egEnable(EG_BLEND);
        egBlendFunc(EG_ONE, EG_ONE_MINUS_SRC_ALPHA);
        m_2dstate = egCreateState();
        assert(m_2dstate);

        egDisable(EG_ALL);
        egEnable(EG_LIGHTING | EG_DEPTH_TEST | EG_DEPTH_WRITE);
        m_3dState = egCreateState();
        assert(m_3dState);
#else /* EASY_GRAPHIX */
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
#endif /* !EASY_GRAPHIX */
    }

    void Renderer::loadShaders()
    {
#ifndef EASY_GRAPHIX
        // Create 2D shaders
        {
            std::ifstream vsFile("../../assets/shaders/2dvs.cso", std::ios::binary);
            std::vector<char> vsData = {std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>()};

            std::ifstream psFile("../../assets/shaders/2dps.cso", std::ios::binary);
            std::vector<char> psData = {std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>()};

            auto ret = m_device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &m_p2DVertexShader);
            assert(ret == S_OK);
            ret = m_device->CreatePixelShader(psData.data(), psData.size(), nullptr, &m_p2DPixelShader);
            assert(ret == S_OK);

            // Create input layout
            D3D11_INPUT_ELEMENT_DESC layout[] = {
                {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
                {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
                {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            };
            ret = m_device->CreateInputLayout(layout, 3, vsData.data(), vsData.size(), &m_p2DInputLayout);
            assert(ret == S_OK);
        }
#endif /* !EASY_GRAPHIX */
    }

#ifndef EASY_GRAPHIX
    ID3D11PixelShader* Renderer::create2DShader(const std::string& filename)
    {
        ID3D11PixelShader* pRet = nullptr;

        std::ifstream psFile("../../assets/shaders/" + filename, std::ios::binary);
        std::vector<char> psData = {std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>()};

        auto ret = m_device->CreatePixelShader(psData.data(), psData.size(), nullptr, &pRet);
        assert(ret == S_OK);

        return pRet;
    }
#endif /* !EASY_GRAPHIX */

    void Renderer::createUniforms()
    {
#ifndef EASY_GRAPHIX
        // 2D view projection matrix
        {
            Matrix viewProj = Matrix::CreateOrthographicOffCenter(0, static_cast<float>(getResolution().x), static_cast<float>(getResolution().y), 0, -999, 999);
            viewProj = viewProj.Transpose();
            D3D11_BUFFER_DESC cbDesc = CD3D11_BUFFER_DESC(sizeof(Matrix), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
            D3D11_SUBRESOURCE_DATA initData{&viewProj, 0, 0};
            auto ret = m_device->CreateBuffer(&cbDesc, &initData, &m_pViewProj2dBuffer);
            assert(ret == S_OK);
        }
#endif /* !EASY_GRAPHIX */
    }

    void Renderer::beginFrame()
    {
#ifdef EASY_GRAPHIX
        auto resolution = getResolution();
        egViewPort(0, 0, static_cast<uint32_t>(resolution.x), static_cast<uint32_t>(resolution.y));
        m_renderSetup = eRenderSetup::SETUP_NONE;
#else /* EASY_GRAPHIX */
        // Bind render target
        m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);

        // Set viewport
        auto viewport = CD3D11_VIEWPORT(0.f, 0.f, (float)m_backBufferDesc.Width, (float)m_backBufferDesc.Height);
        m_deviceContext->RSSetViewports(1, &viewport);
#endif /* !EASY_GRAPHIX */

        // Reset 2d view
        set2DCamera(Vector2::Zero);
    }

    void Renderer::endFrame()
    {
#ifdef EASY_GRAPHIX
        egPostProcess();
        egSwap();
#else /* EASY_GRAPHIX */
        // Swap the buffer!
        m_swapChain->Present(1, 0);
#endif /* !EASY_GRAPHIX */
    }

    void Renderer::bindRenderTarget(Texture *pTexture)
    {
#ifdef EASY_GRAPHIX
        if (pTexture == nullptr)
        {
            egBindRenderTarget(0);
        }
        else
        {
            egBindRenderTarget(pTexture->getResource());
        }
#else /* EASY_GRAPHIX */
#endif /* !EASY_GRAPHIX */
    }

#ifdef EASY_GRAPHIX
    EGDevice Renderer::getDevice()
    {
        return m_device;
    }
#else /* EASY_GRAPHIX */
    ID3D11Device* Renderer::getDevice()
    {
        return m_device;
    }
#endif /* !EASY_GRAPHIX */

#ifndef EASY_GRAPHIX
    ID3D11DeviceContext* Renderer::getDeviceContext()
    {
        return m_deviceContext;
    }
#endif /* !EASY_GRAPHIX */

    POINT Renderer::getResolution() const
    {
#ifdef EASY_GRAPHIX
        int res[2];
        egGetiv(EG_RESOLUTION, res);
        return {static_cast<LONG>(res[0]), static_cast<LONG>(res[1])};
#else /* EASY_GRAPHIX */
        return{m_backBufferDesc.Width, m_backBufferDesc.Height};
#endif /* !EASY_GRAPHIX */
    }

    void Renderer::clear(const Color& color)
    {
#ifdef EASY_GRAPHIX
        egClearColor(color.x, color.y, color.z, color.w);
        egClear(EG_CLEAR_ALL);
#else /* EASY_GRAPHIX */
        m_deviceContext->ClearRenderTargetView(m_renderTargetView, &color.x);
#endif /* !EASY_GRAPHIX */
    }

    void Renderer::resetState()
    {
        m_renderSetup = eRenderSetup::SETUP_NONE;
    }

    void Renderer::setupFor2D()
    {
        if (m_renderSetup == eRenderSetup::SETUP_2D) return;
        m_renderSetup = eRenderSetup::SETUP_2D;

#ifdef EASY_GRAPHIX
        egSet2DViewProj(-999, 999);
        egBindState(m_2dstate);
        egBindDiffuse(0);
        egBindNormal(0);
        egBindMaterial(0);
#else /* EASY_GRAPHIX */
        // Set 2d render states
        m_deviceContext->OMSetDepthStencilState(m_pDs2D, 1);
        m_deviceContext->RSSetState(m_pSr2D);
        m_deviceContext->OMSetBlendState(m_pBs2D, NULL, 0xffffffff);
        m_deviceContext->PSSetSamplers(0, 1, &m_pSs2D);

        // Bind the shaders
        m_deviceContext->IASetInputLayout(m_p2DInputLayout);
        m_deviceContext->VSSetShader(m_p2DVertexShader, nullptr, 0);
        m_deviceContext->PSSetShader(m_p2DPixelShader, nullptr, 0);

        // Bind the matrix
        m_deviceContext->VSSetConstantBuffers(0, 1, &m_pViewProj2dBuffer);
#endif /* !EASY_GRAPHIX */

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

    void Renderer::set2DCamera(const Matrix& viewProj)
    {
#ifdef EASY_GRAPHIX
        egSetViewProjMerged(&viewProj._11);
#else /* EASY_GRAPHIX */
        D3D11_MAPPED_SUBRESOURCE map;
        m_deviceContext->Map(m_pViewProj2dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        memcpy(map.pData, &viewProj._11, sizeof(viewProj));
        m_deviceContext->Unmap(m_pViewProj2dBuffer, 0);
#endif /* !EASY_GRAPHIX */
    }

    Matrix Renderer::set2DCamera(const Vector2& position, float zoom)
    {
        auto viewProj = build2DCamera(position, zoom);
        set2DCamera(viewProj);
        return viewProj;
    }

    Matrix Renderer::set2DCameraOffCenter(const Vector2& position, float zoom)
    {
        auto viewProj = build2DCameraOffCenter(position, zoom);
        set2DCamera(viewProj);
        return viewProj;
    }

    void Renderer::setupFor3D()
    {
        assert(false); // 3D not implemented

        if (m_renderSetup == eRenderSetup::SETUP_3D) return;
        m_renderSetup = eRenderSetup::SETUP_3D;
    }

    void Renderer::setScissor(bool enabled, const Rect& rect)
    {
#ifdef EASY_GRAPHIX
        if (enabled)
        {
            egEnable(EG_SCISSOR);
            egScissor(static_cast<uint32_t>(rect.x),
                      static_cast<uint32_t>(rect.y),
                      static_cast<uint32_t>(rect.z), 
                      static_cast<uint32_t>(rect.w));
        }
        else
        {
            egDisable(EG_SCISSOR);
        }
#else /* EASY_GRAPHIX */
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
#endif /* !EASY_GRAPHIX */
    }
}
