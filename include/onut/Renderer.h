#pragma once
// Onut
#include <onut/BlendMode.h>
#include <onut/Maths.h>
#include <onut/Point.h>
#include <onut/PrimitiveMode.h>
#include <onut/SampleMode.h>

// STL
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(IndexBuffer)
OForwardDeclare(Renderer)
OForwardDeclare(Shader)
OForwardDeclare(Texture)
OForwardDeclare(VertexBuffer)
OForwardDeclare(Window)

namespace onut
{
    class Window;

    template<typename Ttype>
    class RenderState
    {
    public:
        RenderState() {}
        RenderState(const Ttype& value) : m_value(value) {}
        RenderState(const RenderState& other) : m_value(other.m_value) {}

        Ttype& operator=(const Ttype& value)
        {
            if (value != m_value)
            {
                m_value = value;
                m_isDirty = true;
            }
            return m_value;
        }

        RenderState& operator=(const RenderState& other)
        {
            *this = other.m_value;
            return *this;
        }

        const Ttype& get() const
        {
            return m_value;
        }

        operator const Ttype&() const
        {
            return m_value;
        }

        bool isDirty() const
        {
            return m_isDirty;
        }

        void resetDirty()
        {
            m_isDirty = false;
        }

        void forceDirty()
        {
            m_isDirty = true;
        }

        void push(const Ttype& value)
        {
            m_stack.push_back(m_value);
            *this = value;
        }

        void pop()
        {
            if (!m_stack.empty())
            {
                *this = m_stack.back();
                m_stack.pop_back();
            }
        }

    private:
        std::vector<Ttype> m_stack;
        Ttype m_value;
        bool m_isDirty = true;
    };

    class RenderStates
    {
    public:
        static const int MAX_TEXTURES = 8;

        RenderStates();
        RenderStates(const RenderStates& other);
        RenderStates& operator=(const RenderStates& other);

        void reset();

        RenderState<OTextureRef> textures[MAX_TEXTURES];
        RenderState<BlendMode> blendMode;
        RenderState<sample::Filtering> sampleFiltering;
        RenderState<sample::AddressMode> sampleAddressMode;
        RenderState<iRect> viewport;
        RenderState<iRect> scissor;
        RenderState<Matrix> viewProjection;
        RenderState<Matrix> world;
        RenderState<bool> depthEnabled;
        RenderState<bool> scissorEnabled;
        RenderState<PrimitiveMode> primitiveMode;
        RenderState<OShaderRef> vertexShader;
        RenderState<OShaderRef> pixelShader;
        RenderState<OVertexBufferRef> vertexBuffer;
        RenderState<OIndexBufferRef> indexBuffer;
        RenderState<OTextureRef> renderTarget;
    };

    class Renderer
    {
    public:
        static ORendererRef create(const OWindowRef& pWindow);

        virtual ~Renderer();

        void setupFor2D();
        void setupFor2D(const Matrix& transform);
        void set2DCamera(const Matrix& viewProj, const Matrix& transform = Matrix::Identity);
        Matrix set2DCamera(const Vector2& position, float zoom = 1.f);
        Matrix set2DCameraOffCenter(const Vector2& position, float zoom = 1.f);
        Matrix build2DCamera(const Vector2& position, float zoom = 1.f);
        Matrix build2DCameraOffCenter(const Vector2& position, float zoom = 1.f);

        virtual void clear(const Color& color = {.25f, .5f, 1, 1}) = 0;
        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;
        virtual void draw(uint32_t vertexCount) = 0;
        virtual void drawIndexed(uint32_t indexCount) = 0;

        virtual Point getResolution() const = 0;
        virtual void onResize(const Point& newSize) = 0;

        // For effects
        virtual void setKernelSize(const Vector2& kernelSize) = 0;
        virtual void setSepia(const Vector3& tone = Vector3(1.40f, 1.10f, 0.90f), // 0 - 2.55
                      float saturation = .25f, // 0 - 1
                      float sepiaAmount = .75f) = 0; // 0 - 1
        virtual void setCRT(const Vector2& kernelSize) = 0;
        virtual void setCartoon(const Vector3& tone) = 0;
        virtual void setVignette(const Vector2& kernelSize, float amount = .5f) = 0;
        void drawBlurH();
        void drawBlurV();
        void drawSepia();
        void drawCRT();
        void drawCartoon();
        void drawVignette();

        virtual void applyRenderStates() = 0;
        virtual void init(const OWindowRef& pWindow);

        RenderStates renderStates;

    protected:
        Renderer();

        void loadShaders();
        void setupEffectRenderStates();

        OVertexBufferRef m_pEffectsVertexBuffer;

        OShaderRef m_p2DVertexShader;
        OShaderRef m_p2DPixelShader;
        OShaderRef m_pEffectsVertexShader;
        OShaderRef m_pBlurHPixelShader;
        OShaderRef m_pBlurVPixelShader;
        OShaderRef m_pSepiaPixelShader;
        OShaderRef m_pCRTPixelShader;
        OShaderRef m_pCartoonPixelShader;
        OShaderRef m_pVignettePixelShader;
    };
}

extern ORendererRef oRenderer;

#define OScreen             oRenderer->getResolution()
#define OScreenf            Vector2{static_cast<float>(oRenderer->getResolution().x), static_cast<float>(oRenderer->getResolution().y)}
#define OScreenW            oRenderer->getResolution().x
#define OScreenH            oRenderer->getResolution().y
#define OScreenWf           static_cast<float>(oRenderer->getResolution().x)
#define OScreenHf           static_cast<float>(oRenderer->getResolution().y)
#define OScreenCenter       (oRenderer->getResolution() / 2)
#define OScreenCenterf      Vector2{static_cast<float>(oRenderer->getResolution().x / 2), static_cast<float>(oRenderer->getResolution().y / 2)}
#define OScreenCenterX      (oRenderer->getResolution().x / 2)
#define OScreenCenterY      (oRenderer->getResolution().y / 2)
#define OScreenCenterXf     static_cast<float>(oRenderer->getResolution().x / 2)
#define OScreenCenterYf     static_cast<float>(oRenderer->getResolution().y / 2)
