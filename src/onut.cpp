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
onut::UIContext*                    OUIContext = nullptr;
onut::UIControl*                    OUI = nullptr;

namespace onut
{
    void createUI()
    {
        OUIContext = new UIContext(sUIVector2(OScreenWf, OScreenHf));
        OUI = new UIControl();
        OUI->retain();
        OUI->widthType = eUIDimType::DIM_RELATIVE;
        OUI->heightType = eUIDimType::DIM_RELATIVE;

        OUIContext->onClipping = [](bool enabled, const onut::sUIRect& rect)
        {
            oSpriteBatch->end();
            oRenderer->renderStates.scissorEnabled = enabled;
            oRenderer->renderStates.scissor = iRect{
                static_cast<int>(rect.position.x),
                static_cast<int>(rect.position.y),
                static_cast<int>(rect.position.x + rect.size.x),
                static_cast<int>(rect.position.y + rect.size.y)
            };
            oSpriteBatch->begin();
        };

        auto getTextureForState = [](onut::UIControl *pControl, const std::string &filename)
        {
            static std::string stateFilename;
            stateFilename = filename;
            OTextureRef pTexture;
            switch (pControl->getState(*OUIContext))
            {
                case onut::eUIState::NORMAL:
                    pTexture = OGetTexture(filename.c_str());
                    break;
                case onut::eUIState::DISABLED:
                    stateFilename.insert(filename.size() - 4, "_disabled");
                    pTexture = OGetTexture(stateFilename.c_str());
                    if (!pTexture) pTexture = OGetTexture(filename.c_str());
                    break;
                case onut::eUIState::HOVER:
                    stateFilename.insert(filename.size() - 4, "_hover");
                    pTexture = OGetTexture(stateFilename.c_str());
                    if (!pTexture) pTexture = OGetTexture(filename.c_str());
                    break;
                case onut::eUIState::DOWN:
                    stateFilename.insert(filename.size() - 4, "_down");
                    pTexture = OGetTexture(stateFilename.c_str());
                    if (!pTexture) pTexture = OGetTexture(filename.c_str());
                    break;
            }
            return pTexture;
        };

        OUIContext->drawRect = [=](onut::UIControl *pControl, const onut::sUIRect &rect, const onut::sUIColor &color)
        {
            oSpriteBatch->drawRect(nullptr, onut::UI2Onut(rect), onut::UI2Onut(color));
        };

        OUIContext->drawTexturedRect = [=](onut::UIControl *pControl, const onut::sUIRect &rect, const onut::sUIImageComponent &image)
        {
            oSpriteBatch->drawRect(getTextureForState(pControl, image.filename),
                          onut::UI2Onut(rect),
                          onut::UI2Onut(image.color));
        };

        OUIContext->drawScale9Rect = [=](onut::UIControl* pControl, const onut::sUIRect& rect, const onut::sUIScale9Component& scale9)
        {
            const std::string &filename = scale9.image.filename;
            static std::string stateFilename;
            stateFilename = filename;
            OTextureRef pTexture;
            switch (pControl->getState(*OUIContext))
            {
                case onut::eUIState::NORMAL:
                    pTexture = OGetTexture(filename.c_str());
                    break;
                case onut::eUIState::DISABLED:
                    stateFilename.insert(filename.size() - 4, "_disabled");
                    pTexture = OGetTexture(stateFilename.c_str());
                    if (!pTexture) pTexture = OGetTexture(filename.c_str());
                    break;
                case onut::eUIState::HOVER:
                    stateFilename.insert(filename.size() - 4, "_hover");
                    pTexture = OGetTexture(stateFilename.c_str());
                    if (!pTexture) pTexture = OGetTexture(filename.c_str());
                    break;
                case onut::eUIState::DOWN:
                    stateFilename.insert(filename.size() - 4, "_down");
                    pTexture = OGetTexture(stateFilename.c_str());
                    if (!pTexture) pTexture = OGetTexture(filename.c_str());
                    break;
            }
            if (scale9.isRepeat)
            {
                oSpriteBatch->drawRectScaled9RepeatCenters(getTextureForState(pControl, scale9.image.filename),
                                                  onut::UI2Onut(rect),
                                                  onut::UI2Onut(scale9.padding),
                                                  onut::UI2Onut(scale9.image.color));
            }
            else
            {
                oSpriteBatch->drawRectScaled9(getTextureForState(pControl, scale9.image.filename),
                                     onut::UI2Onut(rect),
                                     onut::UI2Onut(scale9.padding),
                                     onut::UI2Onut(scale9.image.color));
            }
        };

        OUIContext->drawText = [=](onut::UIControl* pControl, const onut::sUIRect& rect, const onut::sUITextComponent& text)
        {
            if (text.text.empty()) return;
            auto align = onut::UI2Onut(text.font.align);
            auto oRect = onut::UI2Onut(rect);
            auto pFont = OGetFont(text.font.typeFace.c_str());
            auto oColor = onut::UI2Onut(text.font.color);
            if (pControl->getState(*OUIContext) == onut::eUIState::DISABLED)
            {
                oColor = {.4f, .4f, .4f, 1};
            }
            oColor.Premultiply();

            if (pFont)
            {
                if (pControl->getStyleName() == "password")
                {
                    std::string pwd;
                    pwd.resize(text.text.size(), '*');
                    if (pControl->hasFocus(*OUIContext) && ((onut::UITextBox*)pControl)->isCursorVisible())
                    {
                        pwd.back() = '_';
                    }
                    pFont->draw(pwd, ORectAlign<>(oRect, align), Vector2(align), oColor);
                }
                else
                {
                    pFont->draw(text.text, ORectAlign<>(oRect, align), Vector2(align), oColor);
                }
            }
        };

        OUIContext->addTextCaretSolver<onut::UITextBox>("", [=](const onut::UITextBox* pTextBox, const onut::sUIVector2& localPos) -> decltype(std::string().size())
        {
            auto pFont = OGetFont(pTextBox->textComponent.font.typeFace.c_str());
            if (!pFont) return 0;
            auto& text = pTextBox->textComponent.text;
            return pFont->caretPos(text, localPos.x - 4);
        });

        oWindow->onWrite = [](char c)
        {
            OUIContext->write(c);
        };
        oWindow->onKey = [](uintptr_t key)
        {
            OUIContext->keyDown(key);
        };

        OUIContext->addStyle<onut::UIPanel>("blur", [](const onut::UIPanel* pPanel, const onut::sUIRect& rect)
        {
            oSpriteBatch->end();
            oRenderer->renderStates.renderTarget.get()->blur();
            oSpriteBatch->begin();
            oSpriteBatch->drawRect(nullptr, onut::UI2Onut(rect), Color(0, 0, 0, .5f));
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
        delete OUIContext;
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
                if (OUIContext->useNavigation)
                {
                    OUI->update(*OUIContext, sUIVector2(mousePosf.x, mousePosf.y), OGamePadPressed(OGamePadA), false, false,
                                OGamePadJustPressed(OGamePadDPadLeft) || OGamePadJustPressed(OGamePadLeftThumbLeft),
                                OGamePadJustPressed(OGamePadDPadRight) || OGamePadJustPressed(OGamePadLeftThumbRight),
                                OGamePadJustPressed(OGamePadDPadUp) || OGamePadJustPressed(OGamePadLeftThumbUp),
                                OGamePadJustPressed(OGamePadDPadDown) || OGamePadJustPressed(OGamePadLeftThumbDown),
                                0.f);
                }
                else
                {
                    OUI->update(*OUIContext, sUIVector2(mousePosf.x, mousePosf.y), 
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
            OUI->render(*OUIContext);
            oSpriteBatch->end();
            oRenderer->endFrame();
        }

        cleanup();
    }
}
