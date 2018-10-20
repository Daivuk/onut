// Onut
#include <onut/ContentManager.h>
#include <onut/Files.h>
#include <onut/Sound.h>
#include <onut/SpriteAnim.h>
#include <onut/Strings.h>
#include <onut/Texture.h>
#include <onut/Timing.h>

// Third party
#include <json/json.h>
#include <tinyxml2/tinyxml2.h>

// STL
#include <cassert>
#include <fstream>

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

        if (onut::getExtension(pRet->m_filename) == "JSON")
        {
            std::ifstream fic(pRet->m_filename);
            Json::Value json;
            fic >> json;
            fic.close();
            std::string textureName = onut::getFilename(json["meta"]["image"].asString());
            auto pTexture = pContentManager->getResourceAs<OTexture>(textureName);
            assert(pTexture);
            int spriteW = json["meta"]["size"]["w"].asInt();
            int spriteH = json["meta"]["size"]["h"].asInt();
            int originX = 0;
            int originY = 0;
            const auto& tags = json["meta"]["frameTags"];
            const auto& layers = json["meta"]["layers"];
            const auto& frames = json["frames"];
            std::map<int, std::vector<std::string>> frameDatas;
            for (const auto& layer : layers)
            {
                const auto& cels = layer["cels"];
                for (const auto& cel : cels)
                {
                    frameDatas[cel["frame"].asInt()].push_back(cel["data"].asString());
                }
            }
            for (const auto& tag : tags)
            {
                std::string name = tag["name"].asString();
                auto& anim = pRet->m_anims[name];
                anim.name = name;
                anim.duration = 0.0f;
                bool isFlipH = false;
                bool isFlipV = false;
                int from = tag["from"].asInt();
                int to = tag["to"].asInt();
                for (int i = from; i <= to; ++i)
                {
                    Frame frame;
                    frame.pTexture = pTexture;
                    const auto& jsonFrame = frames[i];
                    int offsetX = 0;
                    int offsetY = 0;
                    spriteW = jsonFrame["sourceSize"]["w"].asInt();
                    spriteH = jsonFrame["sourceSize"]["h"].asInt();
                    const auto& frameData = frameDatas[i];
                    for (const auto& fdata : frameData)
                    {
                        auto splits = onut::splitString(fdata, ":");
                        for (const auto& data : splits)
                        {
                            if (data.find("loop") == 0)
                            {
                                anim.loop = true;
                            }
                            else if (data.find("genfliph") == 0)
                            {
                                isFlipH = true;
                            }
                            else if (data.find("genflipv") == 0)
                            {
                                isFlipV = true;
                            }
                            else if (data.find("next=") == 0 && data.size() > 5)
                            {
                                anim.next = data.substr(5);
                            }
                            else if (data.find("origin=") == 0 && data.size() > 9)
                            {
                                auto pos = data.find(",");
                                if (pos != std::string::npos && pos + 1 < data.size())
                                {
                                    try
                                    {
                                        originX = std::stoi(data.substr(7, pos - 7));
                                    }
                                    catch (...) {}
                                    try
                                    {
                                        originY = std::stoi(data.substr(pos + 1));
                                    }
                                    catch (...) {}
                                }
                            }
                            else if (data.find("sound=") == 0 && data.size() > 6)
                            {
                                frame.pSound = OGetSound(data.substr(6));
                            }
                            else if (data.find("soundcue=") == 0 && data.size() > 9)
                            {
                                frame.pSoundCue = OGetSoundCue(data.substr(9));
                            }
                            else if (data.find("vol=") == 0 && data.size() > 4)
                            {
                                try
                                {
                                    frame.vol = std::stof(data.substr(4));
                                }
                                catch (...) {}
                            }
                            else if (data.find("pan=") == 0 && data.size() > 4)
                            {
                                try
                                {
                                    frame.pan = std::stof(data.substr(4));
                                }
                                catch (...) {}
                            }
                            else if (data.find("pitch=") == 0 && data.size() > 6)
                            {
                                try
                                {
                                    frame.pitch = std::stof(data.substr(6));
                                }
                                catch (...) {}
                            }
                        }
                    }
                    frame.origin = Vector2((float)originX / (float)spriteW, (float)originY / (float)spriteH);
                    const auto& jsonRect = jsonFrame["frame"];
                    int jsonX = jsonRect["x"].asInt();
                    int jsonY = jsonRect["y"].asInt();
                    int jsonW = jsonRect["w"].asInt();
                    int jsonH = jsonRect["h"].asInt();
                    frame.UVs = Vector4(
                        (float)jsonX / (float)pTexture->getSize().x,
                        (float)jsonY / (float)pTexture->getSize().y,
                        (float)(jsonX + jsonW) / (float)pTexture->getSize().x,
                        (float)(jsonY + jsonH) / (float)pTexture->getSize().y);
                    frame.origin.x -= (float)offsetX / (float)spriteW;
                    frame.origin.y -= (float)offsetY / (float)spriteH;
                    frame.duration = (float)jsonFrame["duration"].asInt() / 1000.0f;
                    anim.duration += frame.duration;
                    anim.frames.push_back(frame);
                }

                if (isFlipH)
                {
                    name = anim.name + "_fliph";
                    auto& flippedAnim = pRet->m_anims[name];
                    flippedAnim = anim;
                    flippedAnim.name = name;
                    for (auto& frame : flippedAnim.frames)
                    {
                        std::swap(frame.UVs.x, frame.UVs.z);
                    }
                }
                if (isFlipV)
                {
                    name = anim.name + "_flipv";
                    auto& flippedAnim = pRet->m_anims[name];
                    flippedAnim = anim;
                    flippedAnim.name = name;
                    for (auto& frame : flippedAnim.frames)
                    {
                        std::swap(frame.UVs.x, frame.UVs.z);
                    }
                }
            }
        }
        else
        {
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
                anim.duration = 0.0f;
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
                    bool isFlipH = false;
                    bool isFlipV = false;
                    int offsetX = 0;
                    int offsetY = 0;
                    pXMLFrame->QueryAttribute("id", &id);
                    pXMLFrame->QueryAttribute("repeat", &repeat);
                    pXMLFrame->QueryAttribute("flipH", &isFlipH);
                    pXMLFrame->QueryAttribute("flipV", &isFlipV);
                    pXMLFrame->QueryAttribute("offsetX", &offsetX);
                    pXMLFrame->QueryAttribute("offsetY", &offsetY);
                    int col = id % (pTexture->getSize().x / spriteW);
                    int row = id / (pTexture->getSize().x / spriteW);
                    col *= spriteW;
                    row *= spriteH;
                    frame.UVs = Vector4(
                        (float)col / (float)pTexture->getSize().x,
                        (float)row / (float)pTexture->getSize().y,
                        (float)(col + spriteW) / (float)pTexture->getSize().x,
                        (float)(row + spriteH) / (float)pTexture->getSize().y);
                    frame.origin.x -= (float)offsetX / (float)spriteW;
                    frame.origin.y -= (float)offsetY / (float)spriteH;
                    if (isFlipH) std::swap(frame.UVs.x, frame.UVs.z);
                    if (isFlipV) std::swap(frame.UVs.y, frame.UVs.w);
                    frame.duration = (1.0f / (float)fps) * (float)repeat;
                    anim.duration += frame.duration;
                    anim.frames.push_back(frame);
                }
                //anim.frames.push_back(frame); // Repeat last frame
                //anim.duration = (float)anim.frames.size() / (float)fps;
            }

            pRet->m_size.x = spriteW;
            pRet->m_size.y = spriteH;
        }

        // Slightly adjust UVs to avoid bleeding
        //for (auto& kv : pRet->m_anims)
        //{
        //    for (auto& frame : kv.second.frames)
        //    {
        //        auto tSize = frame.pTexture->getSizef();
        //        frame.UVs.x += 0.2f / tSize.x;
        //        frame.UVs.y += 0.2f / tSize.y;
        //        frame.UVs.z -= 0.2f / tSize.x;
        //        frame.UVs.w -= 0.2f / tSize.y;
        //    }
        //}

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
        m_pUpdater = oUpdater;
    }

    SpriteAnimInstance::SpriteAnimInstance(const OSpriteAnimRef& pSpriteAnim)
        : m_pSpriteAnim(pSpriteAnim)
    {
        m_pUpdater = oUpdater;
    }

    SpriteAnimInstance::SpriteAnimInstance(const std::string& filename, const OContentManagerRef& in_pContentManager)
    {
        m_pUpdater = oUpdater;
        auto pContentManager = in_pContentManager;
        if (!pContentManager) pContentManager = oContentManager;
        m_pSpriteAnim = pContentManager->getResourceAs<SpriteAnim>(filename);
    }

    void SpriteAnimInstance::setUpdater(const OUpdaterRef& pUpdater)
    {
        if (isPlaying())
        {
            if (pUpdater)
            {
                pUpdater->registerTarget(this);
            }
            else if (m_pUpdater)
            {
                m_pUpdater->unregisterTarget(this);
                m_pUpdater = nullptr;
            }
        }
        else
        {
            m_pUpdater = pUpdater;
        }
    }

    void SpriteAnimInstance::setSpeed(float framePerSecond)
    {
        m_speed = framePerSecond;
    }

    void SpriteAnimInstance::play(const std::string& animName, float framePerSecond, float offset)
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
                m_frameTime = offset;
                m_frameId = 0;
                while (m_frameTime > getFrame().duration)
                {
                    m_frameTime -= getFrame().duration;
                    m_frameId = std::min<int>(m_frameId + 1, (int)m_pCurrentAnim->frames.size() - 1);
                }

                const auto& frame = getFrame();
                if (frame.pSound) frame.pSound->play(frame.vol, frame.pan, frame.pitch);
                if (frame.pSoundCue) frame.pSoundCue->play(frame.vol, frame.pan, frame.pitch);

                if (framePerSecond == 0)
                {
                    m_speed = 1.0f;
                }
                else
                {
                    m_speed = framePerSecond;
                }

                m_pUpdater->registerTarget(this);
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
                m_frameId = (int)m_pCurrentAnim->frames.size() - 1;
                m_frameTime = getFrame().duration;
                if (framePerSecond == 0)
                {
                    m_speed = -1.0f;
                }
                else
                {
                    m_speed = -framePerSecond;
                }

                m_pUpdater->registerTarget(this);
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
                    m_frameTime = 0.f;
                    m_frameId = 0;
                    m_speed = 1.0f;

                    m_pUpdater->registerTarget(this);
                }
            }
        }
    }

    void SpriteAnimInstance::stop(bool reset)
    {
        m_animQueue.clear();
        m_pUpdater->unregisterTarget(this);
        if (reset)
        {
            m_frameTime = 0.f;
            m_frameId = 0;
        }
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
            return m_frameId;
        }
        return 0;
    }

    OTextureRef SpriteAnimInstance::getTexture() const
    {
        auto frameId = getFrameId();
        if (m_pCurrentAnim && frameId >= 0 && frameId < (int)m_pCurrentAnim->frames.size())
        {
            return m_pCurrentAnim->frames[getFrameId()].pTexture;
        }
        return nullptr;
    }

    const Vector4& SpriteAnimInstance::getUVs() const
    {
        auto frameId = getFrameId();
        if (m_pCurrentAnim && frameId >= 0 && frameId < (int)m_pCurrentAnim->frames.size())
        {
            return m_pCurrentAnim->frames[getFrameId()].UVs;
        }
        static Vector4 ret(0, 0, 1, 1);
        return ret;
    }

    const Vector2& SpriteAnimInstance::getOrigin() const
    {
        auto frameId = getFrameId();
        if (m_pCurrentAnim && frameId >= 0 && frameId < (int)m_pCurrentAnim->frames.size())
        {
            return m_pCurrentAnim->frames[getFrameId()].origin;
        }
        static Vector2 ret(.5f, .5f);
        return ret;
    }

    const SpriteAnim::Frame& SpriteAnimInstance::getFrame() const
    {
        auto frameId = getFrameId();
        if (frameId >= (int)m_pCurrentAnim->frames.size()) return m_pCurrentAnim->frames.back();
        else if (frameId < 0) return m_pCurrentAnim->frames.front();
        else return m_pCurrentAnim->frames[getFrameId()];
    }

    void SpriteAnimInstance::update()
    {
        if (!m_pCurrentAnim) return;
        auto prevFrameId = m_frameId;
        m_frameTime += ODT * m_speed;
        if (m_speed > 0.f)
        {
            const auto& frame = getFrame();
            if (m_frameTime >= frame.duration)
            {
                m_frameTime -= frame.duration;
                ++m_frameId;
                if (m_frameId == (int)m_pCurrentAnim->frames.size())
                {
                    if (m_pCurrentAnim->loop)
                    {
                        m_frameId = 0;
                    }
                    else
                    {
                        m_frameTime = frame.duration;
                        m_frameId = (int)m_pCurrentAnim->frames.size() - 1;
                        m_pUpdater->unregisterTarget(this);
                        playNextQueuedAnim();
                    }
                }
            }
        }
        else if (m_frameTime < 0.f)
        {
            --m_frameId;
            if (m_frameId < 0)
            {
                if (m_pCurrentAnim->loop)
                {
                    m_frameId = (int)m_pCurrentAnim->frames.size() - 1;
                    const auto& frame = getFrame();
                    m_frameTime += frame.duration;
                }
                else
                {
                    m_frameTime = 0.f;
                    m_frameId = 0;
                    m_pUpdater->unregisterTarget(this);
                    playNextQueuedAnim();
                }
            }
            else
            {
                m_frameTime += getFrame().duration;
            }
        }

        if (prevFrameId != m_frameId)
        {
            const auto& frame = getFrame();
            if (frame.pSound) frame.pSound->play(frame.vol, frame.pan, frame.pitch);
            if (frame.pSoundCue) frame.pSoundCue->play(frame.vol, frame.pan, frame.pitch);
        }
    }
}

OSpriteAnimRef OGetSpriteAnim(const std::string& name)
{
    return oContentManager->getResourceAs<OSpriteAnim>(name);
}

OSpriteAnimInstanceRef OPlaySpriteAnim(const std::string& filename, const std::string& animName, float offset)
{
    auto pSpriteAnim = OGetSpriteAnim(filename);

    auto pRet = std::make_shared<OSpriteAnimInstance>(pSpriteAnim);
    pRet->play(animName, 0.0f, offset);
    return pRet;
}
