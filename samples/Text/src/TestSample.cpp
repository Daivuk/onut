// Oak Nut include
#include <onut/Font.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>

void initSettings()
{
    oSettings->setGameName("Text Sample");
}

void init()
{
}

void update()
{
}

void render()
{
    // Clear to dark gray
    oRenderer->clear({.35f, .35f, .35f, 1});

    auto pFont = OGetFont("main.fnt");
    auto pFontBig = OGetFont("mainBig.fnt");

    // Normal text
    pFont->draw("Lorem ipsum dolor sit amet.", {10, 10});

    // Bigger font
    pFontBig->draw("CONSECTETUR ADIPISCING ELIT", {10, 100});

    // Playing with colors
    // ^RGB, where RGB ranged from 0-9
    pFont->draw("^933Sed ^993do ^393eiusmod ^399tempor ^339incididunt ^939ut ^111labore ^222et ^444dolore ^666magna ^888aliqua", {10, 200});

    // Alignment
    pFont->draw("Left aligned", {10, 300}, OLeft);
    pFont->draw("Center aligned", {OScreenWf / 2, 300}, OCenter);
    pFont->draw("Right aligned", {OScreenWf - 10, 300}, ORight);

    // Cheap outline
    pFont->drawOutlined("Duis aute irure dolor in reprehenderit - Cheap oulines", {10, 400});

    // Pretty outline
    pFont->drawOutlined("Duis aute irure dolor in reprehenderit - Better outlines", {10, 450}, OTopLeft, Color::White, Color(0, 0, 0, .75f), 2.f, false);
}

void postRender()
{
}

void renderUI()
{
}
