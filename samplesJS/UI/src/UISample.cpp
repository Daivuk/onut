// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/UIContext.h>
#include <onut/UIControl.h>

void init();
void update();
void render();

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("UI Sample");
    ORun(init, update, render);
}

void init()
{
    // Load our sample UI
    oUI->add(OUIControl::createFromFile("example.json"));

    // Bind events
    OFindUI("QuitButton")->onClick = [](const OUIControlRef& pButton, const onut::UIMouseEvent& mouseEvent)
    {
        OQuit();
    };
    OFindUI("ShowDialogButton")->onClick = [](const OUIControlRef& pButton, const onut::UIMouseEvent& mouseEvent)
    {
        OFindUI("Dialog")->isVisible = true;
    };
    OFindUI("CloseDialogButton")->onClick = [](const OUIControlRef& pButton, const onut::UIMouseEvent& mouseEvent)
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
    oRenderer->clear(OColorHex(1d232d));
}
