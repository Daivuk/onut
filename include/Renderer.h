#pragma once
#include <d3d11.h>
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

namespace onut
{
    class Window;
    class Texture;

    class Renderer
    {
    public:
        Renderer(Window& window);
        virtual ~Renderer();

        void                    clear(const Color& color = {.25f, .5f, 1, 1});

        void                    beginFrame();
        void                    endFrame();

        void                    resetState();
        void                    setupFor2D();
        void                    setupFor2D(const Matrix& transform);
        void                    setupFor3D();
        void                    set2DCamera(const Matrix& viewProj, const Matrix& transform = Matrix::Identity);
        Matrix                  set2DCamera(const Vector2& position, float zoom = 1.f);
        Matrix                  set2DCameraOffCenter(const Vector2& position, float zoom = 1.f);
        Matrix                  build2DCamera(const Vector2& position, float zoom = 1.f);
        Matrix                  build2DCameraOffCenter(const Vector2& position, float zoom = 1.f);

        void                    setScissor(bool enabled, const Rect& rect);

        Vector3                 getCameraPos() const { return m_cameraPos; }
        Vector3                 getCameraDir() const { return m_cameraDir; }
        Vector3                 getCameraUp() const { return m_cameraUp; }

        POINT                   getResolution() const;
        void                    onResize();

        void                    bindRenderTarget(Texture *pTexture = nullptr);

        ID3D11Device*           getDevice();
        ID3D11DeviceContext*    getDeviceContext();
        ID3D11PixelShader*      create2DShader(const std::string& filename);

        // For effects
        void                    setKernelSize(const Vector2& kernelSize);
        void                    drawBlurH();
        void                    drawBlurV();

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

        ID3D11VertexShader*         m_pEffectsVertexShader = nullptr;
        ID3D11PixelShader*          m_pBlurHPixelShader = nullptr;
        ID3D11PixelShader*          m_pBlurVPixelShader = nullptr;
        ID3D11InputLayout*          m_pEffectsInputLayout = nullptr;
        ID3D11Buffer*               m_pEffectsVertexBuffer = nullptr;

        // Constant buffers
        ID3D11Buffer*               m_pViewProj2dBuffer = nullptr;
        ID3D11Buffer*               m_pWorldMatrixBuffer = nullptr;
        ID3D11Buffer*               m_pKernelSizeBuffer = nullptr;

        eRenderSetup                m_renderSetup = eRenderSetup::SETUP_NONE;

        // Camera
        Vector3                     m_cameraPos;
        Vector3                     m_cameraDir;
        Vector3                     m_cameraUp;
        Matrix                      m_2dViewProj;
    };
}
