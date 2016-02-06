#include "onut.h"
#include <sstream>
#include <fstream>

namespace onut
{
    int BMFont::parseInt(const std::string& arg, const std::vector<std::string>& lineSplit)
    {
        std::stringstream ss;
        ss << BMFont::parseString(arg, lineSplit);
        int theInt = 0;
        if (!(ss >> theInt)) return 0;
        return theInt;
    }

    std::string BMFont::parseString(const std::string& arg, const std::vector<std::string>& lineSplit)
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

    BMFont* BMFont::createFromFile(const std::string& filename, std::function<OTexture*(const char*)> loadTextureFn)
    {
        std::ifstream in(filename);
        assert(!in.fail());

        auto pFont = new BMFont();

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
                pNewPage->pTexture = loadTextureFn(pNewPage->file.c_str());
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

        pFont->m_name = filename.substr(filename.find_last_of("/\\") + 1);

        return pFont;
    }

    BMFont::BMFont()
    {}

    BMFont::~BMFont()
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

    Vector2 BMFont::measure(const std::string& in_text)
    {
        Vector2 result;

        result.y += (float)m_common.lineHeight;
        float curX = 0;
        unsigned int len = in_text.length();
        int charId;
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
            const auto& it = m_chars.find(charId);
            if (it == m_chars.end()) continue;
            auto pDatChar = it->second;
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

    decltype(std::string().size()) BMFont::caretPos(const std::string& in_text, float at)
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

    Rect BMFont::drawInternal(const std::string& text, const Vector2& in_pos, const Color& color, onut::SpriteBatch* pSpriteBatch, const Vector2& align, bool snapPixels)
    {
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
                curColor = {r, g, b, color.w};
                curColor.Premultiply();
                i += 4;
                continue;
            }
            if (!m_chars.count(charId))
            {
                ++i;
                continue;
            }
            auto pDatChar = m_chars[charId];
            auto pTexture = m_pages[pDatChar->page]->pTexture;

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
