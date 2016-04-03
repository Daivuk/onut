// Onut
#include <onut/ContentManager.h>
#include <onut/SpriteAnim.h>
#include <onut/Texture.h>
#include <onut/Timing.h>

// Third party
#include <tinyxml2/tinyxml2.h>

// STL
#include <cassert>

namespace onut
{
    OSpriteAnimRef SpriteAnim::createFromFile(const std::string& filename, const OContentManagerRef& in_pContentManager)
    {
        auto pContentManager = in_pContentManager;
        if (!pContentManager) pContentManager = oContentManager;

        auto pRet = std::make_shared<SpriteAnim>();

        pRet->m_filename = pContentManager->findResourceFile(filename);
        if (pRet->m_filename.empty())
        {
            pRet->m_filename = filename;
        }

        tinyxml2::XMLDocument doc;
        doc.LoadFile(pRet->m_filename.c_str());
        auto pXMLSheet = doc.FirstChildElement("sheet");
        assert(pXMLSheet);
        std::string textureName = pXMLSheet->Attribute("texture");
        auto pTexture = pContentManager->getResourceAs<OTexture>(textureName);
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
            anim.name = name;
            if (pXMLAnim->Attribute("next"))
            {
                anim.next = pXMLAnim->Attribute("next");
            }
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

    void SpriteAnim::addAnim(const Anim& anim)
    {
        m_anims[anim.name] = anim;
    }

    SpriteAnim::Anim* SpriteAnim::getAnim(const std::string& name)
    {
        auto it = m_anims.find(name);
        if (it == m_anims.end()) return nullptr;
        return &it->second;
    }

    std::vector<std::string> SpriteAnim::getAnimNames() const
    {
        std::vector<std::string> anims;
        for (auto& kv : m_anims)
        {
            anims.push_back(kv.first);
        }
        return std::move(anims);
    }
}

namespace onut
{
    SpriteAnimInstance::SpriteAnimInstance()
    {
    }

    SpriteAnimInstance::SpriteAnimInstance(const OSpriteAnimRef& pSpriteAnim)
        : m_pSpriteAnim(pSpriteAnim)
    {
    }

    SpriteAnimInstance::SpriteAnimInstance(const std::string& filename, const OContentManagerRef& in_pContentManager)
    {
        auto pContentManager = in_pContentManager;
        if (!pContentManager) pContentManager = oContentManager;
        m_pSpriteAnim = pContentManager->getResourceAs<SpriteAnim>(filename);
    }

    void SpriteAnimInstance::play(const std::string& animName, float framePerSecond)
    {
        stop();
        if (m_pSpriteAnim)
        {
            m_pCurrentAnim = m_pSpriteAnim->getAnim(animName);
            if (m_pCurrentAnim)
            {
                if (!m_pCurrentAnim->next.empty())
                {
                    m_animQueue.insert(m_animQueue.begin(), m_pCurrentAnim->next);
                }
                m_frame = 0.f;
                if (framePerSecond == 0)
                {
                    m_speed = (static_cast<float>(m_pCurrentAnim->frames.size()) - 1.f) / m_pCurrentAnim->duration;
                }
                else
                {
                    m_speed = framePerSecond;
                }

                oUpdater->registerTarget(this);
            }
        }
    }

    void SpriteAnimInstance::playBackward(const std::string& animName, float framePerSecond)
    {
        stop();
        if (m_pSpriteAnim)
        {
            m_pCurrentAnim = m_pSpriteAnim->getAnim(animName);
            if (m_pCurrentAnim)
            {
                m_frame = static_cast<float>(m_pCurrentAnim->frames.size()) - 1.f;
                if (framePerSecond == 0)
                {
                    m_speed = -m_frame / m_pCurrentAnim->duration;
                }
                else
                {
                    m_speed = -framePerSecond;
                }

                oUpdater->registerTarget(this);
            }
        }
    }

    void SpriteAnimInstance::queueAnim(const std::string& animName)
    {
        m_animQueue.push_back(animName);
    }

    void SpriteAnimInstance::playNextQueuedAnim()
    {
        if (!m_animQueue.empty())
        {
            auto animName = m_animQueue.front();
            m_animQueue.erase(m_animQueue.begin());
            if (m_pSpriteAnim)
            {
                m_pCurrentAnim = m_pSpriteAnim->getAnim(animName);
                if (m_pCurrentAnim)
                {
                    if (!m_pCurrentAnim->next.empty())
                    {
                        m_animQueue.insert(m_animQueue.begin(), m_pCurrentAnim->next);
                    }
                    m_frame = 0.f;
                    m_speed = (static_cast<float>(m_pCurrentAnim->frames.size()) - 1.f) / m_pCurrentAnim->duration;

                    oUpdater->registerTarget(this);
                }
            }
        }
    }

    void SpriteAnimInstance::stop(bool reset)
    {
        m_animQueue.clear();
        oUpdater->unregisterTarget(this);
        if (reset) m_frame = 0.f;
        m_pCurrentAnim = nullptr;
    }

    bool SpriteAnimInstance::isPlaying() const
    {
        return isUpdateTargetRegistered();
    }

    int SpriteAnimInstance::getFrameId() const
    {
        if (m_pCurrentAnim)
        {
            return (int)m_frame;
        }
        return 0;
    }

    OTextureRef SpriteAnimInstance::getTexture() const
    {
        if (m_pCurrentAnim)
        {
            return m_pCurrentAnim->frames[getFrameId()].pTexture;
        }
        return nullptr;
    }

    const Vector4& SpriteAnimInstance::getUVs() const
    {
        if (m_pCurrentAnim)
        {
            return m_pCurrentAnim->frames[getFrameId()].UVs;
        }
        static Vector4 ret(0, 0, 1, 1);
        return ret;
    }

    const Vector2& SpriteAnimInstance::getOrigin() const
    {
        if (m_pCurrentAnim)
        {
            return m_pCurrentAnim->frames[getFrameId()].origin;
        }
        static Vector2 ret(.5f, .5f);
        return ret;
    }

    void SpriteAnimInstance::update()
    {
        m_frame += ODT * m_speed;
        if (m_speed > 0.f)
        {
            if (m_frame >= static_cast<float>(m_pCurrentAnim->frames.size()) - 1.f)
            {
                if (m_pCurrentAnim->loop)
                {
                    m_frame -= static_cast<float>(m_pCurrentAnim->frames.size()) - 1.f;
                }
                else
                {
                    m_frame = static_cast<float>(m_pCurrentAnim->frames.size()) - 1.f;
                    oUpdater->unregisterTarget(this);
                    playNextQueuedAnim();
                }
            }
        }
        else if (m_frame <= 0.f)
        {
            if (m_pCurrentAnim->loop)
            {
                m_frame += static_cast<float>(m_pCurrentAnim->frames.size());
            }
            else
            {
                m_frame = 0.f;
                oUpdater->unregisterTarget(this);
                playNextQueuedAnim();
            }
        }
    }
}

OSpriteAnimRef OGetSpriteAnim(const std::string& name)
{
    return oContentManager->getResourceAs<OSpriteAnim>(name);
}

OSpriteAnimInstanceRef OPlaySpriteAnim(const std::string& filename, const std::string& animName)
{
    auto pSpriteAnim = OGetSpriteAnim(filename);

    auto pRet = std::make_shared<OSpriteAnimInstance>(pSpriteAnim);
    pRet->play(animName);
    return pRet;
}
