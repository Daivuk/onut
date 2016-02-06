#include "SpriteAnim.h"
#include "tinyxml2.h"

#include <cassert>

namespace onut
{
    SpriteAnimDefinition* SpriteAnimDefinition::createFromFile(const std::string& filename, onut::ContentManager* pContentManager)
    {
        SpriteAnimDefinition* pRet = new SpriteAnimDefinition();

        pRet->m_filename = pContentManager->find(filename);
        if (pRet->m_filename.empty())
        {
            pRet->m_filename = filename;
        }

        tinyxml2::XMLDocument doc;
        doc.LoadFile(pRet->m_filename.c_str());
        auto pXMLSheet = doc.FirstChildElement("sheet");
        assert(pXMLSheet);
        std::string textureName = pXMLSheet->Attribute("texture");
        onut::Texture* pTexture = pContentManager->getResource<onut::Texture>(textureName);
        assert(pTexture);
        int spriteW = pTexture->getSize().x;
        int spriteH = pTexture->getSize().y;
        int originX = 0;
        int originY = 0;
        pXMLSheet->QueryAttribute("spriteWidth", &spriteW);
        pXMLSheet->QueryAttribute("spriteHeight", &spriteH);
        pXMLSheet->QueryAttribute("originX", &originX);
        pXMLSheet->QueryAttribute("originY", &originY);
        Vector2 origin((float)originX / (float)spriteW, (float)originY / (float)spriteH);
        for (auto pXMLAnim = pXMLSheet->FirstChildElement("anim"); pXMLAnim; pXMLAnim = pXMLAnim->NextSiblingElement("anim"))
        {
            std::string name = pXMLAnim->Attribute("name");
            auto& anim = pRet->m_anims[name];
            int fps = 30;
            pXMLAnim->QueryAttribute("fps", &fps);
            pXMLAnim->QueryAttribute("loop", &anim.loop);
            Frame frame;
            frame.pTexture = pTexture;
            frame.origin = origin;
            for (auto pXMLFrame = pXMLAnim->FirstChildElement("frame"); pXMLFrame; pXMLFrame = pXMLFrame->NextSiblingElement("frame"))
            {
                int repeat = 1;
                int id = 0;
                pXMLFrame->QueryAttribute("id", &id);
                pXMLFrame->QueryAttribute("repeat", &repeat);
                int col = id % (pTexture->getSize().x / spriteW);
                int row = id / (pTexture->getSize().x / spriteW);
                col *= spriteW;
                row *= spriteH;
                frame.UVs = Vector4(
                    (float)col / (float)pTexture->getSize().x,
                    (float)row / (float)pTexture->getSize().y,
                    (float)(col + spriteW) / (float)pTexture->getSize().x,
                    (float)(row + spriteH) / (float)pTexture->getSize().y);
                for (auto i = 0; i < repeat; ++i)
                {
                    anim.frames.push_back(frame);
                }
            }
            anim.frames.push_back(frame); // Repeat last frame
            anim.duration = (float)anim.frames.size() / (float)fps;
        }

        return pRet;
    }

    void SpriteAnimDefinition::addAnim(const Anim& anim)
    {
        m_anims[anim.name] = anim;
    }

    SpriteAnimDefinition::Anim* SpriteAnimDefinition::getAnim(const std::string& name)
    {
        auto it = m_anims.find(name);
        if (it == m_anims.end()) return nullptr;
        return &it->second;
    }

    std::vector<std::string> SpriteAnimDefinition::getAnimNames() const
    {
        std::vector<std::string> anims;
        for (auto& kv : m_anims)
        {
            anims.push_back(kv.first);
        }
        return std::move(anims);
    }

    SpriteAnimDefinition::SpriteAnimDefinition()
    {
    }

    SpriteAnimDefinition::~SpriteAnimDefinition()
    {
    }
}

namespace onut
{
    SpriteAnim::SpriteAnim()
    {
    }

    SpriteAnim::SpriteAnim(SpriteAnimDefinition* pDefinition)
    {
        m_pDefinition = pDefinition;
    }

    SpriteAnim::SpriteAnim(const std::string& definitionFilename, ContentManager* pContentManager)
    {
        m_pDefinition = pContentManager->getResource<SpriteAnimDefinition>(definitionFilename);
    }

    void SpriteAnim::start(const std::string& animName)
    {
        stop();
        if (m_pDefinition)
        {
            m_pCurrentAnim = m_pDefinition->getAnim(animName);
            if (m_pCurrentAnim)
            {
                m_frame.start(0.f, (float)m_pCurrentAnim->frames.size() - 1, m_pCurrentAnim->duration, onut::TweenType::LINEAR,
                              m_pCurrentAnim->loop ? onut::LoopType::LOOP : onut::LoopType::NONE);
            }
        }
    }

    void SpriteAnim::startBackward(const std::string& animName)
    {
        stop();
        if (m_pDefinition)
        {
            m_pCurrentAnim = m_pDefinition->getAnim(animName);
            if (m_pCurrentAnim)
            {
                m_frame.start((float)m_pCurrentAnim->frames.size() - 1, 0.f, m_pCurrentAnim->duration, onut::TweenType::LINEAR,
                              m_pCurrentAnim->loop ? onut::LoopType::LOOP : onut::LoopType::NONE);
            }
        }
    }

    void SpriteAnim::stop(bool reset)
    {
        m_frame.stop();
        if (reset) m_frame = 0.f;
        m_pCurrentAnim = nullptr;
    }

    bool SpriteAnim::isPlaying() const
    {
        return m_frame.isPlaying();
    }

    int SpriteAnim::getFrameId() const
    {
        if (m_pCurrentAnim)
        {
            return (int)m_frame.get();
        }
        return 0;
    }

    onut::Texture* SpriteAnim::getTexture() const
    {
        if (m_pCurrentAnim)
        {
            return m_pCurrentAnim->frames[getFrameId()].pTexture;
        }
        return nullptr;
    }

    const Vector4& SpriteAnim::getUVs() const
    {
        if (m_pCurrentAnim)
        {
            return m_pCurrentAnim->frames[getFrameId()].UVs;
        }
        static Vector4 ret(0, 0, 1, 1);
        return ret;
    }

    const Vector2& SpriteAnim::getOrigin() const
    {
        if (m_pCurrentAnim)
        {
            return m_pCurrentAnim->frames[getFrameId()].origin;
        }
        static Vector2 ret(.5f, .5f);
        return ret;
    }
}
