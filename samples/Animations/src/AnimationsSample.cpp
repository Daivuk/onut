// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/Anim.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Sound.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>

void init();
void update();
void render();

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Animations Sample");
    ORun(init, update, render);
}

OAnimFloat angleAnim1;
OAnimFloat angleAnim2;
OAnimVector2 bounceAnim;
OAnimRect frameAnim;
OAnimVector2 callbackAnim;

void playBounceSound()
{
    OPlaySound("bounce.wav", .5f);
}

void init()
{
    // Loop
    angleAnim1.play(
        0.f, // From
        360.f, // To
        2.f, // Duration in Seconds
        OTweenLinear, // Tween
        OLoop // Loop
        );

    // Pingpong loop
    angleAnim2.play(
        0.f, // From
        720.f, // To
        2.f, // Duration in Seconds
        OTweenEaseBoth, // Tween
        OPingPongLoop // Loop
        );
    
    // Bounce
    bounceAnim.play(
        Vector2(0.f, 0.f), // From
        Vector2(0.f, 200.f), // To
        2.f, // Duration in Seconds
        OTweenBounceOut, // Tween
        OLoop // Loop
        );
    
    // Keyframes
    frameAnim.playKeyFrames(
        Rect(32.f, 192.f, 128.f, 128.f), // From
        {   
            // Keyframes
            {Rect(32.f, 192.f, 128.f, 400.f), 2.f, OTweenEaseBoth},
            {Rect(32.f, 192.f, 192.f, 400.f), 1.5f, OTweenBounceOut},
            {Rect(32.f, 192.f, 128.f, 128.f), 1.5f, OTweenBounceOut},
        }, 
        OLoop // Loop
        );
    
    // Queuing + Callbacks
    callbackAnim = Vector2::Zero;
    callbackAnim.queue(Vector2(256, 0), 1.5f, OTweenEaseIn, playBounceSound);
    callbackAnim.queue(Vector2::Zero, 1.5f, OTweenEaseIn, playBounceSound);
    callbackAnim.play(OLoop);
}

void update()
{
}

void render()
{
    auto pNutTexture = OGetTexture("onutLogo.png");
    auto pFrameTexture = OGetTexture("frameSmall.png");

    // Clear
    oRenderer->clear(OColorHex(1d232d));

    // Draw sprites
    oSpriteBatch->begin();
    oSpriteBatch->drawSprite(pNutTexture, Vector2(128, 128), Color::White, angleAnim1);
    oSpriteBatch->drawSprite(pNutTexture, Vector2(256, 128), Color::White, angleAnim2);
    oSpriteBatch->drawSprite(pNutTexture, Vector2(384, 128) + bounceAnim, Color::White);
    oSpriteBatch->drawRectScaled9RepeatCenters(pFrameTexture, frameAnim, Vector4(48, 48, 48, 48));
    oSpriteBatch->drawSprite(pNutTexture, Vector2(512, 256) + callbackAnim);
    oSpriteBatch->end();
}
