#if defined(__unix__)
// Onut
//#include <onut/IndexBuffer.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
//#include <onut/Texture.h>
//#include <onut/VertexBuffer.h>
#include <onut/Window.h>

// Private
//#include "IndexBufferD3D11.h"
#include "RendererGLES2.h"
//#include "ShaderD3D11.h"
//#include "TextureD3D11.h"
//#include "VertexBufferD3D11.h"

// Third party
#include <bcm_host.h>

// STL
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

namespace onut
{
    ORendererRef Renderer::create(const OWindowRef& pWindow)
    {
        return OMake<RendererGLES2>(pWindow);
    }

    RendererGLES2::RendererGLES2(const OWindowRef& pWindow)
    {
    }

    void RendererGLES2::init(const OWindowRef& pWindow)
    {
        createDevice(pWindow);
        createRenderTarget();
        createRenderStates();
        createUniforms();

        Renderer::init(pWindow);
    }

    RendererGLES2::~RendererGLES2()
    { 
        DISPMANX_UPDATE_HANDLE_T dispman_update;
        int s;

        dispman_update = vc_dispmanx_update_start(0);
        s = vc_dispmanx_element_remove(dispman_update, m_dispman_element);
        assert(s == 0);
        vc_dispmanx_update_submit_sync(dispman_update);
        s = vc_dispmanx_display_close(m_dispman_display);
        assert(s == 0);

        // Release OpenGL resources
        eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroyContext(m_display, m_context);
        eglTerminate(m_display);
    }

    void RendererGLES2::createDevice(const OWindowRef& pWindow)
    {
		bcm_host_init();
        
        int32_t success = 0;
        EGLBoolean result;
        EGLint num_config;

        DISPMANX_UPDATE_HANDLE_T dispman_update;
        VC_RECT_T dst_rect;
        VC_RECT_T src_rect;

        static const EGLint attribute_list[] =
        {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_NONE
        };

        EGLConfig config;

        // get an EGL display connection
        m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        assert(m_display!=EGL_NO_DISPLAY);

        // initialize the EGL display connection
        result = eglInitialize(m_display, NULL, NULL);
        assert(EGL_FALSE != result);

        // get an appropriate EGL frame buffer configuration
        result = eglChooseConfig(m_display, attribute_list, &config, 1, &num_config);
        assert(EGL_FALSE != result);

        // create an EGL rendering context
        m_context = eglCreateContext(m_display, config, EGL_NO_CONTEXT, NULL);
        assert(m_context!=EGL_NO_CONTEXT);

        // create an EGL window surface
        uint32_t screen_width, screen_height;
        success = graphics_get_display_size(0 /* LCD */, &screen_width, &screen_height);
        assert(success >= 0);
        if (oSettings->getBorderlessFullscreen())
        {
            m_resolution.x = (int)screen_width;
            m_resolution.y = (int)screen_height;
        }
        else
        {
            m_resolution = oSettings->getResolution();
        }

        dst_rect.x = (screen_width - (uint32_t)m_resolution.x) / 2;
        dst_rect.y = (screen_height - (uint32_t)m_resolution.y) / 2;
        dst_rect.width = (uint32_t)m_resolution.x;
        dst_rect.height = (uint32_t)m_resolution.y;
          
        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = (uint32_t)m_resolution.x << 16;
        src_rect.height = (uint32_t)m_resolution.y << 16;     

        m_dispman_display = vc_dispmanx_display_open(0 /* LCD */);
        dispman_update = vc_dispmanx_update_start(0);
             
        m_dispman_element = vc_dispmanx_element_add(dispman_update, m_dispman_display,
          0/*layer*/, &dst_rect, 0/*src*/,
          &src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha*/, 0/*clamp*/, (DISPMANX_TRANSFORM_T)0/*transform*/);
          
        m_nativeWindow.element = m_dispman_element;
        m_nativeWindow.width = (uint32_t)m_resolution.x;
        m_nativeWindow.height = (uint32_t)m_resolution.y;
        vc_dispmanx_update_submit_sync(dispman_update);
          
        m_surface = eglCreateWindowSurface(m_display, config, &m_nativeWindow, NULL);
        assert(m_surface != EGL_NO_SURFACE);

        // connect the context to the surface
        result = eglMakeCurrent(m_display, m_surface, m_surface, m_context);
        assert(EGL_FALSE != result);
    }

    void RendererGLES2::createRenderTarget()
    {
    }

    void RendererGLES2::onResize(const Point& newSize)
    {
    }

    void RendererGLES2::createRenderStates()
    {
    }

    void RendererGLES2::createUniforms()
    {
    }

    void RendererGLES2::beginFrame()
    {
        // Bind render target
        renderStates.reset();

        // Set viewport/scissor
        const auto& res = getResolution();
        renderStates.viewport = iRect{0, 0, res.x, res.y};
        renderStates.scissorEnabled = false;
        renderStates.scissor = renderStates.viewport.get();

        // Reset 2d view
        set2DCamera(Vector2::Zero);
    }

    void RendererGLES2::endFrame()
    {
        // Swap the buffer!
        eglSwapBuffers(m_display, m_surface);
    }

    EGLDisplay RendererGLES2::getDisplay() const
    {
        return m_display;
    }
    
    EGLSurface RendererGLES2::getSurface() const
    {
        return m_surface;
    }
    
    EGLContext RendererGLES2::getContext() const
    {
        return m_context;
    }

    Point RendererGLES2::getTrueResolution() const
    {
        return m_resolution;
    }

    void RendererGLES2::clear(const Color& color)
    {
        applyRenderStates();
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void RendererGLES2::clearDepth()
    {
        applyRenderStates();
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void RendererGLES2::setKernelSize(const Vector2& kernelSize)
    {
    }

    void RendererGLES2::setCRT(const Vector2& resolution)
    {
    }

    void RendererGLES2::setCartoon(const Vector3& tone)
    {
    }

    void RendererGLES2::setVignette(const Vector2& kernelSize, float amount)
    {
    }

    void RendererGLES2::setSepia(const Vector3& tone, float saturation, float sepiaAmount)
    {
    }

    void RendererGLES2::draw(uint32_t vertexCount)
    {
        applyRenderStates();
    }

    void RendererGLES2::drawIndexed(uint32_t indexCount)
    {
        applyRenderStates();
    }

    void RendererGLES2::applyRenderStates()
    {
        // Render target
    /*    if (renderStates.renderTarget.isDirty())
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
        if (renderStates.scissorEnabled.isDirty() || renderStates.backFaceCull.isDirty())
        {
            if (renderStates.backFaceCull.get())
            {
                if (renderStates.scissorEnabled.get())
                {
                    m_pDeviceContext->RSSetState(m_pRasterizerStates[3]);
                }
                else
                {
                    m_pDeviceContext->RSSetState(m_pRasterizerStates[2]);
                }
            }
            else
            {
                if (renderStates.scissorEnabled.get())
                {
                    m_pDeviceContext->RSSetState(m_pRasterizerStates[1]);
                }
                else
                {
                    m_pDeviceContext->RSSetState(m_pRasterizerStates[0]);
                }
            }
            renderStates.scissorEnabled.resetDirty();
            renderStates.backFaceCull.resetDirty();
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
            auto world = renderStates.world.get();
            auto finalTransform = world * renderStates.viewProjection.get();
            finalTransform = finalTransform.Transpose();

            D3D11_MAPPED_SUBRESOURCE map;
            m_pDeviceContext->Map(m_pViewProj2dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
            memcpy(map.pData, &finalTransform._11, sizeof(finalTransform));
            m_pDeviceContext->Unmap(m_pViewProj2dBuffer, 0);
            m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pViewProj2dBuffer);

            renderStates.viewProjection.resetDirty();
            renderStates.world.resetDirty();
        }

        // Depth
        if (renderStates.depthEnabled.isDirty() ||
            renderStates.depthWrite.isDirty())
        {
            if (renderStates.depthEnabled.get() || renderStates.depthWrite.get())
            {
                auto& pRenderTarget = renderStates.renderTarget.get();
                ID3D11RenderTargetView* pRenderTargetView = m_pRenderTargetView;
                if (pRenderTarget)
                {
                    auto pRenderTargetD3D11 = ODynamicCast<OTextureD3D11>(pRenderTarget);
                    pRenderTargetView = pRenderTargetD3D11->getD3DRenderTargetView();
                }
                m_pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, m_pDepthStencilView);
            }
            if (!renderStates.depthEnabled.get() && !renderStates.depthWrite.get())
                m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilStates[0], 1);
            if (renderStates.depthEnabled.get() && renderStates.depthWrite.get())
                m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilStates[1], 1);
            if (renderStates.depthEnabled.get() && !renderStates.depthWrite.get())
                m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilStates[2], 1);
            if (!renderStates.depthEnabled.get() && renderStates.depthWrite.get())
                m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilStates[3], 1);
            renderStates.depthEnabled.resetDirty();
            renderStates.depthWrite.resetDirty();
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
        auto pShaderD3D11 = std::dynamic_pointer_cast<OShaderD3D11>(renderStates.vertexShader.get());
        if (pShaderD3D11)
        {
            if (renderStates.vertexShader.isDirty())
            {
                m_pDeviceContext->VSSetShader(pShaderD3D11->getVertexShader(), nullptr, 0);
                m_pDeviceContext->IASetInputLayout(pShaderD3D11->getInputLayout());
                renderStates.vertexShader.resetDirty();

                auto& uniforms = pShaderD3D11->getUniforms();
                for (UINT i = 0; i < (UINT)uniforms.size(); ++i)
                {
                    m_pDeviceContext->VSSetConstantBuffers(4 + i, 1, &(uniforms[i].pBuffer));
                    uniforms[i].dirty = false;
                }
            }
            else
            {
                auto& uniforms = pShaderD3D11->getUniforms();
                for (UINT i = 0; i < (UINT)uniforms.size(); ++i)
                {
                    auto& uniform = uniforms[i];
                    if (uniform.dirty)
                    {
                        m_pDeviceContext->VSSetConstantBuffers(4 + i, 1, &(uniform.pBuffer));
                        uniform.dirty = false;
                    }
                }
            }
        }

        pShaderD3D11 = std::dynamic_pointer_cast<OShaderD3D11>(renderStates.pixelShader.get());
        if (pShaderD3D11)
        {
            if (renderStates.pixelShader.isDirty())
            {
                m_pDeviceContext->PSSetShader(pShaderD3D11->getPixelShader(), nullptr, 0);
                renderStates.pixelShader.resetDirty();

                auto& uniforms = pShaderD3D11->getUniforms();
                for (UINT i = 0; i < (UINT)uniforms.size(); ++i)
                {
                    m_pDeviceContext->PSSetConstantBuffers(4 + i, 1, &(uniforms[i].pBuffer));
                    uniforms[i].dirty = false;
                }
            }
            else
            {
                auto& uniforms = pShaderD3D11->getUniforms();
                for (UINT i = 0; i < (UINT)uniforms.size(); ++i)
                {
                    auto& uniform = uniforms[i];
                    if (uniform.dirty)
                    {
                        m_pDeviceContext->PSSetConstantBuffers(4 + i, 1, &(uniform.pBuffer));
                        uniform.dirty = false;
                    }
                }
            }
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
        }*/
    }
}

#endif
