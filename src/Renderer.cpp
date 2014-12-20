#include "Renderer.h"
#include "Window.h"

#include <fstream>
#include <vector>

namespace onut {
    Renderer::Renderer(Window& window) {
        createDevice(window);
        createRenderTarget();
        createRenderStates();
        loadShaders();
        createUniforms();
    }

    void Renderer::createDevice(Window& window) {
        // Define our swap chain
        DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = window.getHandle();
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.Windowed = true;

        // Create the swap chain, device and device context
        auto result = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
            nullptr, 0, D3D11_SDK_VERSION,
            &swapChainDesc, &m_swapChain,
            &m_device, nullptr, &m_deviceContext);

        // Check for error
        if (result != S_OK) {
            MessageBox(nullptr, L"D3D11CreateDeviceAndSwapChain", L"Error", MB_OK);
            exit(0);
        }
    }

    void Renderer::createRenderTarget() {
        ID3D11Texture2D* backBuffer;
        auto result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
        if (result != S_OK) {
            MessageBox(nullptr, L"m_swapChain->GetBuffer", L"Error", MB_OK);
            exit(0);
        }
        result = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
        if (result != S_OK) {
            MessageBox(nullptr, L"m_device->CreateRenderTargetView", L"Error", MB_OK);
            exit(0);
        }

        backBuffer->GetDesc(&m_backBufferDesc);
        backBuffer->Release();
    }

    void Renderer::createRenderStates() {
        // 2D Depth state
        assert(m_device->CreateDepthStencilState(&(D3D11_DEPTH_STENCIL_DESC{
            false,
            D3D11_DEPTH_WRITE_MASK_ZERO,
            D3D11_COMPARISON_LESS,
            false,
            0xFF,
            0xFF,
            { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_INCR, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS },
            { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_DECR, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS }
        }), &m_pDs2D) == S_OK);

        // 2D Rasterizer state
        assert(m_device->CreateRasterizerState(&(D3D11_RASTERIZER_DESC{
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
        }), &m_pSr2D) == S_OK);

        // 2D Blend state
        assert(m_device->CreateBlendState(&(D3D11_BLEND_DESC{
            FALSE,
            FALSE,
            { {
                    TRUE,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_INV_SRC_ALPHA,
                    D3D11_BLEND_OP_ADD,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_OP_ADD,
                    D3D10_COLOR_WRITE_ENABLE_ALL
                }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 } }
        }), &m_pBs2D) == S_OK);

        // 2D Sampler state
        assert(m_device->CreateSamplerState(&(D3D11_SAMPLER_DESC{
            D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
            D3D11_TEXTURE_ADDRESS_CLAMP,
            D3D11_TEXTURE_ADDRESS_CLAMP,
            D3D11_TEXTURE_ADDRESS_CLAMP,
            0.f,
            1,
            D3D11_COMPARISON_ALWAYS,
            { 0, 0, 0, 0 },
            0,
            D3D11_FLOAT32_MAX
        }), &m_pSs2D) == S_OK);
    }

    void Renderer::loadShaders() {
        // Create 2D shaders
        {
            std::ifstream vsFile("assets/shaders/2dvs.cso", std::ios::binary);
            std::vector<char> vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };

            std::ifstream psFile("assets/shaders/2dps.cso", std::ios::binary);
            std::vector<char> psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

            assert(m_device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &m_p2DVertexShader) == S_OK);
            assert(m_device->CreatePixelShader(psData.data(), psData.size(), nullptr, &m_p2DPixelShader) == S_OK);

            // Create input layout
            D3D11_INPUT_ELEMENT_DESC layout[] = {
                { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };
            assert(m_device->CreateInputLayout(layout, 3, vsData.data(), vsData.size(), &m_p2DInputLayout) == S_OK);
        }
    }

    void Renderer::createUniforms() {
        // 2D view projection matrix
        {
            Matrix viewProj = Matrix::CreateOrthographicOffCenter(0, static_cast<float>(getResolution().x), static_cast<float>(getResolution().y), 0, -999, 999);
            viewProj = viewProj.Transpose();
            D3D11_BUFFER_DESC cbDesc = CD3D11_BUFFER_DESC(sizeof(Matrix), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
            D3D11_SUBRESOURCE_DATA initData{ &viewProj, 0, 0 };
            assert(m_device->CreateBuffer(&cbDesc, &initData, &m_pViewProj2dBuffer) == S_OK);
        }
    }

    void Renderer::beginFrame() {
        // Bind render target
        m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);

        // Set viewport
        auto viewport = CD3D11_VIEWPORT(0.f, 0.f, (float)m_backBufferDesc.Width, (float)m_backBufferDesc.Height);
        m_deviceContext->RSSetViewports(1, &viewport);
    }

    void Renderer::endFrame() {
        // Swap the buffer!
        m_swapChain->Present(0, 0);
    }

    ID3D11Device* Renderer::getDevice() {
        return m_device;
    }

    ID3D11DeviceContext* Renderer::getDeviceContext() {
        return m_deviceContext;
    }

    POINT Renderer::getResolution() const {
        return{ m_backBufferDesc.Width, m_backBufferDesc.Height };
    }

    void Renderer::clear(const Vector4& color) {
        m_deviceContext->ClearRenderTargetView(m_renderTargetView, &color.x);
    }

    void Renderer::setupFor2D() {
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
    }

    void Renderer::setupFor3D() {
        assert(false); // 3D not implemented
    }
}
