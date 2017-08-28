// Oak Nut include
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/UIContext.h>
#include <onut/UIControl.h>

void initSettings()
{
    oSettings->setGameName("UI Navigation Sample");
}

void init()
{
    oUIContext->useNavigation = true; // We want to navigate the UI manually

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

void postRender()
{
}

void renderUI()
{
}
