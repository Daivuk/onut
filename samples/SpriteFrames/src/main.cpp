// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include "onut_old.h"

void init();
void update();
void render();

OSpriteAnim spriteAnim;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    OSettings->setGameName("Sprite Animation Sample");
    ORun(init, update, render);
}

void init()
{
    spriteAnim = OSpriteAnim("baltAnims.xml");
}

void update()
{
    if (OJustPressed(OINPUT_1))
    {
        spriteAnim.start("idle");
    }
    if (OJustPressed(OINPUT_2))
    {
        spriteAnim.start("run");
    }
    if (OJustPressed(OINPUT_3))
    {
        spriteAnim.start("attack");
    }
}

void render()
{
    auto pBaltPng = OGetTexture("balt.png");
    auto pFont = OGetFont("font.fnt");

    // Clear
    ORenderer->clear(OColorHex(1d232d));

    // Draw sprites
    OSpriteBatch->changeFiltering(onut::SpriteBatch::eFiltering::Nearest);
    OSpriteBatch->begin(Matrix::CreateScale(2.f));
    OSpriteBatch->drawSprite(pBaltPng, Vector2(4, 8), Color::White, Vector2::Zero);

    // Sprite the animation
    OSpriteBatch->drawSpriteWithUVs(spriteAnim.getTexture(), Vector2(256, 64), spriteAnim.getUVs(), Color::White, 0.f, 2.f, spriteAnim.getOrigin());

    // Draw the origin
    OSpriteBatch->drawCross(Vector2(256, 64), 5.f, Color(1, 1, 0, 1));
    OSpriteBatch->end();

    OSpriteBatch->begin();
    pFont->draw("Sheet", Vector2(8, 8), OLeft);
    pFont->draw("Press ^9901^999 to play ^991idle^999 anim", Vector2(8, 300), OLeft);
    pFont->draw("Press ^9902^999 to play ^991run^999 anim", Vector2(8, 320), OLeft);
    pFont->draw("Press ^9903^999 to play ^991attack^999 anim", Vector2(8, 340), OLeft);
    OSpriteBatch->end();
}
