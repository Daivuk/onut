// Onut
#include <onut/IndexBuffer.h>
#include <onut/Log.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Texture.h>
#include <onut/VertexBuffer.h>
#include <onut/Window.h>

// Private
#include "IndexBufferGL.h"
#include "RendererGL.h"
#include "ShaderGL.h"
#include "TextureGL.h"
#include "VertexBufferGL.h"
#if defined(WIN32)
    #include "WindowWIN32.h"
#else
    #include "WindowSDL2.h"
#endif

// STL
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

namespace onut
{
    ORendererRef Renderer::create(const OWindowRef& pWindow)
    {
        return OMake<RendererGL>(pWindow);
    }

    RendererGL::RendererGL(const OWindowRef& pWindow)
    {
    }

    void RendererGL::init(const OWindowRef& pWindow)
    {
        createDevice(pWindow);
        createRenderTarget();
        createRenderStates();
        createUniforms();

        Renderer::init(pWindow);
    }

    RendererGL::~RendererGL()
    {
#if defined(WIN32)
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(m_hRC);
        ReleaseDC(m_hWnd, m_hDC);
#else
        if (m_pSDLWindow)
        {
            SDL_GL_DeleteContext(m_glContext);
        }
#endif
    }

    void RendererGL::createDevice(const OWindowRef& pWindow)
    {
#if defined(WIN32)
        m_hWnd = pWindow->getHandle();

        // Initialize OpenGL
        GLuint PixelFormat;
        static PIXELFORMATDESCRIPTOR pfd =  // pfd Tells Windows How We Want Things To Be
        {
            sizeof(PIXELFORMATDESCRIPTOR),  // Size Of This Pixel Format Descriptor
            1,                              // Version Number
            PFD_DRAW_TO_WINDOW |            // Format Must Support Window
            PFD_SUPPORT_OPENGL |            // Format Must Support OpenGL
            PFD_DOUBLEBUFFER,               // Must Support Double Buffering
            PFD_TYPE_RGBA,                  // Request An RGBA Format
            32,                             // Select Our Color Depth
            0, 0, 0, 0, 0, 0,               // Color Bits Ignored
            0,                              // No Alpha Buffer
            0,                              // Shift Bit Ignored
            0,                              // No Accumulation Buffer
            0, 0, 0, 0,                     // Accumulation Bits Ignored
            16,                             // 16Bit Z-Buffer (Depth Buffer)
            0,                              // No Stencil Buffer
            0,                              // No Auxiliary Buffer
            PFD_MAIN_PLANE,                 // Main Drawing Layer
            0,                              // Reserved
            0, 0, 0                         // Layer Masks Ignored
        };

        m_hDC = GetDC(m_hWnd);
        PixelFormat = ChoosePixelFormat(m_hDC, &pfd);
        SetPixelFormat(m_hDC, PixelFormat, &pfd);
        
        m_hRC = wglCreateContext(m_hDC);
        wglMakeCurrent(m_hDC, m_hRC);

        m_resolution = oSettings->getResolution();
#else
        m_pSDLWindow = ODynamicCast<OWindowSDL2>(pWindow)->getSDLWindow();
        assert(m_pSDLWindow);

        SDL_GetWindowSize(m_pSDLWindow, &m_resolution.x, &m_resolution.y);
            
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        m_glContext = SDL_GL_CreateContext(m_pSDLWindow);

        SDL_GL_SetSwapInterval(1);
#endif

#if !defined(__APPLE__)
        glewInit();
#endif
    }

    void RendererGL::createRenderTarget()
    {
    }

    void RendererGL::onResize(const Point& newSize)
    {
        m_resolution.x = newSize.x;
        m_resolution.y = newSize.y;
    }

    void RendererGL::createRenderStates()
    {
        glCullFace(GL_BACK);
        glDepthFunc(GL_LESS);
        glEnable(GL_TEXTURE_2D);
    }

    void RendererGL::createUniforms()
    {
    }

    void RendererGL::beginFrame()
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

    void RendererGL::endFrame()
    {
#if defined(WIN32)
        SwapBuffers(m_hDC);
#else
        SDL_GL_SwapWindow(m_pSDLWindow);
#endif
    }

    Point RendererGL::getTrueResolution() const
    {
        return m_resolution;
    }

    void RendererGL::clear(const Color& color)
    {
        renderStates.clearColor = color;
        applyRenderStates();
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void RendererGL::clearDepth()
    {
        applyRenderStates();
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void RendererGL::draw(uint32_t vertexCount)
    {
        applyRenderStates();
        
        GLenum mode = GL_POINTS;
        switch (renderStates.primitiveMode.get())
        {
            case PrimitiveMode::PointList:
                mode = GL_POINTS;
                break;
            case PrimitiveMode::LineList:
                mode = GL_LINES;
                break;
            case PrimitiveMode::LineStrip:
                mode = GL_LINE_STRIP;
                break;
            case PrimitiveMode::TriangleList:
                mode = GL_TRIANGLES;
                break;
            case PrimitiveMode::TriangleStrip:
                mode = GL_TRIANGLE_STRIP;
                break;
            default:
                break;
        }

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        //renderStates.indexBuffer.forceDirty();

        glDrawArrays(mode, 0, vertexCount);
    }

    void RendererGL::drawIndexed(uint32_t indexCount)
    {
        applyRenderStates();
        
        GLenum mode = GL_POINTS;
        switch (renderStates.primitiveMode.get())
        {
            case PrimitiveMode::PointList:
                mode = GL_POINTS;
                break;
            case PrimitiveMode::LineList:
                mode = GL_LINES;
                break;
            case PrimitiveMode::LineStrip:
                mode = GL_LINE_STRIP;
                break;
            case PrimitiveMode::TriangleList:
                mode = GL_TRIANGLES;
                break;
            case PrimitiveMode::TriangleStrip:
                mode = GL_TRIANGLE_STRIP;
                break;
            default:
                break;
        }

        glDrawElements(mode, indexCount, GL_UNSIGNED_SHORT, NULL);
    }

    void RendererGL::applyRenderStates()
    {
        // Clear color
        if (renderStates.clearColor.isDirty())
        {
            const auto& clearColor = renderStates.clearColor.get();
            glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
            renderStates.clearColor.resetDirty();
        }
        
        // Render target
        if (renderStates.renderTarget.isDirty())
        {
            auto& pRenderTarget = renderStates.renderTarget.get();
            auto stackCount = renderStates.renderTarget.size();
            if (pRenderTarget)
            {
                auto pRenderTargetGL = ODynamicCast<OTextureGL>(pRenderTarget);
                auto frameBuffer = pRenderTargetGL->getFramebuffer();
                glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
                ++stackCount;
            }
            else
            {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }
            renderStates.renderTarget.resetDirty();
#if !defined(__APPLE__)
            if (stackCount > 1)
            {
                glClipControl(GL_UPPER_LEFT, GL_NEGATIVE_ONE_TO_ONE);
            }
            else
            {
                glClipControl(GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE);
            }
#endif
        }
        
        // Blend
        if (renderStates.blendMode.isDirty())
        {
            switch (renderStates.blendMode.get())
            {
                case onut::BlendMode::Opaque:
                    glDisable(GL_BLEND);
                    break;
                case onut::BlendMode::Alpha:
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    break;
                case onut::BlendMode::Add:
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                    break;
                case onut::BlendMode::PreMultiplied:
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                    break;
                case onut::BlendMode::Multiply:
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
                    break;
                case onut::BlendMode::ForceWrite:
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_ONE, GL_ZERO);
                    break;
                default: 
                    break;
            }
            renderStates.blendMode.resetDirty();
        }
        
        // Viewport
        if (renderStates.viewport.isDirty())
        {
            auto& rect = renderStates.viewport.get();
            glViewport(
                static_cast<GLint>(rect.left), 
                static_cast<GLint>(getResolution().y) - static_cast<GLint>(rect.top) - static_cast<GLint>(rect.bottom - rect.top), 
                static_cast<GLsizei>(rect.right - rect.left),
                static_cast<GLsizei>(rect.bottom - rect.top));
            renderStates.viewport.resetDirty();
        }
        
        // Scissor enabled
        if (renderStates.scissorEnabled.isDirty())
        {
            if (renderStates.scissorEnabled.get())
            {
                glEnable(GL_SCISSOR_TEST);
            }
            else
            {
                glDisable(GL_SCISSOR_TEST);
            }
            renderStates.scissorEnabled.resetDirty();
        }
        
        // Culling
        if (renderStates.backFaceCull.isDirty())
        {
            if (renderStates.backFaceCull.get())
            {
                glEnable(GL_CULL_FACE);
            }
            else
            {
                glDisable(GL_CULL_FACE);
            }
            renderStates.backFaceCull.resetDirty();
        }
        
        // Scissor
        if (renderStates.scissorEnabled.get() &&
            renderStates.scissor.isDirty())
        {
            auto& rect = renderStates.scissor.get();
            glScissor(
                static_cast<GLint>(rect.left), 
                static_cast<GLint>(getResolution().y) - static_cast<GLint>(rect.top) - static_cast<GLint>(rect.bottom - rect.top), 
                static_cast<GLsizei>(rect.right - rect.left),
                static_cast<GLsizei>(rect.bottom - rect.top));
            renderStates.scissor.resetDirty();
        }
        
        // Depth write
        if (renderStates.depthWrite.isDirty())
        {
            if (renderStates.depthWrite.get())
            {
                glDepthMask(GL_TRUE);
            }
            else
            {
                glDepthMask(GL_FALSE);
            }
            renderStates.depthWrite.resetDirty();
        }
        
        // Depth test
        if (renderStates.depthEnabled.isDirty())
        {
            if (renderStates.depthEnabled.get())
            {
                glEnable(GL_DEPTH_TEST);
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
            }
            renderStates.depthEnabled.resetDirty();
        }

        // Shaders
        bool programDirty = false;
        OShaderGL::Program* pProgram = nullptr;
        if (renderStates.vertexShader.get() &&
            renderStates.pixelShader.get())
        {
            auto pShaderGL_vs = ODynamicCast<OShaderGL>(renderStates.vertexShader.get());
            auto pShaderGL_ps = ODynamicCast<OShaderGL>(renderStates.pixelShader.get());
            auto pVSRaw = pShaderGL_vs.get();
            auto pPSRaw = pShaderGL_ps.get();
            pProgram = pPSRaw->getProgram(pShaderGL_vs).get();

            if (renderStates.vertexShader.isDirty() ||
                renderStates.pixelShader.isDirty())
            {
                programDirty = true;
                glUseProgram(pProgram->program);

                // Update all uniforms
                for (int i = 0; i < (int)pVSRaw->m_uniforms.size(); ++i)
                {
                    auto& uniform = pVSRaw->m_uniforms[i];
                    auto uniformId = pProgram->uniformsVS[i];
                    if (uniformId == -1) continue;
                    switch (uniform.type)
                    {
                    case Shader::VarType::Float:
                        glUniform1f(uniformId, uniform.value._11);
                        break;
                    case Shader::VarType::Float2:
                        glUniform2fv(uniformId, 1, &uniform.value._11);
                        break;
                    case Shader::VarType::Float3:
                        glUniform3fv(uniformId, 1, &uniform.value._11);
                        break;
                    case Shader::VarType::Float4:
                        glUniform4fv(uniformId, 1, &uniform.value._11);
                        break;
                    case Shader::VarType::Matrix:
                        glUniformMatrix4fv(uniformId, 1, GL_FALSE, &uniform.value._11);
                        break;
                    }
                    uniform.dirty = false;
                }
                for (int i = 0; i < (int)pPSRaw->m_uniforms.size(); ++i)
                {
                    auto& uniform = pPSRaw->m_uniforms[i];
                    auto uniformId = pProgram->uniformsPS[i];
                    if (uniformId == -1) continue;
                    switch (uniform.type)
                    {
                    case Shader::VarType::Float:
                        glUniform1f(uniformId, uniform.value._11);
                        break;
                    case Shader::VarType::Float2:
                        glUniform2fv(uniformId, 1, &uniform.value._11);
                        break;
                    case Shader::VarType::Float3:
                        glUniform3fv(uniformId, 1, &uniform.value._11);
                        break;
                    case Shader::VarType::Float4:
                        glUniform4fv(uniformId, 1, &uniform.value._11);
                        break;
                    case Shader::VarType::Matrix:
                        glUniformMatrix4fv(uniformId, 1, GL_FALSE, &uniform.value._11);
                        break;
                    }
                    uniform.dirty = false;
                }
            }
            else
            {
                // Only update uniforms that are dirty in that case
                for (int i = 0; i < (int)pVSRaw->m_uniforms.size(); ++i)
                {
                    auto& uniform = pVSRaw->m_uniforms[i];
                    if (!uniform.dirty) continue;
                    uniform.dirty = false;
                    auto uniformId = pProgram->uniformsVS[i];
                    if (uniformId == -1) continue;
                    switch (uniform.type)
                    {
                    case Shader::VarType::Float:
                        glUniform1f(uniformId, uniform.value._11);
                        break;
                    case Shader::VarType::Float2:
                        glUniform2fv(uniformId, 1, &uniform.value._11);
                        break;
                    case Shader::VarType::Float3:
                        glUniform3fv(uniformId, 1, &uniform.value._11);
                        break;
                    case Shader::VarType::Float4:
                        glUniform4fv(uniformId, 1, &uniform.value._11);
                        break;
                    case Shader::VarType::Matrix:
                        glUniformMatrix4fv(uniformId, 1, GL_FALSE, &uniform.value._11);
                        break;
                    }
                }
                for (int i = 0; i < (int)pPSRaw->m_uniforms.size(); ++i)
                {
                    auto& uniform = pPSRaw->m_uniforms[i];
                    if (!uniform.dirty) continue;
                    uniform.dirty = false;
                    auto uniformId = pProgram->uniformsPS[i];
                    if (uniformId == -1) continue;
                    switch (uniform.type)
                    {
                    case Shader::VarType::Float:
                        glUniform1f(uniformId, uniform.value._11);
                        break;
                    case Shader::VarType::Float2:
                        glUniform2fv(uniformId, 1, &uniform.value._11);
                        break;
                    case Shader::VarType::Float3:
                        glUniform3fv(uniformId, 1, &uniform.value._11);
                        break;
                    case Shader::VarType::Float4:
                        glUniform4fv(uniformId, 1, &uniform.value._11);
                        break;
                    case Shader::VarType::Matrix:
                        glUniformMatrix4fv(uniformId, 1, GL_FALSE, &uniform.value._11);
                        break;
                    }
                }
            }

            // Transform matrix
            if (renderStates.projection.isDirty() ||
                renderStates.view.isDirty() ||
                renderStates.world.isDirty() ||
                programDirty)
            {
                auto world = renderStates.world.get();
                auto finalTransform = world * renderStates.view.get() * renderStates.projection.get();
                finalTransform = finalTransform.Transpose();

                renderStates.projection.resetDirty();
                renderStates.view.resetDirty();
                renderStates.world.resetDirty();

                if (pProgram && pProgram->oViewProjectionUniform != -1)
                {
                    glUniformMatrix4fv(pProgram->oViewProjectionUniform, 1, GL_FALSE, &finalTransform._11);
                }
            }
        }

        // Textures
        auto pShaderGL_ps_s = ODynamicCast<OShaderGL>(renderStates.pixelShader.get());
        ShaderGL* pPSRaw_s = pShaderGL_ps_s.get();
        bool isSampleDirty =
            renderStates.sampleFiltering.isDirty() ||
            renderStates.sampleAddressMode.isDirty();
        for (int i = 0; i < RenderStates::MAX_TEXTURES; ++i)
        {
            auto& pTextureState = renderStates.textures[i];
            if (pTextureState.isDirty() || (isSampleDirty && i == 0) || renderStates.pixelShader.isDirty())
            {
                auto pTexture = pTextureState.get().get();
                if (pTexture != nullptr)
                {
                    auto pTextureEGLS2 = static_cast<TextureGL*>(pTexture);

                    if (pPSRaw_s && i >= (int)pPSRaw_s->m_textures.size())
                    {
                        continue;
                    }

                    if (pProgram)
                    {
                        glUniform1i(pProgram->textures[i], i);
                    }

                    glActiveTexture(GL_TEXTURE0 + i);
                    glBindTexture(GL_TEXTURE_2D, pTextureEGLS2->getHandle());

                    if (i > 0 && pPSRaw_s && i < (int)pPSRaw_s->m_textures.size())
                    {
                        const auto& textureSamplers = pPSRaw_s->m_textures[i];
                        if (textureSamplers.filter != pTextureEGLS2->filtering)
                        {
                            pTextureEGLS2->filtering = textureSamplers.filter;
                            if (pTextureEGLS2->filtering == sample::Filtering::Nearest)
                            {
                              //  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                            }
                            else if (pTextureEGLS2->filtering == sample::Filtering::Linear)
                            {
                            //    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            }
                            else if (pTextureEGLS2->filtering == sample::Filtering::Bilinear)
                            {
                         //       glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            }
                            else if (pTextureEGLS2->filtering == sample::Filtering::Trilinear)
                            {
                       //         glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            }
                       /*     else if (pTextureEGLS2->filtering == sample::Filtering::Anisotropic)
                            {
                                float aniso = 0.0f;
                                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
                                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            }*/
                        }
                        if (textureSamplers.repeatX != pTextureEGLS2->addressModeX)
                        {
                            pTextureEGLS2->addressModeX = textureSamplers.repeatX;
                            if (pTextureEGLS2->addressModeX == sample::AddressMode::Wrap)
                            {
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                            }
                            else if (pTextureEGLS2->addressModeX == sample::AddressMode::Clamp)
                            {
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                            }
                        }
                        if (textureSamplers.repeatY != pTextureEGLS2->addressModeY)
                        {
                            pTextureEGLS2->addressModeY = textureSamplers.repeatY;
                            if (pTextureEGLS2->addressModeY == sample::AddressMode::Wrap)
                            {
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                            }
                            else if (pTextureEGLS2->addressModeY == sample::AddressMode::Clamp)
                            {
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                            }
                        }
                    }
                    else
                    {
                        if (renderStates.sampleFiltering.get() != pTextureEGLS2->filtering)
                        {
                            pTextureEGLS2->filtering = renderStates.sampleFiltering.get();
                            if (pTextureEGLS2->filtering == sample::Filtering::Nearest)
                            {
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                            }
                            else if (pTextureEGLS2->filtering == sample::Filtering::Linear)
                            {
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            }
                        }
                        auto mode = renderStates.sampleAddressMode.get();
                        if (mode != pTextureEGLS2->addressModeX ||
                            mode != pTextureEGLS2->addressModeY)
                        {
                            pTextureEGLS2->addressModeX = mode;
                            pTextureEGLS2->addressModeY = mode;
                            if (mode == sample::AddressMode::Wrap)
                            {
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                            }
                            else if (mode == sample::AddressMode::Clamp)
                            {
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                            }
                        }
                    }
                }
                pTextureState.resetDirty();
            }
        }
        if (isSampleDirty)
        {
            renderStates.sampleFiltering.resetDirty();
            renderStates.sampleAddressMode.resetDirty();
        }

        renderStates.vertexShader.resetDirty();
        renderStates.pixelShader.resetDirty();

        // Vertex/Index buffers
        if (renderStates.vertexBuffer.isDirty() || programDirty)
        {
            if (renderStates.vertexBuffer.get())
            {
                auto pVBRaw = static_cast<OVertexBufferGL*>(renderStates.vertexBuffer.get().get());
                auto handle = pVBRaw->getHandle();
                auto pShaderGL_vs = ODynamicCast<OShaderGL>(renderStates.vertexShader.get());
                auto pVSRaw = pShaderGL_vs.get();

                glBindBuffer(GL_ARRAY_BUFFER, handle);

                auto attribCount = (int)pVSRaw->m_inputLayout.size();
                for (int i = attribCount; i < m_lastVertexAttribCount; ++i)
                {
                    glDisableVertexAttribArray(i);
                }
                for (int i = m_lastVertexAttribCount; i < attribCount; ++i)
                {
                    glEnableVertexAttribArray(i);
                }

                int offset = 0;
                int dataSize = 0;
                for (int i = 0; i < attribCount; ++i)
                {
                    switch (pVSRaw->m_inputLayout[i].type)
                    {
                    case Shader::VarType::Float:
                        dataSize += 4;
                        break;
                    case Shader::VarType::Float2:
                        dataSize += 4 * 2;
                        break;
                    case Shader::VarType::Float3:
                        dataSize += 4 * 3;
                        break;
                    case Shader::VarType::Float4:
                        dataSize += 4 * 4;
                        break;
                    }
                }
                for (int i = 0; i < attribCount; ++i)
                {
                    int size = 0;
                    switch (pVSRaw->m_inputLayout[i].type)
                    {
                    case Shader::VarType::Float:
                        size = 1;
                        break;
                    case Shader::VarType::Float2:
                        size = 2;
                        break;
                    case Shader::VarType::Float3:
                        size = 3;
                        break;
                    case Shader::VarType::Float4:
                        size = 4;
                        break;
                    }
                    glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, dataSize, (float*)(uintptr_t)offset);
                    offset += size * 4;
                }
                m_lastVertexAttribCount = attribCount;
            }
            renderStates.vertexBuffer.resetDirty();
        }
        if (renderStates.indexBuffer.isDirty() || programDirty)
        {
            if (renderStates.indexBuffer.get())
            {
                auto handle = static_cast<OIndexBufferGL*>(renderStates.indexBuffer.get().get())->getHandle();
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
            }
            renderStates.indexBuffer.resetDirty();
        }
    }
}
