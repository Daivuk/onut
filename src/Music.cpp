#include "Music.h"
#include "Utils.h"
#include "Mp3.h"
#include "onut.h"

#include <cassert>
#include <Windows.h>

namespace onut
{
    Music::Music()
    {
        m_pMp3 = new Mp3();
    }

    Music::~Music()
    {
        delete m_pMp3;
    }

    void Music::play(const std::string& filename)
    {
        m_pMp3->Stop();
        m_pMp3->Cleanup();

        auto cmd = utf8ToUtf16(OContentManager->find(filename));
        if (!m_pMp3->Load(cmd.c_str()))
        {
            return;
        }
        setVolume(m_volume);
        if (!m_pMp3->Play())
        {
            return;
        }

        m_isPlaying = true;
    }

    void Music::setVolume(float volume)
    {
        float decibelDenominator = 4000.f;
        if (volume < 0.1f)
            decibelDenominator = 10000.f;
        m_volume = volume;
        m_pMp3->SetVolume(static_cast<long>((m_volume - 1.f) * decibelDenominator));
    }

    void Music::stop()
    {
        m_isPlaying = false;
        m_pMp3->Stop();
    }

    void Music::pause()
    {
        if (m_isPlaying)
        {
            m_isPlaying = false;
            m_pMp3->Pause();
        }
    }

    void Music::resume()
    {
        if (!m_isPlaying)
        {
            m_isPlaying = true;
            m_pMp3->Play();
        }
    }

    bool Music::isDone()
    {
        if (m_isPlaying)
        {
            long evCode;
            bool donePlaying = m_pMp3->WaitForCompletion(0, &evCode);
            if (donePlaying)
            {
                m_isPlaying = false;
            }
            return donePlaying;
        }
        return false;
    }
}
