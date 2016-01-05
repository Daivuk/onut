#include <Windows.h>
#include "ActionManager.h"
#include "DocumentView.h"
#include "events.h"
#include "menu.h"
#include "styles.h"

void init();
void update();
void render();

DocumentView*       g_pDocument = nullptr;
onut::UIContext*    g_pUIContext = nullptr;
onut::UIControl*    g_pUIScreen = nullptr;
onut::ActionManager g_actionManager;

int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    auto screenW = GetSystemMetrics(SM_CXSCREEN) * 4 / 5;
    auto screenH = GetSystemMetrics(SM_CYSCREEN) * 4 / 5;
    OSettings->setUserSettingDefault("width", std::to_string(screenW));
    OSettings->setUserSettingDefault("height", std::to_string(screenH));
    OSettings->setResolution({std::stoi(OSettings->getUserSetting("width")), 
                              std::stoi(OSettings->getUserSetting("height"))});
    OSettings->setGameName("Oak Nut UI Editor");
    OSettings->setIsResizableWindow(true);
    OSettings->setIsEditorMode(true);

    onut::run(init, update, render);

    return 0;
}

void init()
{
    OContentManager->addSearchPath("../../assets/textures/icons");

    g_pUIContext = new onut::UIContext(onut::sUIVector2{OScreenWf, OScreenHf});
    g_pUIContext->onClipping = [](bool enabled, const onut::sUIRect& rect)
    {
        OSB->end();
        ORenderer->setScissor(enabled, onut::UI2Onut(rect));
        OSB->begin();
    };
    createUIStyles(g_pUIContext);

    g_pUIContext->addStyle<onut::UIPanel>("sizableRegion", [](const onut::UIPanel* pPanel, const onut::sUIRect& rect)
    {
        OSB->drawRect(nullptr, onut::UI2Onut(rect), Color::Black);
        OSB->end();

        ORenderer->setScissor(true, onut::UI2Onut(rect));

        // Render edited UIs
        ORenderer->set2DCamera({-rect.position.x, -rect.position.y});
        OSB->begin();
        g_pDocument->render();
        OSB->end();

        ORenderer->setScissor(false, onut::UI2Onut(rect));

        ORenderer->set2DCamera({0, 0});
        OSB->begin();
    });

    g_pUIScreen = new onut::UIControl("../../assets/ui/editor.json");
    g_pUIScreen->retain();
    hookUIEvents(g_pUIScreen);

    g_pDocument = new DocumentView("");

    buildMenu();

    OWindow->onMenu = onMenu;
    OWindow->onWrite = [](char c){g_pUIContext->write(c); };
    OWindow->onKey = [](uintptr_t key)
    {
        if (!g_pDocument->isBusy())
        {
            g_pUIContext->keyDown(key);
            if (!dynamic_cast<onut::UITextBox*>(g_pUIContext->getFocusControl()))
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
    g_pUIScreen->update(*g_pUIContext, {OMousePos.x, OMousePos.y}, OInput->isStateDown(OINPUT_MOUSEB1), OInput->isStateDown(OINPUT_MOUSEB2), OInput->isStateDown(OINPUT_MOUSEB3));
    g_pDocument->update();
}

void render()
{
    ORenderer->clear(OColorHex(1e1e1e));

    OSB->begin();
    g_pUIScreen->render(*g_pUIContext);
    OSB->end();
}
