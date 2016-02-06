#pragma once
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "SimpleMath.h"
#include "Texture.h"
#include "Typedefs.h"
using namespace DirectX::SimpleMath;

namespace onut
{
    class Texture;
    class SpriteBatch;
}
extern onut::SpriteBatch* OSpriteBatch;

namespace onut
{
    class BMFont
    {
    public:
        template<typename TcontentManagerType>
        static BMFont* createFromFile(const std::string& filename, TcontentManagerType* pContentManager)
        {
            return BMFont::createFromFile(filename, [pContentManager](const char* pFilename)
            {
                return pContentManager->getResource<Texture>(pFilename);
            });
        }
        static BMFont* createFromFile(const std::string& filename, std::function<Texture*(const char*)> loadTextureFn);

        BMFont();
        virtual ~BMFont();

        Vector2 measure(const std::string& text);
        decltype(std::string().size()) caretPos(const std::string& text, float at);

        template<Align Talign = Align::TOP_LEFT>
        Rect draw(const std::string& text,
                  const Vector2& pos,
                  const Color& color = Color::White,
                  onut::SpriteBatch* pSpriteBatch = OSpriteBatch,
                  Align align = Talign)
        {
            Vector2 alignf;
            switch (align)
            {
                case Align::TOP_LEFT:
                    alignf = {0, 0};
                    break;
                case Align::TOP:
                    alignf = {.5f, 0};
                    break;
                case Align::TOP_RIGHT:
                    alignf = {1, 0};
                    break;
                case Align::LEFT:
                    alignf = {0, .5f};
                    break;
                case Align::CENTER:
                    alignf = {.5f, .5f};
                    break;
                case Align::RIGHT:
                    alignf = {1, .5f};
                    break;
                case Align::BOTTOM_LEFT:
                    alignf = {0, 1};
                    break;
                case Align::BOTTOM:
                    alignf = {.5f, 1};
                    break;
                case Align::BOTTOM_RIGHT:
                    alignf = {1, 1};
                    break;
            }
            return drawInternal(text, pos, color, pSpriteBatch, alignf);
        }

        Rect draw(const std::string& text,
                  const Vector2& pos,
                  const Color& color = Color::White,
                  onut::SpriteBatch* pSpriteBatch = OSpriteBatch,
                  const Vector2& align = Vector2::Zero)
        {
            return drawInternal(text, pos, color, pSpriteBatch, align);
        }

        template<Align Talign = Align::TOP_LEFT,
            bool Tcheap = true>
            Rect drawOutlined(const std::string& text,
            Vector2 pos,
            const Color& color = Color::White,
            Color outlineColor = {0, 0, 0, .75f},
            float outlineSize = 2.f,
            onut::SpriteBatch* pSpriteBatch = OSpriteBatch,
            Align align = Talign)
        {
            Vector2 alignf;
            switch (align)
            {
                case Align::TOP_LEFT:
                    alignf = {0, 0};
                    break;
                case Align::TOP:
                    alignf = {.5f, 0};
                    break;
                case Align::TOP_RIGHT:
                    alignf = {1, 0};
                    break;
                case Align::LEFT:
                    alignf = {0, .5f};
                    break;
                case Align::CENTER:
                    alignf = {.5f, .5f};
                    break;
                case Align::RIGHT:
                    alignf = {1, .5f};
                    break;
                case Align::BOTTOM_LEFT:
                    alignf = {0, 1};
                    break;
                case Align::BOTTOM:
                    alignf = {.5f, 1};
                    break;
                case Align::BOTTOM_RIGHT:
                    alignf = {1, 1};
                    break;
            }
            outlineColor.w *= color.w;
            pos = {std::round(pos.x), std::round(pos.y)};
            if (Tcheap)
            {
                drawInternal(text, pos + Vector2{-outlineSize *0.86602540378443864676372317075294f, -outlineSize *0.5f}, outlineColor, pSpriteBatch, alignf, false);
                drawInternal(text, pos + Vector2{outlineSize * 0.86602540378443864676372317075294f, -outlineSize *0.5f}, outlineColor, pSpriteBatch, alignf, false);
                drawInternal(text, pos + Vector2{0, outlineSize}, outlineColor, pSpriteBatch, alignf, false);
            }
            else
            {
                drawInternal(text, pos + Vector2{-outlineSize *0.5f, -outlineSize *0.86602540378443864676372317075294f}, outlineColor, pSpriteBatch, alignf, false);
                drawInternal(text, pos + Vector2{outlineSize * 0.5f, -outlineSize *0.86602540378443864676372317075294f}, outlineColor, pSpriteBatch, alignf, false);
                drawInternal(text, pos + Vector2{-outlineSize, 0}, outlineColor, pSpriteBatch, alignf, false);
                drawInternal(text, pos + Vector2{outlineSize, 0}, outlineColor, pSpriteBatch, alignf, false);
                drawInternal(text, pos + Vector2{-outlineSize *0.5f, outlineSize *0.86602540378443864676372317075294f}, outlineColor, pSpriteBatch, alignf, false);
                drawInternal(text, pos + Vector2{outlineSize * 0.5f, outlineSize *0.86602540378443864676372317075294f}, outlineColor, pSpriteBatch, alignf, false);
            }
            return drawInternal(text, pos, color, pSpriteBatch, alignf, false);
        }

        const std::string& getName() const { return m_name; }

    private:
        struct fntCommon
        {
            int lineHeight = 0;
            int base = 0;
            int scaleW = 0;
            int scaleH = 0;
            int pages = 0;
            int packed = 0;
        };
        struct fntPage
        {
            int         id = 0;
            std::string file;
            Texture*    pTexture = nullptr;
        };
        struct fntChars
        {
            int count = 0;
        };
        struct fntChar
        {
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

        Rect drawInternal(const std::string& text,
                          const Vector2& pos,
                          const Color& color,
                          onut::SpriteBatch* pSpriteBatch,
                          const Vector2& align,
                          bool snapPixels = true);

        fntCommon                           m_common;
        fntPage**                           m_pages = nullptr;
        int                                 m_charsCount = 0;
        std::unordered_map<int, fntChar*>   m_chars;
        std::string                         m_name;
    };
}

typedef onut::BMFont OFont;
