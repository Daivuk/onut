// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/Font.h>
#include <onut/Input.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteAnim.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>

void init();
void update();
void render();

OSpriteAnimInstance spriteAnim;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Sprite Animation Sample");
    ORun(init, update, render);
}

void init()
{
    spriteAnim = OSpriteAnimInstance("baltAnims.spriteanim");
}

void update()
{
    if (OInputJustPressed(OKey1))
    {
        spriteAnim.play("idle");
    }
    if (OInputJustPressed(OKey2))
    {
        spriteAnim.play("run");
    }
    if (OInputJustPressed(OKey3))
    {
        spriteAnim.play("run", 20);
    }
    if (OInputJustPressed(OKey4))
    {
        spriteAnim.play("attack");
    }
}

void render()
{
    auto pBaltPng = OGetTexture("balt.png");
    auto pFont = OGetFont("font.fnt");

    // Clear
    oRenderer->clear(OColorHex(1d232d));

    // Draw sprites
    oSpriteBatch->changeFiltering(OFilterNearest);
    oSpriteBatch->begin(Matrix::CreateScale(2.f));
    oSpriteBatch->drawSprite(pBaltPng, Vector2(4, 8), Color::White, Vector2::Zero);

    // Sprite the animation
    oSpriteBatch->drawSpriteWithUVs(spriteAnim.getTexture(), Vector2(256, 64), spriteAnim.getUVs(), Color::White, 0.f, 2.f, spriteAnim.getOrigin());

    // Draw the origin
    oSpriteBatch->drawCross(Vector2(256, 64), 5.f, Color(1, 1, 0, 1));
    oSpriteBatch->end();

    oSpriteBatch->begin();
    pFont->draw("Sheet", Vector2(8, 8), OLeft);
    pFont->draw("Press ^9901^999 to play ^991idle^999 anim", Vector2(8, 300), OLeft);
    pFont->draw("Press ^9902^999 to play ^991run^999 anim", Vector2(8, 320), OLeft);
    pFont->draw("Press ^9903^999 to play ^991run fast^999 anim", Vector2(8, 340), OLeft);
    pFont->draw("Press ^9904^999 to play ^991attack^999 anim", Vector2(8, 360), OLeft);
    oSpriteBatch->end();
}
