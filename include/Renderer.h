#pragma once
#include <d3d11.h>

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

namespace onut {
    class Window;

    class Renderer {
    public:
        Renderer(Window& window);

        void                    clear(const Color& color = { .25f, .5f, 1, 1 });

        void                    beginFrame();
        void                    endFrame();
        void                    setupFor2D();
        void                    setupFor3D();

        ID3D11Device*           getDevice();
        ID3D11DeviceContext*    getDeviceContext();

        POINT                   getResolution() const;

    private:
        enum class eRenderSetup {
            SETUP_NONE,
            SETUP_2D,
            SETUP_3D
        };

        void                    createDevice(Window& window);
        void                    createRenderTarget();
        void                    createRenderStates();
        void                    loadShaders();
        void                    createUniforms();

        // Device stuff
        IDXGISwapChain*             m_swapChain = nullptr;
        ID3D11Device*               m_device = nullptr;
        ID3D11DeviceContext*        m_deviceContext = nullptr;

        // Render target
        ID3D11RenderTargetView*     m_renderTargetView = nullptr;
        D3D11_TEXTURE2D_DESC        m_backBufferDesc;

        // Render states
        ID3D11DepthStencilState*    m_pDs2D;
        ID3D11RasterizerState*      m_pSr2D;
        ID3D11BlendState*           m_pBs2D;
        ID3D11SamplerState*         m_pSs2D;

        // Shaders
        ID3D11VertexShader*         m_p2DVertexShader = nullptr;
        ID3D11PixelShader*          m_p2DPixelShader = nullptr;
        ID3D11InputLayout*          m_p2DInputLayout = nullptr;

        // Constant buffers
        ID3D11Buffer*               m_pViewProj2dBuffer;
        ID3D11Buffer*               m_pWorldMatrixBuffer;

        eRenderSetup                m_renderSetup = eRenderSetup::SETUP_NONE;
    };
}
