#pragma once
#include <d3d11.h>
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

namespace onut
{
    class Window;

    class Renderer
    {
    public:
        Renderer(Window& window);
        virtual ~Renderer();

        void                    clear(const Color& color = {.25f, .5f, 1, 1});

        void                    beginFrame();
        void                    endFrame();

        void                    setupFor2D();
        void                    setupFor3D();
        void                    set2DCamera(const Matrix& viewProj);
        Matrix                  set2DCamera(const Vector2& position, float zoom = 1.f);
        Matrix                  set2DCameraOffCenter(const Vector2& position, float zoom = 1.f);
        Matrix                  build2DCamera(const Vector2& position, float zoom = 1.f);
        Matrix                  build2DCameraOffCenter(const Vector2& position, float zoom = 1.f);

        void                    setScissor(bool enabled, const Rect& rect);

        Vector3                 getCameraPos() const { return m_cameraPos; }
        Vector3                 getCameraDir() const { return m_cameraDir; }
        Vector3                 getCameraUp() const { return m_cameraUp; }

        ID3D11Device*           getDevice();
        ID3D11DeviceContext*    getDeviceContext();

        POINT                   getResolution() const;

        void                    onResize();

        ID3D11PixelShader*      create2DShader(const std::string& filename);

    private:
        enum class eRenderSetup
        {
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
        ID3D11DepthStencilState*    m_pDs2D = nullptr;
        ID3D11RasterizerState*      m_pSr2D = nullptr;
        ID3D11RasterizerState*      m_pSr2DScissor = nullptr;
        ID3D11BlendState*           m_pBs2D = nullptr;
        ID3D11SamplerState*         m_pSs2D = nullptr;

        // Shaders
        ID3D11VertexShader*         m_p2DVertexShader = nullptr;
        ID3D11PixelShader*          m_p2DPixelShader = nullptr;
        ID3D11InputLayout*          m_p2DInputLayout = nullptr;

        // Constant buffers
        ID3D11Buffer*               m_pViewProj2dBuffer = nullptr;
        ID3D11Buffer*               m_pWorldMatrixBuffer = nullptr;

        eRenderSetup                m_renderSetup = eRenderSetup::SETUP_NONE;

        // Camera
        Vector3                     m_cameraPos;
        Vector3                     m_cameraDir;
        Vector3                     m_cameraUp;
    };
}
