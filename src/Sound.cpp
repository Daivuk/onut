#include "Sound.h"
#include "Audio.h"
#include "StringUtils.h"
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

    void Sound::play(float volume, float balance)
    {
        if (m_pSound->GetFormat()->nChannels > 1)
        {
            m_pSound->Play();
            return;
        }
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
            pInst->SetPan(balance);
            pInst->Play();
        }
    }

    void Sound::stop()
    {}
}
