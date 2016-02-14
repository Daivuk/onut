#include "onut/ContentManager.h"
#include "onut/Font.h"
#include "onut/GamePad.h"
#include "onut/Input.h"
#include "onut/onut.h"
#include "onut/Settings.h"
#include "onut/Texture.h"
#include "onut/Updater.h"

#include "audio/Audio.h"
#include "onut_old.h"
#include "Window.h"

#include <cassert>
#include <mutex>
#include <sstream>

using namespace DirectX;

// Our engine services
onut::Window*                       OWindow = nullptr;
onut::Renderer*                     ORenderer = nullptr;
onut::SpriteBatch*                  OSpriteBatch = nullptr;
onut::PrimitiveBatch*               OPrimitiveBatch = nullptr;
AudioEngine*                        g_pAudioEngine = nullptr;
onut::TimeInfo<>                    g_timeInfo;
onut::Synchronous                   g_mainSync;
onut::ParticleSystemManager<>*      OParticles = nullptr;
Vector2                             OMousePos;
onut::UIContext*                    OUIContext = nullptr;
onut::UIControl*                    OUI = nullptr;

// So commonly used stuff
float                               ODT = 0.f;

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
            OSB->end();
            ORenderer->setScissor(enabled, onut::UI2Onut(rect));
            OSB->begin();
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
            OSB->drawRect(nullptr, onut::UI2Onut(rect), onut::UI2Onut(color));
        };

        OUIContext->drawTexturedRect = [=](onut::UIControl *pControl, const onut::sUIRect &rect, const onut::sUIImageComponent &image)
        {
            OSB->drawRect(getTextureForState(pControl, image.filename),
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
                OSB->drawRectScaled9RepeatCenters(getTextureForState(pControl, scale9.image.filename),
                                                  onut::UI2Onut(rect),
                                                  onut::UI2Onut(scale9.padding),
                                                  onut::UI2Onut(scale9.image.color));
            }
            else
            {
                OSB->drawRectScaled9(getTextureForState(pControl, scale9.image.filename),
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

        OWindow->onWrite = [](char c)
        {
            OUIContext->write(c);
        };
        OWindow->onKey = [](uintptr_t key)
        {
            OUIContext->keyDown(key);
        };

        OUIContext->addStyle<onut::UIPanel>("blur", [](const onut::UIPanel* pPanel, const onut::sUIRect& rect)
        {
            OSB->end();
            ORenderer->getRenderTarget()->blur();
            OSB->begin();
            OSB->drawRect(nullptr, onut::UI2Onut(rect), Color(0, 0, 0, .5f));
        });
    }

    void createServices()
    {
        // Random
        randomizeSeed();

        oUpdater = std::make_shared<OUpdater>();

        // Window
        OWindow = new Window(oSettings->getResolution(), oSettings->getIsResizableWindow());

        // DirectX
        ORenderer = new Renderer(*OWindow);

        // SpriteBatch
        OSB = new SpriteBatch();
        OPB = new PrimitiveBatch();

        // Content
        oContentManager = std::make_shared<ContentManager>();
        oContentManager->addDefaultSearchPaths();

        // Mouse/Keyboard
        oInput = OInput::create();

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
        oUpdater = nullptr;
        delete OUIContext;
        delete OParticles;
        delete g_pAudioEngine;
        oInput = nullptr;
        oContentManager = nullptr;
        delete OPB;
        delete OSB;
        delete ORenderer;
        delete OWindow;
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
            g_mainSync.processQueue();

            // Update
            if (g_pAudioEngine) g_pAudioEngine->Update();
            auto framesToUpdate = g_timeInfo.update(oSettings->getIsFixedStep());
            ODT = onut::getTimeInfo().getDeltaTime<float>();
            while (framesToUpdate--)
            {
                oInput->update();
                POINT cur;
                GetCursorPos(&cur);
                ScreenToClient(OWindow->getHandle(), &cur);
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
            g_timeInfo.render();
            ORenderer->beginFrame();
            if (renderCallback)
            {
                renderCallback();
            }
            OParticles->render();
            OSB->begin();
            OUI->render(*OUIContext);
            OSB->end();
            ORenderer->endFrame();
        }

        cleanup();
    }

    const TimeInfo<>& getTimeInfo()
    {
        return g_timeInfo;
    }
}
