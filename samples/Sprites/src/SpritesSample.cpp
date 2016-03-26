// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/Anim.h>
#include <onut/Input.h>
#include <onut/Maths.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>

void init();
void update();
void render();

float g_spriteAngle = 0.f;
OAnimMatrix batchTransform;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Sprites Sample");
    ORun(init, update, render);
}

void init()
{
    batchTransform = Matrix::Identity;
}

void update()
{
    g_spriteAngle += ODT * 45.f;

    if (OInputJustPressed(OKeySpaceBar))
    {
        batchTransform.playKeyFrames(
            Matrix::Identity,
            {
                {
                    Matrix::CreateTranslation(-OScreenCenterXf, -OScreenCenterYf, 0) *
                    Matrix::CreateRotationZ(DirectX::XMConvertToRadians(45.f)) *
                    Matrix::CreateTranslation(OScreenCenterXf, OScreenCenterYf, 0),
                    1.0f,
                    OTweenLinear
                },
                {
                    Matrix::CreateTranslation(-OScreenCenterXf, -OScreenCenterYf, 0) *
                    Matrix::CreateRotationZ(DirectX::XMConvertToRadians(-45.f)) *
                    Matrix::CreateTranslation(OScreenCenterXf, OScreenCenterYf, 0),
                    2.0f,
                    OTweenLinear
                },
                {
                    Matrix::Identity,
                    1.f,
                    OTweenLinear
                }
            });
    }
}

void render()
{
    auto pNutTexture = OGetTexture("onutLogo.png");
    auto pFrameTexture = OGetTexture("frameSmall.png");
    auto pChainTexture = OGetTexture("chain.png");

    // Clear
    oRenderer->clear(OColorHex(1d232d));

    // Begin a batch
    oSpriteBatch->begin(batchTransform);

    // Opaque
    oSpriteBatch->drawRect(nullptr, Rect(0, 0, 64, 64));
    oSpriteBatch->drawRect(nullptr, Rect(64, 0, 64, 64), Color(1, 0, 0, 1)); // Red
    oSpriteBatch->drawInclinedRect(nullptr, Rect(192, 0, 64, 64), -.5f); // Inclined

    // Textured
    oSpriteBatch->drawRect(pNutTexture, Rect(0, 64, 64, 64));
    oSpriteBatch->drawRect(pNutTexture, Rect(64, 64, 64, 64), Color(1, 0, 0, 1)); // Tinted

    // Per vertex coloring
    oSpriteBatch->drawRectWithColors(nullptr, Rect(0, 128, 64, 64), {Color(1, 0, 0, 1), Color(1, 1, 0, 1), Color(0, 1, 0, 1), Color(0, 0, 1, 1)});

    // Custom UVs
    oSpriteBatch->drawRectWithUVs(pNutTexture, Rect(64, 128, 64, 64), Vector4(0, 0, .5f, .5f));

    // Split the texture in 4 corner (Good for corsairs)
    oSpriteBatch->draw4Corner(pNutTexture, Rect(128, 64, 160, 160));

    // Scale 9, 9 Patch, 9 slice (whatever you guys call it)
    oSpriteBatch->drawRectScaled9(pFrameTexture, Rect(0, 256, 160, 160), Vector4(48, 48, 48, 48));
    oSpriteBatch->drawRectScaled9(pFrameTexture, Rect(160, 256, 256, 160), Vector4(48, 48, 48, 48));
    oSpriteBatch->drawRectScaled9RepeatCenters(pFrameTexture, Rect(416, 256, 256, 160), Vector4(48, 48, 48, 48));

    // Sprites
    oSpriteBatch->drawSprite(pNutTexture, Vector2(64, 480));
    oSpriteBatch->drawSprite(pNutTexture, Vector2(192, 480), Color::White, g_spriteAngle);
    oSpriteBatch->drawSprite(pNutTexture, Vector2(320, 440), Color::White, g_spriteAngle, .5f);
    oSpriteBatch->drawSprite(nullptr, Vector2(320, 500), Color::White, g_spriteAngle, 32.f);

    // With a custom matrix
    Matrix customTransform = Matrix::Identity;
    customTransform *= Matrix::CreateRotationZ(DirectX::XMConvertToRadians(g_spriteAngle));
    customTransform *= Matrix::CreateScale(4.f, 0.5f, 1.f);
    customTransform *= Matrix::CreateTranslation(520, 120, 0);
    oSpriteBatch->drawSprite(pNutTexture, customTransform, Color::White);

    // Lines
    oSpriteBatch->drawBeam(pChainTexture, Vector2(448, 480), Vector2(648, 440), 32.0f, Color::White);
    oSpriteBatch->drawBeam(pChainTexture, Vector2(448, 520), Vector2(648, 480), 32.0f, Color::White, g_spriteAngle * .1f);

    // Blend modes
    oSpriteBatch->changeBlendMode(OBlendAdd);
    oSpriteBatch->drawSprite(pNutTexture, Vector2(400, 50), Color::White, 0, .25f);
    oSpriteBatch->changeBlendMode(OBlendAlpha);
    oSpriteBatch->drawSprite(pNutTexture, Vector2(450, 50), Color::White, 0, .25f);
    oSpriteBatch->changeBlendMode(OBlendForceWrite);
    oSpriteBatch->drawSprite(pNutTexture, Vector2(500, 50), Color::White, 0, .25f);
    oSpriteBatch->changeBlendMode(OBlendMultiply);
    oSpriteBatch->drawSprite(pNutTexture, Vector2(550, 50), Color::White, 0, .25f);
    oSpriteBatch->changeBlendMode(OBlendOpaque);
    oSpriteBatch->drawSprite(pNutTexture, Vector2(600, 50), Color::White, 0, .25f);
    oSpriteBatch->changeBlendMode(OBlendPreMultiplied);
    oSpriteBatch->drawSprite(pNutTexture, Vector2(650, 50), Color::White, 0, .25f);

    // Origin
    oSpriteBatch->drawSprite(pNutTexture, Vector2(400, 200), Color::White, 0, .25f, Vector2(0, 0));
    oSpriteBatch->drawCross(Vector2(400, 200), 10.f, Color(1, 1, 0, 1));
    oSpriteBatch->drawSprite(pNutTexture, Vector2(500, 200), Color::White, 0, .25f, Vector2(.5f, .5f));
    oSpriteBatch->drawCross(Vector2(500, 200), 10.f, Color(1, 1, 0, 1));
    oSpriteBatch->drawSprite(pNutTexture, Vector2(600, 200), Color::White, 0, .25f, Vector2(1, 1));
    oSpriteBatch->drawCross(Vector2(600, 200), 10.f, Color(1, 1, 0, 1));

    // Filtering
    oSpriteBatch->changeFiltering(OFilterNearest);
    oSpriteBatch->drawSprite(pNutTexture, Vector2(725, 100), Color::White, 0.f, 1.5f);
    oSpriteBatch->changeFiltering(OFilterLinear);
    oSpriteBatch->drawSprite(pNutTexture, Vector2(725, 300), Color::White, 0.f, 1.5f);

    // End and flush the batch
    oSpriteBatch->end();
}
