#include <Windows.h>
#include "DocumentView.h"
#include "events.h"
#include "menu.h"
#include "styles.h"

#include "onut/ActionManager.h"
#include "onut/Input.h"
#include "onut/onut.h"
#include "onut/Renderer.h"
#include "onut/Settings.h"
#include "onut/SpriteBatch.h"
#include "onut/UIContext.h"
#include "onut/UIControl.h"
#include "onut/UIPanel.h"
#include "onut/Window.h"

void init();
void update();
void render();

DocumentView*       g_pDocument = nullptr;
OUIContextRef g_pUIContext = nullptr;
OUIControlRef g_pUIScreen = nullptr;

int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    auto screenW = GetSystemMetrics(SM_CXSCREEN) * 4 / 5;
    auto screenH = GetSystemMetrics(SM_CYSCREEN) * 4 / 5;
    oSettings->setUserSettingDefault("width", std::to_string(screenW));
    oSettings->setUserSettingDefault("height", std::to_string(screenH));
    oSettings->setResolution({std::stoi(oSettings->getUserSetting("width")),
                             std::stoi(oSettings->getUserSetting("height"))});
    oSettings->setGameName("Oak Nut UI Editor");
    oSettings->setIsResizableWindow(true);
    oSettings->setIsEditorMode(true);

    onut::run(init, update, render);

    return 0;
}

void init()
{
    oContentManager->addSearchPath("../../assets/textures/icons");

    g_pUIContext = OUIContext::create(Vector2{OScreenWf, OScreenHf});
    createUIStyles(g_pUIContext);

    g_pUIContext->addStyle<OUIPanel>("sizableRegion", [](const OUIPanelRef& pPanel, const Rect& rect)
    {
        oSpriteBatch->drawRect(nullptr, (rect), Color::Black);
        oSpriteBatch->end();

        oRenderer->renderStates.scissorEnabled = true;
        oRenderer->renderStates.scissor = {
            static_cast<int>(rect.x),
            static_cast<int>(rect.y),
            static_cast<int>(rect.x + rect.z),
            static_cast<int>(rect.y + rect.w)};

        // Render edited UIs
        //oRenderer->set2DCamera();
        oSpriteBatch->begin(Matrix::CreateTranslation(rect.x, rect.y, 0.f));
        g_pDocument->render();
        oSpriteBatch->end();

        oRenderer->renderStates.scissorEnabled = false;

        oRenderer->set2DCamera({0, 0});
        oSpriteBatch->begin();
    });

    g_pUIScreen = OUIControl::createFromFile("editor.json");
    hookUIEvents(g_pUIScreen);

    g_pDocument = new DocumentView("");

    buildMenu();

    oWindow->onMenu = onMenu;
    oWindow->onWrite = [](char c){g_pUIContext->write(c); };
    oWindow->onKey = [](uintptr_t key)
    {
        if (!g_pDocument->isBusy())
        {
            g_pUIContext->keyDown(key);
            if (!ODynamicCast<OUITextBox>(g_pUIContext->getFocusControl()))
            {
                checkShortCut(key);
                g_pDocument->onKeyDown(key);
            }
        }
    };
}

void update()
{
    // Adjust size
    g_pUIContext->resize({OScreenWf, OScreenHf});

    // Update.
    g_pUIScreen->update(g_pUIContext, {oInput->mousePosf.x, oInput->mousePosf.y}, OInputPressed(OMouse1), OInputPressed(OMouse2), OInputPressed(OMouse3),
                        false, false, false, false,
                        OInputPressed(OKeyLeftControl), oInput->getStateValue(OMouseZ));
    g_pDocument->update();
}

void render()
{
    oRenderer->clear(OColorHex(1e1e1e));

    oSpriteBatch->begin();
    g_pUIScreen->render(g_pUIContext);
    oSpriteBatch->end();
}
