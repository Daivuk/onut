// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include "onut.h"

void init();
void update();
void render();

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    OSettings->setGameName("UI Navigation Sample");
    ORun(init, update, render);
}

void init()
{
    OUIContext->useNavigation = true; // We want to navigate the UI manually

    // Load our sample UI
    OUI->add(OLoadUI("example.json"));

    // Bind events
    OFindUI("QuitButton")->onClick = [](onut::UIControl* pButton, const onut::UIMouseEvent& mouseEvent)
    {
        OQuit();
    };
    OFindUI("ShowDialogButton")->onClick = [](onut::UIControl* pButton, const onut::UIMouseEvent& mouseEvent)
    {
        OFindUI("Dialog")->isVisible = true;
    };
    OFindUI("CloseDialogButton")->onClick = [](onut::UIControl* pButton, const onut::UIMouseEvent& mouseEvent)
    {
        OFindUI("Dialog")->isVisible = false;
    };
}

void update()
{
}

void render()
{
    // Clear
    ORenderer->clear(OColorHex(1d232d));
}
