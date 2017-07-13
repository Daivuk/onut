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
#include <onut/Window.h>

// Private
#include "JSBindings.h"

// STL
#include <cassert>
#include <mutex>
#include <sstream>

OTextureRef g_pMainRenderTarget;

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
    }

    void cleanup()
    {
        onut::js::shutdown();

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

    // Start the engine
    void run(std::function<void()> initCallback,
             std::function<void()> updateCallback, 
             std::function<void()> renderCallback,
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

            // Render
            oTiming->render();
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

            // Draw final render target
            oRenderer->renderStates.renderTarget = nullptr;
            const auto& res = oRenderer->getResolution();
            oRenderer->renderStates.viewport = iRect{0, 0, res.x, res.y};
            oRenderer->renderStates.scissorEnabled = false;
            oRenderer->renderStates.scissor = oRenderer->renderStates.viewport.get();
            oSpriteBatch->begin();
            oSpriteBatch->changeBlendMode(OBlendOpaque);
            oSpriteBatch->changeFiltering(OFilterNearest);
            oSpriteBatch->drawRect(g_pMainRenderTarget, ORectSmartFit(Rect{0, 0, OScreenf}, g_pMainRenderTarget->getSizef()));

            // Show the log
            if (oSettings->getShowOnScreenLog())
            {
                onut::drawLog();
            }
            oSpriteBatch->end();
            oSpriteBatch->changeBlendMode(OBlendAlpha);
            oSpriteBatch->changeFiltering(OFilterLinear);

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
    onut::run(init, update, render, postRender);
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
    onut::run(init, update, render, postRender);
    return 0;
}
#endif
