// Onut
#include <onut/ContentManager.h>
#include <onut/Sound.h>
#include <onut/Strings.h>
#include <onut/Random.h>

// Third party
#include <audio/Audio.h>
#include <tinyxml2/tinyxml2.h>

using namespace DirectX;

extern AudioEngine* g_pAudioEngine;

namespace onut
{
    OSoundRef Sound::createFromFile(const std::string& filename, const OContentManagerRef& pContentManager)
    {
        if (g_pAudioEngine)
        {
            auto pRet = std::make_shared<OSound>();
            pRet->m_pSound = std::make_shared<SoundEffect>(g_pAudioEngine, utf8ToUtf16(filename).c_str());
            return pRet;
        }
        else
        {
            return nullptr;
        }
    }

    void Sound::play(float volume, float balance, float pitch)
    {
        if (m_pSound)
        {
            // Find a free instance
            DXSoundEffectInstanceRef pInst = nullptr;
            for (auto &inst : m_instances)
            {
                if (inst->GetState() == SoundState::STOPPED)
                {
                    pInst = inst;
                    break;
                }
            }
            if (!pInst)
            {
                if (m_maxInstance != -1)
                {
                    if (static_cast<int>(m_instances.size()) >= m_maxInstance)
                    {
                        return; // We have reach max instance count
                    }
                }
                DXSoundEffectInstanceRef inst = m_pSound->CreateInstance();
                m_instances.push_back(inst);
                pInst = inst;
            }

            // Play it
            pInst->SetVolume(volume);
            if (m_pSound->GetFormat()->nChannels == 1)
            {
                if (balance < -1) balance = -1;
                if (balance > 1) balance = 1;
                pInst->SetPan(balance);
            }
            if (pitch < (1.f / 1024.0f)) pitch = (1.f / 1024.0f);
            if (pitch > 2.f) pitch = 2.f;
            pInst->SetPitch(pitch);
            pInst->Play();
        }
    }

    void Sound::stop()
    {
        for (auto &inst : m_instances)
        {
            if (inst->GetState() != SoundState::STOPPED)
            {
                inst->Stop(true);
            }
        }
    }

    OSoundInstanceRef Sound::createInstance()
    {
        auto pInstance = std::make_shared<SoundInstance>();
        pInstance->m_pSound = m_pSound;
        pInstance->m_pInstance = m_pSound->CreateInstance();
        return pInstance;
    }

    void SoundInstance::setLoop(bool loop)
    {
        m_loop = loop;
    }

    void SoundInstance::setVolume(float volume)
    {
        m_volume = volume;
        m_pInstance->SetVolume(m_volume);
    }

    void SoundInstance::setBalance(float balance)
    {
        m_balance = balance;
        if (m_balance < -1) m_balance = -1;
        if (m_balance > 1) m_balance = 1;
        if (m_pSound->GetFormat()->nChannels == 1)
        {
            m_pInstance->SetPan(m_balance);
        }
    }

    void SoundInstance::setPitch(float pitch)
    {
        m_pitch = pitch;
        if (m_pitch < (1.f / 1024.0f)) m_pitch = (1.f / 1024.0f);
        if (m_pitch > 2.f) m_pitch = 2.f;
        m_pInstance->SetPitch(m_pitch);
    }

    void SoundInstance::play()
    {
        if (m_isPaused)
        {
            m_pInstance->Resume();
            m_isPaused = false;
        }
        else
        {
            m_pInstance->SetVolume(m_volume);
            if (m_pSound->GetFormat()->nChannels == 1)
            {
                m_pInstance->SetPan(m_balance);
            }
            m_pInstance->SetPitch(m_pitch);
            m_pInstance->Play(m_loop);
        }
    }

    void SoundInstance::pause()
    {
        m_isPaused = true;
        m_pInstance->Pause();
    }

    void SoundInstance::stop()
    {
        m_isPaused = false;
        m_pInstance->Stop();
    }

    bool SoundInstance::isPlaying() const
    {
        return m_pInstance->GetState() == DirectX::SoundState::PLAYING;
    }

    bool SoundInstance::isPaused() const
    {
        return m_isPaused;
    }

    bool SoundInstance::getLoop() const
    {
        return m_loop;
    }

    float SoundInstance::getVolume() const
    {
        return m_volume;
    }

    float SoundInstance::getBalance() const
    {
        return m_balance;
    }

    float SoundInstance::getPitch() const
    {
        return m_pitch;
    }

    OSoundCueRef SoundCue::createFromFile(const std::string& filename, const OContentManagerRef& in_pContentManager)
    {
        OContentManagerRef pContentManager = in_pContentManager;
        if (!pContentManager) pContentManager = oContentManager;
        auto pSoundCue = std::make_shared<OSoundCue>();

        tinyxml2::XMLDocument doc;
        doc.LoadFile(filename.c_str());
        assert(!doc.Error());
        auto pXmlCue = doc.FirstChildElement("cue");
        assert(pXmlCue);

        for (auto pXmlPlay = pXmlCue->FirstChildElement("play"); pXmlPlay; pXmlPlay = pXmlPlay->NextSiblingElement("play"))
        {
            Play play;
            for (auto pXmlSound = pXmlPlay->FirstChildElement("sound"); pXmlSound; pXmlSound = pXmlSound->NextSiblingElement("sound"))
            {
                Sound sound;
                sound.pSound = oContentManager->getResourceAs<OSound>(pXmlSound->Attribute("name"));
                pXmlSound->QueryFloatAttribute("balance", &sound.balance);
                pXmlSound->QueryFloatAttribute("volume", &sound.volume);
                pXmlSound->QueryFloatAttribute("pitch", &sound.pitch);
                float chance = 1;
                pXmlSound->QueryFloatAttribute("chance", &chance);
                play.totalChances += chance;
                sound.chance = play.totalChances;
                pXmlSound->QueryFloatAttribute("delay", &sound.delay);
                play.sounds.push_back(sound);

            }
            pSoundCue->m_plays.push_back(play);
        }

        return pSoundCue;
    }

    void SoundCue::play(float volume, float balance, float pitch)
    {
        for (auto& play : m_plays)
        {
            auto rnd = onut::randf(play.totalChances);
            for (auto& sound : play.sounds)
            {
                if (rnd <= sound.chance)
                {
                    if (sound.pSound)
                    {
                        if (sound.delay != 0)
                        {
                            play.playTimer.start(sound.delay, [=]
                            {
                                sound.pSound->play(sound.volume * volume,
                                                   sound.balance + balance,
                                                   sound.pitch * pitch);
                            });
                        }
                        else
                        {
                            sound.pSound->play(sound.volume * volume,
                                                sound.balance += balance,
                                                sound.pitch *= pitch);
                        }
                    }
                    break;
                }   
            }
        }
    }
}

OSoundRef OGetSound(const std::string& name)
{
    return oContentManager->getResourceAs<OSound>(name);
}

void OPlaySound(const std::string& name, float volume, float balance, float pitch)
{
    auto pSound = OGetSound(name);
    if (pSound)
    {
        pSound->play(volume, balance, pitch);
    }
}

void OPlayRandomSound(const std::vector<std::string>& sounds, float volume, float balance, float pitch)
{
    OPlaySound(ORandVector(sounds), volume, balance, pitch);
}

OSoundInstanceRef OCreateSoundInstance(const std::string& name)
{
    auto pSound = OGetSound(name);
    if (pSound)
    {
        return pSound->createInstance();
    }
    return nullptr;
}

OSoundCueRef OGetSoundCue(const std::string& name)
{
    return oContentManager->getResourceAs<OSoundCue>(name);
}

void OPlaySoundCue(const std::string& name, float volume, float balance, float pitch)
{
    auto pSoundCue = OGetSoundCue(name);
    if (pSoundCue)
    {
        pSoundCue->play(volume, balance, pitch);
    }
}
