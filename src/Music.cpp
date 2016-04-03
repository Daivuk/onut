// Onut
#include <onut/ContentManager.h>
#include <onut/Music.h>
#include <onut/Strings.h>

// Third party
#if defined(WIN32)
#include <mp3/Mp3.h>
#endif

// STL
#include <cassert>

namespace onut
{
    OMusicRef Music::createFromFile(const std::string& filename, const OContentManagerRef& pContentManager)
    {
        auto pRet = std::shared_ptr<OMusic>(new OMusic());

        auto cmd = utf8ToUtf16(filename);
        if (!pRet->m_pMp3->Load(cmd.c_str()))
        {
            return nullptr;
        }

        return pRet;
    }

    Music::Music()
    {
#if defined(WIN32)
        m_pMp3 = OMake<Mp3>();
#endif
    }

    void Music::play()
    {
#if defined(WIN32)
        if (m_isPlaying) return;

        setVolume(m_volume);
        if (!m_pMp3->Play())
        {
            return;
        }

        m_isPlaying = true;
#endif
    }

    void Music::setVolume(float volume)
    {
#if defined(WIN32)
        float decibelDenominator = 4000.f;
        if (volume < 0.1f)
            decibelDenominator = 10000.f;
        m_volume = volume;
        m_pMp3->SetVolume(static_cast<long>((m_volume - 1.f) * decibelDenominator));
#endif
    }

    void Music::stop()
    {
#if defined(WIN32)
        m_isPlaying = false;
        m_pMp3->Stop();
#endif
    }

    void Music::pause()
    {
#if defined(WIN32)
        if (m_isPlaying)
        {
            m_isPlaying = false;
            m_pMp3->Pause();
        }
#endif
    }

    void Music::resume()
    {
#if defined(WIN32)
        if (!m_isPlaying)
        {
            m_isPlaying = true;
            m_pMp3->Play();
        }
#endif
    }

    bool Music::isDone()
    {
#if defined(WIN32)
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
#endif
        return false;
    }
}

OMusicRef OGetMusic(const std::string& name)
{
    return oContentManager->getResourceAs<OMusic>(name);
}

void OPlayMusic(const std::string& name)
{
    auto pMusic = OGetMusic(name);
    if (pMusic) pMusic->play();
}

void OStopMusic(const std::string& name)
{
    auto pMusic = OGetMusic(name);
    if (pMusic) pMusic->stop();
}
