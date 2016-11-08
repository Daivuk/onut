// Onut
#include <onut/ContentManager.h>
#include <onut/Font.h>
#include <onut/SpriteBatch.h>
#include <onut/Strings.h>
#include <onut/Texture.h>

// STL
#include <cassert>
#include <sstream>
#include <fstream>

namespace onut
{
    int Font::parseInt(const std::string& arg, const std::vector<std::string>& lineSplit)
    {
        std::stringstream ss;
        ss << Font::parseString(arg, lineSplit);
        int theInt = 0;
        if (!(ss >> theInt)) return 0;
        return theInt;
    }

    std::string Font::parseString(const std::string& arg, const std::vector<std::string>& lineSplit)
    {
        for (unsigned int i = 1; i < lineSplit.size(); ++i)
        {
            std::vector<std::string> argSplit = splitString(lineSplit[i], '=');
            if (argSplit[0] == arg)
            {
                std::string result = argSplit[1];
                if (result[0] == '\"') result = result.substr(1, result.size() - 2);
                return result;
            }
        }
        return "";
    }

    OFontRef Font::createFromFile(const std::string& filename, const OContentManagerRef& pContentManager)
    {
        std::ifstream in(filename);
        assert(!in.fail());

        auto pFont = std::make_shared<OFont>();

        std::string line;
        std::getline(in, line);
        std::vector<std::string> split;
        while (!in.eof())
        {
            split = splitString(line, ' ');
            std::string command = split[0];
            if (command == "info") {}
            else if (command == "common")
            {
                pFont->m_common.lineHeight = parseInt("lineHeight", split);
                pFont->m_common.base = parseInt("base", split);
                pFont->m_common.scaleW = parseInt("scaleW", split);
                pFont->m_common.scaleH = parseInt("scaleH", split);
                pFont->m_common.pages = parseInt("pages", split);
                pFont->m_common.packed = parseInt("packed", split);

                pFont->m_pages = new fntPage*[pFont->m_common.pages];
            }
            else if (command == "page")
            {
                auto pNewPage = new fntPage();

                pNewPage->id = parseInt("id", split);
                pNewPage->file = parseString("file", split);

                pFont->m_pages[pNewPage->id] = pNewPage;

                // Load its texture
                pNewPage->pTexture = pContentManager->getResourceAs<OTexture>(pNewPage->file);
            }
            else if (command == "chars")
            {
                pFont->m_charsCount = parseInt("count", split);
            }
            else if (command == "char")
            {
                fntChar* pNewChar = new fntChar();

                pNewChar->id = parseInt("id", split);
                pNewChar->x = parseInt("x", split);
                pNewChar->y = parseInt("y", split);
                pNewChar->width = parseInt("width", split);
                pNewChar->height = parseInt("height", split);
                pNewChar->xoffset = parseInt("xoffset", split);
                pNewChar->yoffset = parseInt("yoffset", split);
                pNewChar->xadvance = parseInt("xadvance", split);
                pNewChar->page = parseInt("page", split);
                pNewChar->chnl = parseInt("chnl", split);

                pFont->m_chars[pNewChar->id] = pNewChar;
            }

            getline(in, line);
        }
        in.close();

        return pFont;
    }

    Font::~Font()
    {
        for (int i = 0; i < m_common.pages; ++i)
        {
            delete m_pages[i];
        }
        delete[] m_pages;
        m_pages = nullptr;

        for (auto& kv : m_chars)
        {
            delete kv.second;
        }
    }

    Vector2 Font::measure(const std::string& in_text)
    {
        Vector2 result;

        result.y += (float)m_common.lineHeight;
        float curX = 0;
        unsigned int len = in_text.length();
        char charId;
        for (unsigned int i = 0; i < len; ++i)
        {
            charId = in_text[i];
            if (charId == '\n')
            {
                result.y += (float)m_common.lineHeight;
                if (curX > result.x) result.x = curX;
                curX = 0;
                continue;
            }
            if (charId == '^' && i + 3 < len)
            {
                i += 3;
                continue;
            }
            auto iCharId = static_cast<int>(static_cast<unsigned char>(charId));
            if (!m_chars.count(iCharId))
            {
                continue;
            }
            auto pDatChar = m_chars[iCharId];
            if (i == len - 1)
            {
                curX += static_cast<float>(pDatChar->xoffset) + static_cast<float>(pDatChar->width);
            }
            else if (in_text[i + 1] == '\n')
            {
                curX += static_cast<float>(pDatChar->xoffset) + static_cast<float>(pDatChar->width);
            }
            else
            {
                curX += static_cast<float>(pDatChar->xadvance);
            }
        }
        if (curX > result.x) result.x = curX;

        return result;
    }

    decltype(std::string().size()) Font::caretPos(const std::string& in_text, float at)
    {
        decltype(std::string().size()) pos = 0;

        float curX = 0;
        unsigned int len = in_text.length();
        int charId;
        for (; pos < len; ++pos)
        {
            charId = in_text[pos];
            if (charId == '\n')
            {
                return pos;
            }
            if (charId == '^' && pos + 3 < len)
            {
                pos += 3;
                continue;
            }
            const auto& it = m_chars.find(charId);
            if (it == m_chars.end()) continue;
            auto pDatChar = it->second;
            auto advance = static_cast<float>(pDatChar->xadvance);
            if (curX + advance * .75f >= at)
            {
                return pos;
            }
            curX += advance;
        }

        return pos;
    }

    Rect Font::drawOutlined(const std::string& text,
                            const Vector2& in_pos,
                            const Vector2& align,
                            const Color& color,
                            const Color& outlineColor,
                            float outlineSize,
                            bool cheap,
                            bool snapPixels,
                            const OSpriteBatchRef& pSpriteBatch)
    {
        Color outlineColorFinal = outlineColor;
        outlineColorFinal.a *= color.a;

        Vector2 pos = in_pos;
        if (snapPixels)
        {
            pos = {std::round(pos.x), std::round(pos.y)};
        }

        if (cheap)
        {
            draw(text, pos + Vector2{-outlineSize *0.86602540378443864676372317075294f, -outlineSize *0.5f}, align, outlineColorFinal, false, pSpriteBatch);
            draw(text, pos + Vector2{outlineSize * 0.86602540378443864676372317075294f, -outlineSize *0.5f}, align, outlineColorFinal, false, pSpriteBatch);
            draw(text, pos + Vector2{0, outlineSize}, align, outlineColorFinal, false, pSpriteBatch);
        }
        else
        {
            draw(text, pos + Vector2{-outlineSize *0.5f, -outlineSize *0.86602540378443864676372317075294f}, align, outlineColorFinal, false, pSpriteBatch);
            draw(text, pos + Vector2{outlineSize * 0.5f, -outlineSize *0.86602540378443864676372317075294f}, align, outlineColorFinal, false, pSpriteBatch);
            draw(text, pos + Vector2{-outlineSize, 0}, align, outlineColorFinal, false, pSpriteBatch);
            draw(text, pos + Vector2{outlineSize, 0}, align, outlineColorFinal, false, pSpriteBatch);
            draw(text, pos + Vector2{-outlineSize *0.5f, outlineSize *0.86602540378443864676372317075294f}, align, outlineColorFinal, false, pSpriteBatch);
            draw(text, pos + Vector2{outlineSize * 0.5f, outlineSize *0.86602540378443864676372317075294f}, align, outlineColorFinal, false, pSpriteBatch);
        }
        return draw(text, pos, align, color, false, pSpriteBatch);
    }

    Rect Font::draw(const std::string& text, const Vector2& in_pos, const Vector2& align, const Color& color, bool snapPixels, const OSpriteBatchRef& in_pSpriteBatch)
    {
        OSpriteBatchRef pSpriteBatch = in_pSpriteBatch;
        if (!pSpriteBatch) pSpriteBatch = oSpriteBatch;
        Vector2 pos = in_pos;
        Rect ret;
        Vector2 dim = measure(text);
        ret.z = dim.x;
        ret.w = dim.y;

        Vector2 posFrom = {pos.x, pos.y - (m_common.lineHeight - m_common.base)};
        Vector2 posTo = {pos.x - dim.x, pos.y - dim.y + (m_common.lineHeight - m_common.base)};

        pos.x = posFrom.x + (posTo.x - posFrom.x) * align.x;
        pos.y = posFrom.y + (posTo.y - posFrom.y) * align.y;

        if (snapPixels)
        {
            pos = {std::round(pos.x), std::round(pos.y)};
        }
        Vector2 curPos = pos;
        ret.x = curPos.x;
        ret.y = curPos.y;
        unsigned int len = text.length();
        int page = -1;
        float r, g, b;
        Color curColor = color;
        bool bHandleBatch = !pSpriteBatch->isInBatch();
        if (bHandleBatch) pSpriteBatch->begin();
        for (unsigned int i = 0; i < text.size(); )
        {
            char charId = text[i];
            if (charId == '\n')
            {
                curPos.x = pos.x;
                curPos.y += static_cast<float>(m_common.lineHeight);
                i += 1;
                continue;
            }
            if (charId == '^' && i + 3 < len)
            {
                // Colored text!
                r = (static_cast<float>(text[i + 1]) - static_cast<float>('0')) / 9.0f;
                g = (static_cast<float>(text[i + 2]) - static_cast<float>('0')) / 9.0f;
                b = (static_cast<float>(text[i + 3]) - static_cast<float>('0')) / 9.0f;
                curColor = {r, g, b, color.a};
                curColor.Premultiply();
                i += 4;
                continue;
            }
            auto iCharId = static_cast<int>(static_cast<unsigned char>(charId));
            if (!m_chars.count(iCharId))
            {
                ++i;
                continue;
            }
            auto pDatChar = m_chars[iCharId];
            auto& pTexture = m_pages[pDatChar->page]->pTexture;

            // Draw it here
            pSpriteBatch->drawRectWithUVs(
                pTexture, {
                    curPos.x + static_cast<float>(pDatChar->xoffset), curPos.y + static_cast<float>(pDatChar->yoffset),
                    static_cast<float>(pDatChar->width), static_cast<float>(pDatChar->height)
                }, {
                    static_cast<float>(pDatChar->x) / static_cast<float>(m_common.scaleW),
                    static_cast<float>(pDatChar->y) / static_cast<float>(m_common.scaleH),
                    static_cast<float>(pDatChar->x + pDatChar->width) / static_cast<float>(m_common.scaleW),
                    static_cast<float>(pDatChar->y + pDatChar->height) / static_cast<float>(m_common.scaleH)
                }, curColor);

            curPos.x += static_cast<float>(pDatChar->xadvance);
            ++i;
        }
        if (bHandleBatch) pSpriteBatch->end();

        return std::move(ret);
    }
}

OFontRef OGetFont(const std::string& name)
{
    return oContentManager->getResourceAs<OFont>(name);
}
