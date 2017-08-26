// Oak Nut include
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Texture.h>

// Third parties
#include <imgui/imgui.h>

void initSettings()
{
    oSettings->setResolution({ 1024, 768 });
    oSettings->setGameName("Imgui Sample");
}

void init()
{
}

void update()
{
}

void render()
{
    // Clear
    oRenderer->clear(OColorHex(1d232d));
}

void renderUI()
{
    ImGui::ShowTestWindow();
}

void postRender()
{
}
