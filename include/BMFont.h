#pragma once
#include <unordered_map>
#include <vector>
#include "SimpleMath.h"
#include "Typedefs.h"
using namespace DirectX::SimpleMath;

namespace onut {
    class Texture;
    class SpriteBatch;

    class BMFont {
    public:
        static BMFont* createFromFile(const std::string& filename);

        BMFont();
        virtual ~BMFont();

        Vector2 measure(const std::string& text);
        template<Align Talign = Align::TOP_LEFT>
        Rect draw(const std::string& text,
                  const Vector2& pos,
                  const Color& color = Color::White,
                  onut::SpriteBatch* pSpriteBatch = OSB,
                  Align align = Talign) {
            return drawInternal(text, pos, color, pSpriteBatch, align);
        }
        template<
            Align Talign = Align::TOP_LEFT,
            bool Tcheap = true>
        Rect drawOutlined(const std::string& text,
            Vector2 pos,
            const Color& color = Color::White,
            Color outlineColor = { 0, 0, 0, .75f},
            float outlineSize = 2.f,
            onut::SpriteBatch* pSpriteBatch = OSB,
            Align align = Talign) {
            outlineColor.w *= color.w;
            pos = { std::round(pos.x), std::round(pos.y) };
            if (Tcheap) {
                drawInternal(text, pos + Vector2{ -outlineSize *0.86602540378443864676372317075294f, -outlineSize *0.5f }, outlineColor, pSpriteBatch, align, false);
                drawInternal(text, pos + Vector2{ outlineSize * 0.86602540378443864676372317075294f, -outlineSize *0.5f }, outlineColor, pSpriteBatch, align, false);
                drawInternal(text, pos + Vector2{ 0, outlineSize }, outlineColor, pSpriteBatch, align, false);
            }
            else {
                drawInternal(text, pos + Vector2{ -outlineSize *0.5f, -outlineSize *0.86602540378443864676372317075294f }, outlineColor, pSpriteBatch, align, false);
                drawInternal(text, pos + Vector2{ outlineSize * 0.5f, -outlineSize *0.86602540378443864676372317075294f }, outlineColor, pSpriteBatch, align, false);
                drawInternal(text, pos + Vector2{ -outlineSize, 0 }, outlineColor, pSpriteBatch, align, false);
                drawInternal(text, pos + Vector2{ outlineSize, 0 }, outlineColor, pSpriteBatch, align, false);
                drawInternal(text, pos + Vector2{ -outlineSize *0.5f, outlineSize *0.86602540378443864676372317075294f }, outlineColor, pSpriteBatch, align, false);
                drawInternal(text, pos + Vector2{ outlineSize * 0.5f, outlineSize *0.86602540378443864676372317075294f }, outlineColor, pSpriteBatch, align, false);
            }
            return drawInternal(text, pos, color, pSpriteBatch, align, false);
        }

    private:
        struct fntCommon {
            int lineHeight = 0;
            int base = 0;
            int scaleW = 0;
            int scaleH = 0;
            int pages = 0;
            int packed = 0;
        };
        struct fntPage {
            int         id = 0;
            std::string file;
            Texture*    pTexture = nullptr;
        };
        struct fntChars {
            int count = 0;
        };
        struct fntChar {
            int id = 0;
            int x = 0;
            int y = 0;
            int width = 0;
            int height = 0;
            int xoffset = 0;
            int yoffset = 0;
            int xadvance = 0;
            int page = 0;
            int chnl = 0;
            int displayList = 0;
        };

        static int                parseInt(const std::string& arg, const std::vector<std::string>& lineSplit);
        static std::string        parseString(const std::string& arg, const std::vector<std::string>& lineSplit);

        Rect drawInternal(
            const std::string& text,
            const Vector2& pos,
            const Color& color,
            onut::SpriteBatch* pSpriteBatch,
            Align align,
            bool snapPixels = true);

        fntCommon                           m_common;
        fntPage**                           m_pages = nullptr;
        int                                 m_charsCount = 0;
        std::unordered_map<int, fntChar*>   m_chars;
    };
}

typedef onut::BMFont OFont;
