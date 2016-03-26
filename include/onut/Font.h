#pragma once
// Onut
#include <onut/Maths.h>
#include <onut/Resource.h>

// STL
#include <unordered_map>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ContentManager)
OForwardDeclare(Font)
OForwardDeclare(SpriteBatch)
OForwardDeclare(Texture)

namespace onut
{
    class SpriteBatch;

    class Font final : public Resource
    {
    public:
        static OFontRef createFromFile(const std::string& filename, const OContentManagerRef& pContentManager);

        ~Font();

        Vector2 measure(const std::string& text);
        size_t caretPos(const std::string& text, float at);

        Rect draw(const std::string& text, 
                  const Vector2& pos, 
                  const Vector2& align = Vector2(0.f, 0.f), 
                  const Color& color = Color::White, 
                  bool snapPixels = true,
                  const OSpriteBatchRef& pSpriteBatch = nullptr);
        Rect drawOutlined(const std::string& text,
                          const Vector2& pos,
                          const Vector2& align = Vector2(0.f, 0.f),
                          const Color& color = Color::White,
                          const Color& outlineColor = Color(0, 0, 0, .75f),
                          float outlineSize = 2.f,
                          bool cheap = true,
                          bool snapPixels = true,
                          const OSpriteBatchRef& pSpriteBatch = nullptr);

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
            int id = 0;
            std::string file;
            OTextureRef pTexture;
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

        static int parseInt(const std::string& arg, const std::vector<std::string>& lineSplit);
        static std::string parseString(const std::string& arg, const std::vector<std::string>& lineSplit);

        fntCommon m_common;
        fntPage** m_pages = nullptr;
        int m_charsCount = 0;
        std::unordered_map<int, fntChar*> m_chars;
    };
}

OFontRef OGetFont(const std::string& name);
