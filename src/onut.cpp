#include "onut/ContentManager.h"
#include "onut/Dispatcher.h"
#include "onut/Font.h"
#include "onut/GamePad.h"
#include "onut/Input.h"
#include "onut/onut.h"
#include "onut/PrimitiveBatch.h"
#include "onut/Renderer.h"
#include "onut/Settings.h"
#include "onut/SpriteBatch.h"
#include "onut/Texture.h"
#include "onut/Timing.h"
#include "onut/UIContext.h"
#include "onut/UIControl.h"
#include "onut/UIPanel.h"
#include "onut/UITextBox.h"
#include "onut/Updater.h"
#include "onut/Window.h"

#include "audio/Audio.h"
#include "onut_old.h"

#include <cassert>
#include <mutex>
#include <sstream>

using namespace DirectX;

// Our engine services
AudioEngine*                        g_pAudioEngine = nullptr;
onut::ParticleSystemManager<>*      OParticles = nullptr;

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
            oSpriteBatch->drawRect(nullptr, (rect), Color(0, 0, 0, .5f));
        });
    }

    void createServices()
    {
        // Random
        randomizeSeed();

        // Dispatcher
        oDispatcher = ODispatcher::create();

        // Timing class
        oTiming = OTiming::create();

        // Updater
        oUpdater = OMake<OUpdater>();

        // Window
        oWindow = OWindow::create(oSettings->getResolution(), oSettings->getIsResizableWindow());

        // DirectX
        oRenderer = ORenderer::create(oWindow);
        oRenderer->init(oWindow);

        // SpriteBatch
        oSpriteBatch = SpriteBatch::create();
        oPrimitiveBatch = PrimitiveBatch::create();

        // Content
        oContentManager = OMake<ContentManager>();
        oContentManager->addDefaultSearchPaths();

        // Mouse/Keyboard
        oInput = OInput::create(oWindow);

        // Audio
#ifdef WIN32
        CoInitializeEx(nullptr, COINIT_MULTITHREADED);
#endif
        AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
        eflags = eflags | AudioEngine_Debug;
#endif
        try
        {
            g_pAudioEngine = new AudioEngine(eflags);
        }
        catch (std::exception e)
        {
        }

        // Particles
        OParticles = new ParticleSystemManager<>();

        // UI Context
        createUI();
    }

    void cleanup()
    {
        oDispatcher = nullptr;
        oUpdater = nullptr;
        oUI = nullptr;
        oUIContext = nullptr;
        delete OParticles;
        delete g_pAudioEngine;
        oInput = nullptr;
        oContentManager = nullptr;
        oPrimitiveBatch = nullptr;
        oSpriteBatch = nullptr;
        oRenderer = nullptr;
        oWindow = nullptr;
    }

    // Start the engine
    void run(std::function<void()> initCallback, std::function<void()> updateCallback, std::function<void()> renderCallback)
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
        MSG msg = {0};
        while (true)
        {
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

            // Sync to main callbacks
            oDispatcher->processQueue();

            // Update
            if (g_pAudioEngine) g_pAudioEngine->Update();
            auto framesToUpdate = oTiming->update(oSettings->getIsFixedStep());
            while (framesToUpdate--)
            {
                oInput->update();
                POINT cur;
                GetCursorPos(&cur);
                ScreenToClient(oWindow->getHandle(), &cur);
                oInput->mousePos.x = cur.x;
                oInput->mousePos.y = cur.y;
                oInput->mousePosf.x = static_cast<float>(cur.x);
                oInput->mousePosf.y = static_cast<float>(cur.y);
                oUpdater->update();
                auto mousePosf = OGetMousePos();
                if (oUIContext->useNavigation)
                {
                    oUI->update(oUIContext, Vector2(mousePosf.x, mousePosf.y), OGamePadPressed(OGamePadA), false, false,
                                OGamePadJustPressed(OGamePadDPadLeft) || OGamePadJustPressed(OGamePadLeftThumbLeft),
                                OGamePadJustPressed(OGamePadDPadRight) || OGamePadJustPressed(OGamePadLeftThumbRight),
                                OGamePadJustPressed(OGamePadDPadUp) || OGamePadJustPressed(OGamePadLeftThumbUp),
                                OGamePadJustPressed(OGamePadDPadDown) || OGamePadJustPressed(OGamePadLeftThumbDown),
                                0.f);
                }
                else
                {
                    oUI->update(oUIContext, Vector2(mousePosf.x, mousePosf.y),
                                OInputPressed(OMouse1), OInputPressed(OMouse2), OInputPressed(OMouse3),
                                false, false, false, false, 
                                OInputPressed(OKeyLeftControl), oInput->getStateValue(OMouseZ));
                }
                OParticles->update();
                if (updateCallback)
                {
                    updateCallback();
                }
            }

            // Render
            oTiming->render();
            oRenderer->beginFrame();
            if (renderCallback)
            {
                renderCallback();
            }
            OParticles->render();
            oSpriteBatch->begin();
            oUI->render(oUIContext);
            oSpriteBatch->end();
            oRenderer->endFrame();
        }

        cleanup();
    }
}
