// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/UIButton.h>
#include <onut/UIImage.h>
#include <onut/UILabel.h>

void init();
void update();
void render();

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("UI from Code Sample");
    ORun(init, update, render);
}

void init()
{
    // Note: UIs are meant to be created using the editor. Doing it manually is a bit tedious.

    // Add frame in the middle of the screen
    auto pFrame = OUIImage::create();
    pFrame->anchor = {0.5f, 0.5f};
    pFrame->align = OCenter;
    pFrame->rect = {{0, 0}, {400, 300}};
    pFrame->scale9Component.image.filename = "Frame.png";
    pFrame->scale9Component.padding = {24, 24, 24, 24};
    pFrame->scale9Component.isScaled9 = true;
    oUI->add(pFrame);

    // Title label
    auto pTitle = OUILabel::create();
    pTitle->anchor = {0.5f, 0.0f};
    pTitle->align = OTop;
    pTitle->rect = {{0, 8}, {-16, 24}};
    pTitle->widthType = OUIControl::DimType::Relative;
    pTitle->textComponent.font.align = OCenter;
    pTitle->textComponent.font.typeFace = "font.fnt";
    pTitle->textComponent.font.color = Color(1, 1, .5f, 1);
    pTitle->textComponent.text = "Dialog's Title";
    pFrame->add(pTitle);

    // Add a quit button in bottom left of the frame
    auto pQuitButton = OUIButton::create();
    pQuitButton->anchor = {1, 1};
    pQuitButton->align = OBottomRight;
    pQuitButton->rect = {{-24, -24}, {100, 34}};
    pQuitButton->textComponent.font.typeFace = "font.fnt";
    pQuitButton->textComponent.text = "QUIT";
    pQuitButton->scale9Component.image.filename = "button.png";
    pQuitButton->scale9Component.padding = {13, 0, 13, 0};
    pQuitButton->scale9Component.isScaled9 = true;
    pQuitButton->onClick = [](const OUIControlRef& p, const onut::UIMouseEvent& e){ OQuit(); };
    pFrame->add(pQuitButton);
}

void update()
{
}

void render()
{
    // Clear
    oRenderer->clear(OColorHex(1d232d));
}
