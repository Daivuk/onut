#include <Windows.h>
#include "ActionManager.h"
#include "DocumentView.h"
#include "events.h"
#include "menu.h"
#include "styles.h"

#include "onut/Input.h"
#include "onut/Renderer.h"
#include "onut/Settings.h"
#include "onut/Window.h"

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

    g_pUIContext = new onut::UIContext(onut::sUIVector2{OScreenWf, OScreenHf});
    g_pUIContext->onClipping = [](bool enabled, const onut::sUIRect& rect)
    {
        oSpriteBatch->end();
        oRenderer->renderStates.scissorEnabled = enabled;
        if (enabled)
        {
            oRenderer->renderStates.scissor = {
                static_cast<int>(rect.position.x), 
                static_cast<int>(rect.position.y), 
                static_cast<int>(rect.position.x + rect.size.x),
                static_cast<int>(rect.position.y + rect.size.y)};
        }
        oSpriteBatch->begin();
    };
    createUIStyles(g_pUIContext);

    g_pUIContext->addStyle<onut::UIPanel>("sizableRegion", [](const onut::UIPanel* pPanel, const onut::sUIRect& rect)
    {
        oSpriteBatch->drawRect(nullptr, onut::UI2Onut(rect), Color::Black);
        oSpriteBatch->end();

        oRenderer->renderStates.scissorEnabled = true;
        oRenderer->renderStates.scissor = {
            static_cast<int>(rect.position.x),
            static_cast<int>(rect.position.y),
            static_cast<int>(rect.position.x + rect.size.x),
            static_cast<int>(rect.position.y + rect.size.y)};

        // Render edited UIs
        //oRenderer->set2DCamera();
        oSpriteBatch->begin(Matrix::CreateTranslation(rect.position.x, rect.position.y, 0.f));
        g_pDocument->render();
        oSpriteBatch->end();

        oRenderer->renderStates.scissorEnabled = false;

        oRenderer->set2DCamera({0, 0});
        oSpriteBatch->begin();
    });

    g_pUIScreen = new onut::UIControl("../../assets/ui/editor.json");
    g_pUIScreen->retain();
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
    g_pUIScreen->update(*g_pUIContext, {oInput->mousePosf.x, oInput->mousePosf.y}, OInputPressed(OMouse1), OInputPressed(OMouse2), OInputPressed(OMouse3),
                        false, false, false, false,
                        OInputPressed(OKeyLeftControl), oInput->getStateValue(OMouseZ));
    g_pDocument->update();
}

void render()
{
    oRenderer->clear(OColorHex(1e1e1e));

    oSpriteBatch->begin();
    g_pUIScreen->render(*g_pUIContext);
    oSpriteBatch->end();
}
