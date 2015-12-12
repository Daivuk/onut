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
    OSettings->setGameName("Text Sample");
    ORun(init, update, render);
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
    ORenderer->clear({.35f, .35f, .35f, 1});

    auto pFont = OGetBMFont("main.fnt");
    auto pFontBig = OGetBMFont("mainBig.fnt");

    // Normal text
    pFont->draw("Lorem ipsum dolor sit amet.", {10, 10});

    // Bigger font
    pFontBig->draw("CONSECTETUR ADIPISCING ELIT", {10, 100});

    // Playing with colors
    // ^RGB, where RGB ranged from 0-9
    pFont->draw("^933Sed ^993do ^393eiusmod ^399tempor ^339incididunt ^939ut ^111labore ^222et ^444dolore ^666magna ^888aliqua", {10, 200});

    // Alignment
    pFont->draw<OLeft>("Left aligned", {10, 300});
    pFont->draw<OCenter>("Center aligned", {OScreenWf / 2, 300});
    pFont->draw<ORight>("Right aligned", {OScreenWf - 10, 300});

    // Cheap outline
    pFont->drawOutlined("Duis aute irure dolor in reprehenderit - Cheap oulines", {10, 400});

    // Pretty outline
    pFont->drawOutlined<OTopLeft, false>("Duis aute irure dolor in reprehenderit - Better outlines", {10, 450});
}
