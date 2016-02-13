// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include "onut/ParticleSystem.h"
#include "onut/Font.h"
#include "onut_old.h"

void init();
void update();
void render();

OEmitterInstance emitter;
OAnim3 position;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Particles Sample");
    ORun(init, update, render);
}

void init()
{
}

void update()
{
    emitter.setTransform(position);

    if (OJustPressed(OINPUT_1))
    {
        OEmitPFX("test.pfx", Vector3{OScreenWf / 3, OScreenHf / 2, 0});
    }
    if (OJustPressed(OINPUT_2))
    {
        if (emitter.isPlaying())
        {
            emitter.stop();
        }
        else
        {
            // Animate it back and forth across the screen
            position.start(Vector3{OScreenWf - 100, OScreenHf / 2, 0}, Vector3{100, OScreenHf / 2, 0}, 2.f, OLinear, OPingPongLoop);

            emitter = OEmitPFX("test2.pex", position);
            emitter.setRenderEnabled(false);
        }
    }
}

void render()
{
    // Clear to black
    ORenderer->clear({0, 0, 0, 1});

    OSB->begin();

    // Draw info
    auto pFont = OGetFont("font.fnt");
    pFont->draw("Press ^9901^999 to spawn particles from an onut PFX file", {10, 10});
    pFont->draw("Press ^9902^999 to spawn particles from PEX file", {10, 30});

    // It is possible to manually call render an emitter,
    // so we can specify in which order it is renderer manually
    emitter.render();

    pFont->draw("FPS: " + std::to_string(onut::getTimeInfo().getFPS()), {10, 50});

    OSB->end();
}
