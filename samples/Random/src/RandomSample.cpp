// Oak Nut include
#include <onut/PrimitiveBatch.h>
#include <onut/Random.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>

void initSettings()
{
    oSettings->setGameName("Random Sample");
    oSettings->setIsRetroMode(true); // So we can see the points bigger
    oSettings->setRetroResolution({200, 200});
}

void init()
{
}

void update()
{
}

void render()
{
    oRenderer->clear({0, 0, 0, 1});

    oPrimitiveBatch->begin(OPrimitivePointList);

    if (ORandBool()) oPrimitiveBatch->draw(Vector2(10, 25), Color(0, 1, 0, 1)); // 50%
    if (ORandBool(.25f)) oPrimitiveBatch->draw(Vector2(20, 25), Color(0, 1, 0, 1)); // 25%

    for (int i = 0; i < 100; ++i)
    {
        oPrimitiveBatch->draw(Vector2(ORandInt(10) * 10, 5), Color(1, 1, 0, 1));
        oPrimitiveBatch->draw(Vector2(ORandInt(5, 10) * 10, 10), Color(1, 1, 0, 1));
        oPrimitiveBatch->draw(Vector2(ORandInt(100), 15), Color(0, 1, 1, 1));
        oPrimitiveBatch->draw(Vector2(ORandInt(50, 100), 20), Color(0, 1, 1, 1));
        oPrimitiveBatch->draw(ORandVector2(Vector2(25, 25)) + Vector2(0, 30), Color(1, 0, 1, 1));
        oPrimitiveBatch->draw(ORandVector2(Vector2(10, 10), Vector2(25, 25)) + Vector2(30, 30), Color(1, 0, 1, 1));
        oPrimitiveBatch->draw(onut::randCircle(Vector2(50, 100), 25), Color(1, 1, 1, 1));
        oPrimitiveBatch->draw(onut::randCircleEdge(Vector2(150, 100), 25), Color(1, 1, 1, 1));
    }

    oPrimitiveBatch->end();

    oSpriteBatch->begin();

    oSpriteBatch->drawRect(nullptr, Rect(0, 150, 20, 12), ORandColor());
    oSpriteBatch->drawRect(nullptr, Rect(21, 150, 20, 12), ORandColor(.5f));
    oSpriteBatch->drawRect(nullptr, Rect(42, 150, 20, 12), ORandColor(Color(1, 0, 1)));
    oSpriteBatch->drawRect(nullptr, Rect(63, 150, 20, 12), ORandColor(Color(0, 1, 0), Color(1, 0, 0)));
    oSpriteBatch->drawRect(nullptr, Rect(63, 150, 20, 12), ORandColor(Color(0, 1, 0), Color(1, 0, 0)));

    onut::Palette palette = {
        OColorHex(43A08E),
        OColorHex(B3E0CB),
        OColorHex(EE5351),
        OColorHex(D19A8E),
        OColorHex(DBC9A3)
    };
    float x = 0;
    for (auto c : palette)
    {
        oSpriteBatch->drawRect(nullptr, Rect(x, 170, 20, 12), c);
        x += 21;
    }
    oSpriteBatch->drawRect(nullptr, Rect(0, 183, 20, 12), ORandColor(palette));
    oSpriteBatch->drawRect(nullptr, Rect(21, 183, 20, 12), ORandColor(palette, .5f));

    oSpriteBatch->end();
}

void postRender()
{
}

void renderUI()
{
}
