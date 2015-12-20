#include "Music.h"
#include "Utils.h"
#include "Mp3.h"

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

        auto cmd = utf8ToUtf16("../../assets/musics/" + filename);
        m_pMp3->Load(cmd.c_str());
        m_pMp3->SetVolume(static_cast<long>((m_volume - 1) * 10000));
        m_pMp3->Play();

        m_isPlaying = true;
    }

    void Music::setVolume(float volume)
    {
        m_pMp3->SetVolume(static_cast<long>((m_volume - 1) * 10000));
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
}
