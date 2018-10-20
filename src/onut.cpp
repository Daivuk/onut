// Onut includes
#include <onut/ActionManager.h>
#include <onut/AudioEngine.h>
//#include <onut/Cloud.h>
#include <onut/ComponentFactory.h>
#include <onut/ContentManager.h>
#include <onut/Dispatcher.h>
#include <onut/SceneManager.h>
#include <onut/Font.h>
#include <onut/GamePad.h>
#include <onut/IndexBuffer.h>
#include <onut/Input.h>
#include <onut/Http.h>
#include <onut/Log.h>
#include <onut/Multiplayer.h>
#include <onut/onut.h>
#include <onut/ParticleSystemManager.h>
#include <onut/PrimitiveBatch.h>
#include <onut/Random.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
#include <onut/ThreadPool.h>
#include <onut/Timing.h>
#include <onut/UIContext.h>
#include <onut/UIControl.h>
#include <onut/UIPanel.h>
#include <onut/UITextBox.h>
#include <onut/Updater.h>
#include <onut/VertexBuffer.h>
#include <onut/Window.h>

// Private
#include "JSBindings.h"

// Third parties
#include <imgui/imgui.h>

// STL
#include <cassert>
#include <mutex>
#include <sstream>

OTextureRef g_pMainRenderTarget;
static OTextureRef g_pImguiFontTexture;
static OIndexBufferRef g_pImguiIB;
static OVertexBufferRef g_pImguiVB;

struct ImguiVertex
{
    Vector2 pos;
    Vector2 tex;
    Color color;
};

std::atomic<bool> g_bIsRunning;
            
namespace onut
{
    void createUI()
    {
        oUIContext = UIContext::create(Vector2(OScreenWf, OScreenHf));
        oUI = UIControl::create();
        oUI->widthType = UIControl::DimType::Relative;
        oUI->heightType = UIControl::DimType::Relative;

        oUIContext->addTextCaretSolver<onut::UITextBox>("", [=](const OUITextBoxRef& pTextBox, const Vector2& localPos) -> decltype(std::string().size())
        {
            auto pFont = OGetFont(pTextBox->textComponent.font.typeFace.c_str());
            if (!pFont) return 0;
            auto& text = pTextBox->textComponent.text;
            return pFont->caretPos(text, localPos.x - 4);
        });

        oWindow->onWrite = [](char c)
        {
            oUIContext->write(c);
            ImGui::GetIO().AddInputCharacter((unsigned short)c);
        };
        oWindow->onKey = [](uintptr_t key)
        {
            oUIContext->keyDown(key);
        };

        oUIContext->addStyle<onut::UIPanel>("blur", [](const OUIPanelRef& pPanel, const Rect& rect)
        {
            oSpriteBatch->end();
            if (oRenderer->renderStates.renderTarget.get())
            {
                oRenderer->renderStates.renderTarget.get()->blur();
            }
            oSpriteBatch->begin();
            oSpriteBatch->drawRect(nullptr, (rect), pPanel->color);
        });
    }

    void createServices()
    {
        // Random
        randomizeSeed();

        // Thread pool
        if (!oThreadPool) oThreadPool = OThreadPool::create();

        // Dispatcher
        if (!oDispatcher) oDispatcher = ODispatcher::create();

        // Timing class
        if (!oTiming) oTiming = OTiming::create();

        // Updater
        if (!oUpdater) oUpdater = OUpdater::create();

        // Window
        if (!oWindow) oWindow = OWindow::create();

        // Renderer
        if (!oRenderer)
        {
            oRenderer = ORenderer::create(oWindow);
            oRenderer->init(oWindow);
        }

        // SpriteBatch
        if (!oSpriteBatch) oSpriteBatch = SpriteBatch::create();
        if (!oPrimitiveBatch) oPrimitiveBatch = PrimitiveBatch::create();
        
        // Content
        if (!oContentManager) oContentManager = ContentManager::create();

        // Cloud
        //oCloud = Cloud::create(oSettings->getAppId(), oSettings->getAppSecret());

        // Mouse/Keyboard
        if (!oInput) oInput = OInput::create(oWindow);

        // Audio
        if (!oAudioEngine) oAudioEngine = AudioEngine::create();

        // Particles
        if (!oParticleSystemManager) oParticleSystemManager = ParticleSystemManager::create();

        // Http
        if (!oHttp) oHttp = Http::create();

        // Multiplayer
        if (!oMultiplayer) oMultiplayer = Multiplayer::create();

        // UI Context
        createUI();

        // Component factory
        if (!oComponentFactory)
        {
            oComponentFactory = ComponentFactory::create();
            oComponentFactory->registerDefaultComponents();
        }

        // Entity Manager
        if (!oSceneManager) oSceneManager = SceneManager::create();

        // Undo/Redo for editors
        if (!oActionManager) oActionManager = ActionManager::create();

        g_pMainRenderTarget = OTexture::createScreenRenderTarget();

        // Initialize Javascript
        onut::js::init();

        // imgui
        auto& io = ImGui::GetIO();
        uint8_t *pPixelData;
        int w, h;
        io.Fonts->GetTexDataAsRGBA32(&pPixelData, &w, &h);
        g_pImguiFontTexture = OTexture::createFromData(pPixelData, { w, h }, false);
        io.Fonts->SetTexID(&g_pImguiFontTexture);
        g_pImguiIB = OIndexBuffer::createDynamic(sizeof(uint16_t) * 300 * 6);
        g_pImguiVB = OVertexBuffer::createDynamic(sizeof(ImguiVertex) * 300 * 4);
        io.KeyMap[ImGuiKey_Tab] = (int)OKeyTab; // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array that we will update during the application lifetime.
        io.KeyMap[ImGuiKey_LeftArrow] = (int)OKeyLeft;
        io.KeyMap[ImGuiKey_RightArrow] = (int)OKeyRight;
        io.KeyMap[ImGuiKey_UpArrow] = (int)OKeyUp;
        io.KeyMap[ImGuiKey_DownArrow] = (int)OKeyDown;
        io.KeyMap[ImGuiKey_PageUp] = (int)OKeyPageUp;
        io.KeyMap[ImGuiKey_PageDown] = (int)OKeyPageDown;
        io.KeyMap[ImGuiKey_Home] = (int)OKeyHome;
        io.KeyMap[ImGuiKey_End] = (int)OKeyEnd;
        io.KeyMap[ImGuiKey_Delete] = (int)OKeyDelete;
        io.KeyMap[ImGuiKey_Backspace] = (int)OKeyBackspace;
        io.KeyMap[ImGuiKey_Enter] = (int)OKeyEnter;
        io.KeyMap[ImGuiKey_Escape] = (int)OKeyEscape;
        io.KeyMap[ImGuiKey_A] = (int)OKeyA;
        io.KeyMap[ImGuiKey_C] = (int)OKeyC;
        io.KeyMap[ImGuiKey_V] = (int)OKeyV;
        io.KeyMap[ImGuiKey_X] = (int)OKeyX;
        io.KeyMap[ImGuiKey_Y] = (int)OKeyY;
        io.KeyMap[ImGuiKey_Z] = (int)OKeyZ;
    }

    void cleanup()
    {
        ImGui::Shutdown();
        onut::js::shutdown();

        g_pImguiVB = nullptr;
        g_pImguiIB = nullptr;
        g_pImguiFontTexture = nullptr;
        g_pMainRenderTarget = nullptr;
        oActionManager = nullptr;
        oSceneManager = nullptr;
        oComponentFactory = nullptr;
        oDispatcher = nullptr;
        oUpdater = nullptr;
        oUI = nullptr;
        oUIContext = nullptr;
        oMultiplayer = nullptr;
        oHttp = nullptr;
        oParticleSystemManager = nullptr;
        oAudioEngine = nullptr;
        oInput = nullptr;
        //oCloud = nullptr;
        oContentManager = nullptr;
        oPrimitiveBatch = nullptr;
        oSpriteBatch = nullptr;
        oRenderer = nullptr;
        oWindow = nullptr;
        oSettings = nullptr;
        oThreadPool = nullptr;
        oTiming = nullptr;
    }

    void drawImgui()
    {
        oRenderer->setupFor2D();
        oRenderer->renderStates.blendMode.push(OBlendAlpha);
        oRenderer->renderStates.scissorEnabled.push(true);
        oRenderer->renderStates.primitiveMode = OPrimitiveTriangleList;

        auto pDrawData = ImGui::GetDrawData();
        auto cmdListCount = pDrawData->CmdListsCount;
        for (int i = 0; i < cmdListCount; ++i)
        {
            auto pCmdList = pDrawData->CmdLists[i];

            auto vertCount = pCmdList->VtxBuffer.size();
            auto indexCount = pCmdList->IdxBuffer.size();
            int drawOffset = 0;

            // Update ib
            if (g_pImguiIB->size() < indexCount * sizeof(uint16_t))
            {
                g_pImguiIB = OIndexBuffer::createDynamic(indexCount * sizeof(uint16_t));
            }
            auto pIndexData = g_pImguiIB->map();
            memcpy(pIndexData, pCmdList->IdxBuffer.Data, indexCount * sizeof(uint16_t));
            g_pImguiIB->unmap(indexCount * sizeof(uint16_t));

            // Update vb
            if (g_pImguiVB->size() < vertCount * sizeof(ImguiVertex))
            {
                g_pImguiVB = OVertexBuffer::createDynamic(vertCount * sizeof(ImguiVertex));
            }
            auto pVertexData = (ImguiVertex*)g_pImguiVB->map();
            auto pImguiVertexData = pCmdList->VtxBuffer.Data;
            for (int v = 0; v < vertCount; ++v)
            {
                pVertexData->pos.x = pImguiVertexData->pos.x;
                pVertexData->pos.y = pImguiVertexData->pos.y;
                pVertexData->tex.x = pImguiVertexData->uv.x;
                pVertexData->tex.y = pImguiVertexData->uv.y;
                pVertexData->color.r = (float)((pImguiVertexData->col) & 0xff) / 255.0f;
                pVertexData->color.g = (float)((pImguiVertexData->col >> 8) & 0xff) / 255.0f;
                pVertexData->color.b = (float)((pImguiVertexData->col >> 16) & 0xff) / 255.0f;
                pVertexData->color.a = (float)((pImguiVertexData->col >> 24) & 0xff) / 255.0f;

                ++pVertexData;
                ++pImguiVertexData;
            }
            g_pImguiVB->unmap(vertCount * sizeof(ImguiVertex));

            oRenderer->renderStates.indexBuffer = g_pImguiIB;
            oRenderer->renderStates.vertexBuffer = g_pImguiVB;

            // Loop sub meshes
            auto cmdBufferCount = pCmdList->CmdBuffer.size();
            for (int j = 0; j < cmdBufferCount; ++j)
            {
                auto pCmd = pCmdList->CmdBuffer.Data + j;

                if (pCmd->UserCallback)
                {
                    pCmd->UserCallback(pCmdList, pCmd);
                }
                else
                {
                    oRenderer->renderStates.scissor.push({ (int)pCmd->ClipRect.x, (int)pCmd->ClipRect.y, (int)pCmd->ClipRect.z, (int)pCmd->ClipRect.w });
                    oRenderer->renderStates.textures[0] = *(OTextureRef *)pCmd->TextureId;
                    oRenderer->drawIndexed(pCmd->ElemCount, drawOffset);
                    oRenderer->renderStates.scissor.pop();
                }

                drawOffset += pCmd->ElemCount;
            }
        }

        oRenderer->renderStates.scissorEnabled.pop();
        oRenderer->renderStates.blendMode.pop();
    }

    // Start the engine
    void run(std::function<void()> initCallback,
             std::function<void()> updateCallback, 
             std::function<void()> renderCallback,
             std::function<void()> renderUICallback,
             std::function<void()> postRenderCallback)
    {
        // Make sure we run just once
        static bool alreadyRan = false;
        assert(!alreadyRan);
        alreadyRan = true;

        createServices();

        // Call the user defined init
        if (initCallback)
        {
            initCallback();
        }

        // Main loop
        g_bIsRunning = true;
        while (true)
        {
            if (!oWindow->pollEvents()) break;
            if (!g_bIsRunning) break;

            // Sync to main callbacks
            oDispatcher->processQueue();

            // Update
            oAudioEngine->update();
            auto framesToUpdate = oTiming->update(oSettings->getIsFixedStep());
            auto& io = ImGui::GetIO();
            auto imgui_updated = false;
            while (framesToUpdate--)
            {
                oInput->update();
#if defined(__rpi__)
                if (OInputPressed(OKeyLeftAlt) && OInputPressed(OKeyF4))
                {
                    g_bIsRunning = false;
                    break;
                }
#endif
#if defined(WIN32)
                POINT cur;
                GetCursorPos(&cur);
                ScreenToClient(oWindow->getHandle(), &cur);
                oInput->mousePos.x = cur.x;
                oInput->mousePos.y = cur.y;
                oInput->mousePosf.x = static_cast<float>(cur.x);
                oInput->mousePosf.y = static_cast<float>(cur.y);
#endif // WIN32
                // Imgui input updates
                if (!imgui_updated)
                {
                    imgui_updated = true;
                    io.MouseDown[0] = OInputPressed(OMouse1);
                    io.MouseDown[1] = OInputPressed(OMouse2);
                    io.MouseDown[2] = OInputPressed(OMouse3);
                    io.MouseDown[3] = OInputPressed(OMouse4);
                    io.MouseDown[4] = false;
                    io.MouseWheel += oInput->getStateValue(OMouseZ) > 0 ? 1.f : oInput->getStateValue(OMouseZ) < 0 ? -1.f : 0.0f;
                    io.MouseDrawCursor = false;
                    io.KeyCtrl = OInputPressed(OKeyLeftControl);
                    io.KeyShift = OInputPressed(OKeyLeftShift);
                    io.KeyAlt = OInputPressed(OKeyLeftAlt);
                    io.KeySuper = OInputPressed(OKeyLeftWindows);
                    io.KeysDown[(int)OKeyTab] = OInputPressed(OKeyTab);
                    io.KeysDown[(int)OKeyLeft] = OInputPressed(OKeyLeft);
                    io.KeysDown[(int)OKeyRight] = OInputPressed(OKeyRight);
                    io.KeysDown[(int)OKeyUp] = OInputPressed(OKeyUp);
                    io.KeysDown[(int)OKeyDown] = OInputPressed(OKeyDown);
                    io.KeysDown[(int)OKeyPageUp] = OInputPressed(OKeyPageUp);
                    io.KeysDown[(int)OKeyPageDown] = OInputPressed(OKeyPageDown);
                    io.KeysDown[(int)OKeyHome] = OInputPressed(OKeyHome);
                    io.KeysDown[(int)OKeyEnd] = OInputPressed(OKeyEnd);
                    io.KeysDown[(int)OKeyDelete] = OInputPressed(OKeyDelete);
                    io.KeysDown[(int)OKeyBackspace] = OInputPressed(OKeyBackspace);
                    io.KeysDown[(int)OKeyEnter] = OInputPressed(OKeyEnter);
                    io.KeysDown[(int)OKeyEscape] = OInputPressed(OKeyEscape);
                    io.KeysDown[(int)OKeyA] = OInputPressed(OKeyA);
                    io.KeysDown[(int)OKeyC] = OInputPressed(OKeyC);
                    io.KeysDown[(int)OKeyV] = OInputPressed(OKeyV);
                    io.KeysDown[(int)OKeyX] = OInputPressed(OKeyX);
                    io.KeysDown[(int)OKeyY] = OInputPressed(OKeyY);
                    io.KeysDown[(int)OKeyZ] = OInputPressed(OKeyZ);
                }
                io.MousePos = { oInput->mousePosf.x, oInput->mousePosf.y };

                oUpdater->update();
                auto mousePosf = OGetMousePos();
                if (oUIContext->useNavigation)
                {
                    oUI->update(oUIContext, Vector2(mousePosf.x, mousePosf.y), 
                                OGamePadPressed(OGamePadA) || OInputJustPressed(OKeyEnter) || OInputJustPressed(OXArcadeLButton1), 
                                false, false,
                                OGamePadJustPressed(OGamePadDPadLeft) || OGamePadJustPressed(OGamePadLeftThumbLeft) || OInputJustPressed(OKeyLeft) || OInputJustPressed(OXArcadeLJoyLeft),
                                OGamePadJustPressed(OGamePadDPadRight) || OGamePadJustPressed(OGamePadLeftThumbRight) || OInputJustPressed(OKeyRight) || OInputJustPressed(OXArcadeLJoyRight),
                                OGamePadJustPressed(OGamePadDPadUp) || OGamePadJustPressed(OGamePadLeftThumbUp) || OInputJustPressed(OKeyUp) || OInputJustPressed(OXArcadeLJoyUp),
                                OGamePadJustPressed(OGamePadDPadDown) || OGamePadJustPressed(OGamePadLeftThumbDown) || OInputJustPressed(OKeyDown) || OInputJustPressed(OXArcadeLJoyDown),
                                0.f);
                }
                else
                {
                    oUI->update(oUIContext, Vector2(mousePosf.x, mousePosf.y),
                                OInputPressed(OMouse1), OInputPressed(OMouse2), OInputPressed(OMouse3),
                                false, false, false, false, 
                                OInputPressed(OKeyLeftControl), oInput->getStateValue(OMouseZ));
                }
                oParticleSystemManager->update();
                oSceneManager->update();
                onut::js::update(oTiming->getDeltaTime());
                if (updateCallback)
                {
                    updateCallback();
                }
            }

            oTiming->render();

            // Render
            if (oSettings->getShowOnScreenLog() && oSettings->getIsRetroMode())
            {
                oRenderer->clear(Color::Black);
            }
            oRenderer->renderStates.renderTarget = g_pMainRenderTarget;
            oRenderer->beginFrame();
            onut::js::render();
            if (renderCallback)
            {
                renderCallback();
            }
            oSceneManager->render();
            oParticleSystemManager->render();
            oSpriteBatch->begin();
            oUI->render(oUIContext);
            oSpriteBatch->end();

            // Imgui
            io.DeltaTime = oTiming->getRenderDeltaTime();
            io.DisplaySize = { OScreenWf, OScreenHf };
            ImGui::NewFrame();
            onut::js::renderUI();
            if (renderUICallback)
            {
                renderUICallback();
            }
            ImGui::Render();
            drawImgui();

            // Draw final render target
            oRenderer->renderStates.renderTarget = nullptr;
            const auto& res = oRenderer->getResolution();
            oRenderer->renderStates.viewport = iRect{0, 0, res.x, res.y};
            oRenderer->renderStates.scissorEnabled = false;
            oRenderer->renderStates.scissor = oRenderer->renderStates.viewport.get();
            oSpriteBatch->begin();
            oRenderer->renderStates.blendMode.push(OBlendOpaque);
            oRenderer->renderStates.sampleFiltering.push(OFilterNearest);
            oSpriteBatch->drawRect(g_pMainRenderTarget, ORectSmartFit(Rect{0, 0, OScreenf}, g_pMainRenderTarget->getSizef()));

            // Show the log
            if (oSettings->getShowOnScreenLog())
            {
                onut::drawLog();
            }
            oSpriteBatch->end();
            oRenderer->renderStates.blendMode.pop();
            oRenderer->renderStates.sampleFiltering.pop();

            if (postRenderCallback)
            {
                postRenderCallback();
            }
            
            if (oSettings->getShowFPS())
            {
                auto pFont = OGetFont("font.fnt");
                if (pFont)
                {
                    if (oSettings->getIsEditorMode())
                    {
                        static int step = 0;
                        step++;
                        switch (step % 4)
                        {
                            case 0: pFont->draw("|", { 0, 0 }); break;
                            case 1: pFont->draw("/", { 0, 0 }); break;
                            case 2: pFont->draw("-", { 0, 0 }); break;
                            case 3: pFont->draw("\\", { 0, 0 }); break;
                        }
                    }
                    else
                    {
                        pFont->draw("FPS: " + std::to_string(oTiming->getFPS()), { 0, 0 });
                    }
                }
            }

            oRenderer->endFrame();
        }

        cleanup();
    }

    void quit()
    {
#if defined(WIN32)
        PostQuitMessage(0);
#else
		g_bIsRunning = false;
#endif
    }
}

// Main
void initSettings();
void init();
void update();
void render();
void renderUI();
void postRender();

std::vector<std::string> OArguments;

#if defined(WIN32)
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    int argc;
    auto cmdLineW = onut::utf8ToUtf16(cmdLine);
    if (!cmdLineW.empty())
    {
        auto argvW = CommandLineToArgvW(cmdLineW.c_str(), &argc);
        for (int i = 0; i < argc; ++i)
        {
            OArguments.push_back(onut::utf16ToUtf8(argvW[i]));
        }
    }
    initSettings();
    onut::initLog();
    onut::run(init, update, render, renderUI, postRender);
    return 0;
}
#else
int main(int argc, char** argv)
{
    for (int i = 0; i < argc; ++i)
    {
        OArguments.push_back(argv[i]);
    }

    initSettings();
    onut::run(init, update, render, renderUI, postRender);
    return 0;
}
#endif
