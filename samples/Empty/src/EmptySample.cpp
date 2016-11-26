// Oak Nut include
#include <onut/Log.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>

void initSettings()
{
	OLog("initSettings");
    oSettings->setGameName("Empty Sample");
}

void init()
{
	OLog("init");
}

void update()
{
	OLog("update");
}

void render()
{
	OLog("render");
    // Clear
    oRenderer->clear(OColorHex(1d232d));
}

void postRender()
{
	OLog("postRender");
}
