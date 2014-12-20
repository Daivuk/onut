#pragma once
#include "SimpleMath.h"
#include "Typedefs.h"
#include <memory>
#include <vector>
#include <unordered_map>
using namespace DirectX::SimpleMath;

namespace onut {
    class Texture;
    class SpriteBatch;
    extern std::shared_ptr<SpriteBatch> getSpriteBatch();

    class BMFont {
    public:
        static std::shared_ptr<BMFont> createFromFile(const std::string& filename);

        BMFont();
        virtual ~BMFont();

        Vector2        measure(const std::string& text);
        template<Align Talign = Align::TOP_LEFT>
        Rect        draw(    const std::string& text,
                            const Vector2& pos,
                            const Color& color = Color::White,
                            std::shared_ptr<onut::SpriteBatch> pSpriteBatch = getSpriteBatch(),
                            Align align = Talign) {
            return drawInternal(text, pos, color, pSpriteBatch, align);
        }

    private:
        Rect        drawInternal(
            const std::string& text, 
            const Vector2& pos, 
            const Color& color, 
            std::shared_ptr<onut::SpriteBatch> pSpriteBatch,
            Align align);

        struct fntCommon {
            int lineHeight = 0;
            int base = 0;
            int scaleW = 0;
            int scaleH = 0;
            int pages = 0;
            int packed = 0;
        };
        struct fntPage {
            int                            id = 0;
            std::string                    file;
            std::shared_ptr<Texture>    pTexture;
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

        fntCommon                            m_common;
        fntPage**                            m_pages = nullptr;
        int                                    m_charsCount = 0;
        std::unordered_map<int, fntChar*>    m_chars;
    };
}
