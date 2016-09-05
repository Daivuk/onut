// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/onut.h>
#include <onut/Settings.h>

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Text Sample");
    ORun();
}
