#include <Windows.h>
#include "styles.h"

void init();
void update();
void render();

onut::UIContext* g_pUIContext = nullptr;
onut::UIControl* g_pUIScreen = nullptr;

int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    OSettings->setResolution({1280, 720});
    OSettings->setGameName("Oak Nut UI Editor");
    OSettings->setIsResizableWindow(true);

    onut::run(init, update, render);

    return 0;
}

void init()
{
    g_pUIContext = new onut::UIContext(onut::sUIVector2{OScreenWf, OScreenHf});
    g_pUIScreen = new onut::UIControl("../../assets/ui/editor.json");

    createUIStyles(g_pUIContext);
}

void update()
{
    g_pUIContext->resize({OScreenWf, OScreenHf});
    g_pUIScreen->update(*g_pUIContext, {OMousePos.x, OMousePos.y}, OInput->isStateDown(DIK_MOUSEB1));
}

void render()
{
    ORenderer->clear(OColorHex(1e1e1e));

    OSB->begin();
    g_pUIScreen->render(*g_pUIContext);
    OSB->end();
}
