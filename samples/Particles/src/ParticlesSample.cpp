// Oak Nut include
#include <onut/Anim.h>
#include <onut/ParticleSystem.h>
#include <onut/ParticleSystemManager.h>
#include <onut/Font.h>
#include <onut/Input.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/Timing.h>

OEmitterInstance emitter;
OAnimVector3 position;

void initSettings()
{
    oSettings->setGameName("Particles Sample");
    oSettings->setShowFPS(true);
}

void init()
{
}

void update()
{
    emitter.setTransform(position);

    if (OInputJustPressed(OKey1))
    {
        OEmitParticles("test.pfx", Vector3{OScreenWf / 3, OScreenHf / 2, 0});
    }
    if (OInputJustPressed(OKey2))
    {
        if (emitter.isPlaying())
        {
            emitter.stop();
        }
        else
        {
            // Animate it back and forth across the screen
            position.play(Vector3{OScreenWf - 100, OScreenHf / 2, 0}, Vector3{100, OScreenHf / 2, 0}, 2.f, OTweenLinear, OPingPongLoop);

            emitter = OEmitParticles("test2.pex", position);
            emitter.setRenderEnabled(false);
        }
    }
}

void render()
{
    // Clear to black
    oRenderer->clear({0, 0, 0, 1});

    oSpriteBatch->begin();

    // Draw info
    auto pFont = OGetFont("font.fnt");
    pFont->draw("Press ^9901^999 to spawn particles from an onut PFX file", {10, 10});
    pFont->draw("Press ^9902^999 to spawn particles from PEX file", {10, 30});

    // It is possible to manually call render an emitter,
    // so we can specify in which order it is renderer manually
    emitter.render();

    oSpriteBatch->end();
}

void postRender()
{
}
