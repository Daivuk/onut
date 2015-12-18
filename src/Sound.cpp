#include "Audio.h"
#include "onut.h"
#include "tinyxml2.h"
using namespace DirectX;

extern AudioEngine* g_pAudioEngine;

namespace onut
{
    Sound* Sound::createFromFile(const std::string& filename)
    {
        auto pRet = new Sound();
        if (g_pAudioEngine)
        {
            pRet->m_pSound = new SoundEffect(g_pAudioEngine, utf8ToUtf16(filename).c_str());
        }
        return pRet;
    }

    Sound::~Sound()
    {
        if (m_pSound)
        {
            delete m_pSound;
            m_pSound = nullptr;
        }
    }

    void Sound::play(float volume, float balance, float pitch)
    {
        if (m_pSound)
        {
            std::shared_ptr<DirectX::SoundEffectInstance> pInst = nullptr;
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
                    if (static_cast<int>(m_instances.size()) >= m_maxInstance) return;
                }
                std::shared_ptr<DirectX::SoundEffectInstance> inst = m_pSound->CreateInstance();
                m_instances.push_back(inst);
                pInst = inst;
            }
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
    {}

    SoundInstance* Sound::createInstance() const
    {
        auto pInstance = new SoundInstance();
        pInstance->m_pSound = m_pSound;
        pInstance->m_instance = m_pSound->CreateInstance();
        pInstance->setVolume(1.f);
        pInstance->setBalance(0.f);
        pInstance->setPitch(1.f);
        return pInstance;
    }

    void SoundInstance::setLoop(bool bLoop)
    {
        m_isLooping = bLoop;
    }

    void SoundInstance::setVolume(float volume)
    {
        m_instance->SetVolume(volume);
    }

    void SoundInstance::setBalance(float balance)
    {
        if (m_pSound->GetFormat()->nChannels == 1)
        {
            m_instance->SetPan(balance);
        }
    }

    void SoundInstance::setPitch(float pitch)
    {
        if (pitch < (1.f / 1024.0f)) pitch = (1.f / 1024.0f);
        m_instance->SetPitch(pitch);
    }

    void SoundInstance::play()
    {
        if (m_isPaused)
        {
            m_instance->Resume();
        }
        else
        {
            m_instance->Play(m_isLooping);
        }
    }

    void SoundInstance::pause()
    {
        m_isPaused = true;
        m_instance->Pause();
    }

    void SoundInstance::stop()
    {
        m_isPaused = false;
        m_instance->Stop();
    }

    bool SoundInstance::isPlaying() const
    {
        return m_instance->GetState() == DirectX::SoundState::PLAYING;
    }

    SoundCue* SoundCue::createFromFile(const std::string& filename, 
                                       const std::function<Sound*(const char*)>& getSound)
    {
        auto pSoundCue = new SoundCue();

        tinyxml2::XMLDocument doc;
        doc.LoadFile(filename.c_str());
        assert(!doc.Error());
        auto pXmlCue = doc.FirstChildElement("cue");
        assert(pXmlCue);

        for (auto pXmlPlay = pXmlCue->FirstChildElement("play"); pXmlPlay; pXmlPlay = pXmlPlay->NextSiblingElement("play"))
        {
            sPlay play;
            for (auto pXmlSound = pXmlPlay->FirstChildElement("sound"); pXmlSound; pXmlSound = pXmlSound->NextSiblingElement("sound"))
            {
                sSound sound;
                sound.pSound = getSound(pXmlSound->Attribute("name"));
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
