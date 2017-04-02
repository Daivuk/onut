// Onut
#include <onut/ContentManager.h>
#include <onut/Strings.h>

// Third party
#include <mp3/Mp3.h>

// Internal
#include "MusicWin32.h"

// STL
#include <cassert>

namespace onut
{
    OMusicRef Music::createFromFile(const std::string& filename, const OContentManagerRef& pContentManager)
    {
        auto pRet = std::shared_ptr<MusicWin32>(new MusicWin32());

        auto cmd = utf8ToUtf16(filename);
        if (!pRet->m_pMp3->Load(cmd.c_str()))
        {
            return nullptr;
        }

        return pRet;
    }

    MusicWin32::MusicWin32()
    {
        m_pMp3 = OMake<Mp3>();
    }

    void MusicWin32::play(bool loop)
    {
        if (m_isPlaying) return;
        m_loop = loop;
        setVolume(m_volume);
        if (!m_pMp3->Play(m_loop))
        {
            return;
        }

        m_isPlaying = true;
    }

    void MusicWin32::setVolume(float volume)
    {
        float decibelDenominator = 4000.f;
        if (volume < 0.1f)
            decibelDenominator = 10000.f;
        m_volume = volume;
        m_pMp3->SetVolume(static_cast<long>((m_volume - 1.f) * decibelDenominator));
    }

    void MusicWin32::stop()
    {
        m_isPlaying = false;
        m_pMp3->Stop();
    }

    void MusicWin32::pause()
    {
        if (m_isPlaying)
        {
            m_isPlaying = false;
            m_pMp3->Pause();
        }
    }

    void MusicWin32::resume()
    {
        if (!m_isPlaying)
        {
            m_isPlaying = true;
            m_pMp3->Play(m_loop);
        }
    }

    bool MusicWin32::isDone()
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
