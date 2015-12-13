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
    OSettings->setGameName("Animations Sample");
    ORun(init, update, render);
}

OAnimf angleAnim1;
OAnimf angleAnim2;
OAnim2 bounceAnim;
OAnim4 frameAnim;
OAnim2 callbackAnim;

void playBounceSound()
{
    OPlaySound("bounce.wav", .5f);
}

void init()
{
    // Loop
    angleAnim1.start(
        0.f, // From
        360.f, // To
        2.f, // Duration in Seconds
        OLinear, // Tween
        OLoop // Loop
        );

    // Pingpong loop
    angleAnim2.start(
        0.f, // From
        720.f, // To
        2.f, // Duration in Seconds
        OEaseBoth, // Tween
        OPingPongLoop // Loop
        );

    // Bounce
    bounceAnim.start(
        Vector2(0.f, 0.f), // From
        Vector2(0.f, 200.f), // To
        2.f, // Duration in Seconds
        OBounceOut, // Tween
        OLoop // Loop
        );

    // Keyframes
    frameAnim.startKeyframed(
        Rect(32.f, 192.f, 128.f, 128.f), // From
        {   
            // Keyframes
            {Rect(32.f, 192.f, 128.f, 400.f), 2.f, OEaseBoth},
            {Rect(32.f, 192.f, 192.f, 400.f), 1.5f, OBounceOut},
            {Rect(32.f, 192.f, 128.f, 128.f), 1.5f, OBounceOut},
        }, 
        OLoop // Loop
        );

    // Callbacks
    callbackAnim.startKeyframed(
        Vector2(0, 0),
        {
            {Vector2(256, 0), 1.5f, OEaseIn, playBounceSound},
            {Vector2(0, 0), 1.5f, OEaseIn, playBounceSound},
        },
        OLoop);
}

void update()
{
}

void render()
{
    auto pNutTexture = OGetTexture("onutLogo.png");
    auto pFrameTexture = OGetTexture("frameSmall.png");

    // Clear
    ORenderer->clear(OColorHex(1d232d));

    // Draw sprites
    OSpriteBatch->begin();
    OSpriteBatch->drawSprite(pNutTexture, Vector2(128, 128), Color::White, angleAnim1);
    OSpriteBatch->drawSprite(pNutTexture, Vector2(256, 128), Color::White, angleAnim2);
    OSpriteBatch->drawSprite(pNutTexture, Vector2(384, 128) + bounceAnim, Color::White);
    OSpriteBatch->drawRectScaled9RepeatCenters(pFrameTexture, frameAnim, Vector4(48, 48, 48, 48));
    OSpriteBatch->drawSprite(pNutTexture, Vector2(512, 256) + callbackAnim);
    OSpriteBatch->end();
}
