var font = getFont("font.fnt");
var particleSystem1 = getParticleSystem("test.pfx");
var particleSystem2 = getParticleSystem("test2.pex");

var emitter = new ParticleEmitter();
var position = new Vector3Anim();

function update()
{
    var screen = Renderer.getResolution();

    emitter.setTransform(position.get());

    if (Input.isJustDown(Key._1))
    {
        emitParticles(particleSystem1, new Vector3(screen.x / 3, screen.y / 2, 0));
    }
    if (Input.isJustDown(Key._2))
    {
        if (emitter.isPlaying())
        {
            emitter.stop();
        }
        else
        {
            // Animate it back and forth across the screen
            position.playSingle(new Vector3(screen.x - 100, screen.y / 2, 0), new Vector3(100, screen.y / 2, 0), 2, Tween.LINEAR, Loop.PING_PONG_LOOP);

            emitter = emitParticles(particleSystem2, position.get());
            emitter.setRenderEnabled(false);
        }
    }
}

function render()
{
    // Clear to black
    Renderer.clear(Color.BLACK);

    SpriteBatch.begin();

    // Draw info
    SpriteBatch.drawText(font, "Press ^9901^999 to spawn particles from an onut PFX file", new Vector2(10, 10));
    SpriteBatch.drawText(font, "Press ^9902^999 to spawn particles from PEX file", new Vector2(10, 30));

    // It is possible to manually call render an emitter,
    // so we can specify in which order it is renderer manually
    emitter.render();

    SpriteBatch.drawText(font, "FPS: " + Timing.getFPS(), new Vector2(10, 50));

    SpriteBatch.end();
}
