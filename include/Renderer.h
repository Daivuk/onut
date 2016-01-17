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
        bool                    getScissorEnabled() const { return m_scissorEnabled; }
        const Rect&             getScissor() const { return m_scissor; }

        Vector3                 getCameraPos() const { return m_cameraPos; }
        Vector3                 getCameraDir() const { return m_cameraDir; }
        Vector3                 getCameraUp() const { return m_cameraUp; }

        POINT                   getResolution() const;
        void                    onResize(const POINT& newSize);

        void                    bindRenderTarget(Texture *pTexture = nullptr);

        ID3D11Device*           getDevice();
        ID3D11DeviceContext*    getDeviceContext();
        ID3D11PixelShader*      create2DShader(const std::string& filename);
        Texture*                getRenderTarget() const { return m_renderTarget; }

        // For effects
        void                    setKernelSize(const Vector2& kernelSize);
        void                    setSepia(const Vector3& tone = Vector3(1.40f, 1.10f, 0.90f), // 0 - 2.55
                                         float saturation = .25f, // 0 - 1
                                         float sepiaAmount = .75f); // 0 - 1
        void                    setCRT(const Vector2& kernelSize);
        void                    setCartoon(const Vector3& tone);
        void                    setVignette(const Vector2& kernelSize, float amount = .5f);
        void                    drawBlurH();
        void                    drawBlurV();
        void                    drawSepia();
        void                    drawCRT();
        void                    drawCartoon();
        void                    drawVignette();

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
        Texture*                    m_renderTarget = nullptr;

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
        ID3D11PixelShader*          m_pSepiaPixelShader = nullptr;
        ID3D11PixelShader*          m_pCRTPixelShader = nullptr;
        ID3D11PixelShader*          m_pCartoonPixelShader = nullptr;
        ID3D11PixelShader*          m_pVignettePixelShader = nullptr;
        ID3D11InputLayout*          m_pEffectsInputLayout = nullptr;
        ID3D11Buffer*               m_pEffectsVertexBuffer = nullptr;
        ID3D11SamplerState*         m_pEffectsSampler = nullptr;

        // Constant buffers
        ID3D11Buffer*               m_pViewProj2dBuffer = nullptr;
        ID3D11Buffer*               m_pWorldMatrixBuffer = nullptr;
        ID3D11Buffer*               m_pKernelSizeBuffer = nullptr;
        ID3D11Buffer*               m_pSepiaBuffer = nullptr;

        eRenderSetup                m_renderSetup = eRenderSetup::SETUP_NONE;

        // Camera
        Vector3                     m_cameraPos;
        Vector3                     m_cameraDir;
        Vector3                     m_cameraUp;
        Matrix                      m_2dViewProj;

        bool                        m_scissorEnabled = false;
        Rect                        m_scissor;
    };
}
