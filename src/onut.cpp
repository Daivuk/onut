#if !defined(__unix__)
// Onut includes
#include <onut/ActionManager.h>
#include <onut/AudioEngine.h>
//#include <onut/Cloud.h>
#include <onut/ComponentFactory.h>
#include <onut/ContentManager.h>
#endif // __unix__
#include <onut/Dispatcher.h>
#if !defined(__unix__)
#include <onut/SceneManager.h>
#include <onut/Font.h>
#include <onut/GamePad.h>
#include <onut/Input.h>
#include <onut/onut.h>
#include <onut/ParticleSystemManager.h>
#include <onut/PrimitiveBatch.h>
#endif // __unix__
#include <onut/Random.h>
#if !defined(__unix__)
#include <onut/Renderer.h>
#endif // __unix__
#include <onut/Settings.h>
#if !defined(__unix__)
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
#endif // __unix__
#include <onut/ThreadPool.h>
#include <onut/Timing.h>
#if !defined(__unix__)
#include <onut/UIContext.h>
#include <onut/UIControl.h>
#include <onut/UIPanel.h>
#include <onut/UITextBox.h>
#endif // __unix__
#include <onut/Updater.h>
#if !defined(__unix__)
#include <onut/Window.h>

// Private
#include "JSBindings.h"
#endif // __unix__

// STL
#include <cassert>
#include <mutex>
#include <sstream>

#if !defined(__unix__)
OTextureRef g_pMainRenderTarget;
#endif // __unix__

namespace onut
{
    void createUI()
    {
#if !defined(__unix__)
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
#endif // __unix__
    }

    void createServices()
    {
        // Random
        randomizeSeed();

        // Thread pool
        oThreadPool = OThreadPool::create();

        // Dispatcher
        oDispatcher = ODispatcher::create();

        // Timing class
        oTiming = OTiming::create();

        // Updater
        oUpdater = OUpdater::create();

#if !defined(__unix__)
        // Window
        oWindow = OWindow::create();

        // Renderer
        oRenderer = ORenderer::create(oWindow);
        oRenderer->init(oWindow);

        // SpriteBatch
        oSpriteBatch = SpriteBatch::create();
        oPrimitiveBatch = PrimitiveBatch::create();

        // Content
        oContentManager = ContentManager::create();

        // Cloud
        //oCloud = Cloud::create(oSettings->getAppId(), oSettings->getAppSecret());

        // Mouse/Keyboard
        oInput = OInput::create(oWindow);

        // Audio
        oAudioEngine = AudioEngine::create();

        // Particles
        oParticleSystemManager = ParticleSystemManager::create();

        // UI Context
        createUI();

        // Component factory
        oComponentFactory = ComponentFactory::create();
        oComponentFactory->registerDefaultComponents();

        // Entity Manager
        oSceneManager = SceneManager::create();

        // Undo/Redo for editors
        oActionManager = ActionManager::create();

        g_pMainRenderTarget = OTexture::createScreenRenderTarget();
        
        // Initialize Javascript
        onut::js::init();
#endif // __unix__
    }

    void cleanup()
    {
#if !defined(__unix__)
        onut::js::shutdown();

        g_pMainRenderTarget = nullptr;
        oActionManager = nullptr;
        oSceneManager = nullptr;
        oComponentFactory = nullptr;
#endif // __unix__
        oDispatcher = nullptr;
        oUpdater = nullptr;
#if !defined(__unix__)
        oUI = nullptr;
        oUIContext = nullptr;
        oParticleSystemManager = nullptr;
        //delete g_pAudioEngine;
        oAudioEngine = nullptr;
        oInput = nullptr;
        //oCloud = nullptr;
        oContentManager = nullptr;
        oPrimitiveBatch = nullptr;
        oSpriteBatch = nullptr;
        oRenderer = nullptr;
        oWindow = nullptr;
#endif // __unix__
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
#if defined(WIN32)
        MSG msg = {0};
#endif // WIN32
        while (true)
        {
#if defined(WIN32)
            if (oSettings->getIsEditorMode())
            {
                if (GetMessage(&msg, 0, 0, 0) >= 0)
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);

                    if (msg.message == WM_QUIT)
                    {
                        break;
                    }
                }
            }
            else
            {
                if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);

                    if (msg.message == WM_QUIT)
                    {
                        break;
                    }
                }
            }
#elif defined(__unix__)
#endif

            // Sync to main callbacks
            oDispatcher->processQueue();

#if !defined(__unix__)
            // Update
            oAudioEngine->update();
#endif // __unix__
            auto framesToUpdate = oTiming->update(oSettings->getIsFixedStep());
            while (framesToUpdate--)
            {
#if !defined(__unix__)
                oInput->update();
                POINT cur;
                GetCursorPos(&cur);
                ScreenToClient(oWindow->getHandle(), &cur);
                oInput->mousePos.x = cur.x;
                oInput->mousePos.y = cur.y;
                oInput->mousePosf.x = static_cast<float>(cur.x);
                oInput->mousePosf.y = static_cast<float>(cur.y);
#endif // __unix__
                oUpdater->update();
#if !defined(__unix__)
                auto mousePosf = OGetMousePos();
                if (oUIContext->useNavigation)
                {
                    oUI->update(oUIContext, Vector2(mousePosf.x, mousePosf.y), OGamePadPressed(OGamePadA) || OInputJustPressed(OKeyEnter), false, false,
                                OGamePadJustPressed(OGamePadDPadLeft) || OGamePadJustPressed(OGamePadLeftThumbLeft) || OInputJustPressed(OKeyLeft),
                                OGamePadJustPressed(OGamePadDPadRight) || OGamePadJustPressed(OGamePadLeftThumbRight) || OInputJustPressed(OKeyRight),
                                OGamePadJustPressed(OGamePadDPadUp) || OGamePadJustPressed(OGamePadLeftThumbUp) || OInputJustPressed(OKeyUp),
                                OGamePadJustPressed(OGamePadDPadDown) || OGamePadJustPressed(OGamePadLeftThumbDown) || OInputJustPressed(OKeyDown),
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
#endif // __unix__
                if (updateCallback)
                {
                    updateCallback();
                }
            }

            // Render
            oTiming->render();
#if !defined(__unix__)
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
            auto& res = oRenderer->getResolution();
            oRenderer->renderStates.viewport = iRect{0, 0, res.x, res.y};
            oRenderer->renderStates.scissorEnabled = false;
            oRenderer->renderStates.scissor = oRenderer->renderStates.viewport.get();
            oSpriteBatch->begin();
            oSpriteBatch->changeBlendMode(OBlendOpaque);
            oSpriteBatch->changeFiltering(OFilterNearest);
            oSpriteBatch->drawRect(g_pMainRenderTarget, ORectFit(Rect{0, 0, OScreenf}, g_pMainRenderTarget->getSizef()));
            oSpriteBatch->end();
            oSpriteBatch->changeBlendMode(OBlendAlpha);
            oSpriteBatch->changeFiltering(OFilterLinear);
#endif // __unix__
            if (postRenderCallback)
            {
                postRenderCallback();
            }
#if !defined(__unix__)
            oRenderer->endFrame();
#endif // __unix__
        }

        cleanup();
    }

    void quit()
    {
#if !defined(__unix__)
        PostQuitMessage(0);
#endif // __unix__
    }
}

// Main
void initSettings();
void init();
void update();
void render();
void postRender();

#if defined(WIN32)
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    initSettings();
    onut::run(init, update, render, postRender);
    return 0;
}
#elif defined(__unix__)
int main(int argc, char** argv)
{
    initSettings();
    onut::run(init, update, render, postRender);
    return 0;
}
#endif
