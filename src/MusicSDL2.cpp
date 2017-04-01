// Onut
#include <onut/ContentManager.h>
#include <onut/Log.h>
#include <onut/Strings.h>

// Internal
#include "MusicSDL2.h"

// Third party
#include <mp3/Mp3.h>

// STL
#include <cassert>

namespace onut
{
    OMusicRef Music::createFromFile(const std::string& filename, const OContentManagerRef& pContentManager)
    {
        auto pRet = std::shared_ptr<MusicSDL2>(new MusicSDL2());

        pRet->m_pMusic = Mix_LoadMUS(filename.c_str());
        if (!pRet->m_pMusic)
        {
            OLog("Failed to load music: " + filename);
            //return nullptr;
        }

        return pRet;
    }

    MusicSDL2::MusicSDL2()
    {
    }

    MusicSDL2::~MusicSDL2()
    {
        if (m_pMusic)
        {
            Mix_FreeMusic(m_pMusic);
        }
    }

    void MusicSDL2::play(bool loop)
    {
        if (m_isPlaying || !m_pMusic) return;
        m_loop = loop;
        setVolume(m_volume);

        m_isPlaying = Mix_PlayMusic(m_pMusic, m_loop ? -1 : 1) == 0;
    }

    void MusicSDL2::setVolume(float volume)
    {
        float decibelDenominator = 4000.f;
        if (volume < 0.1f)
            decibelDenominator = 10000.f;
        m_volume = volume;

        // set volume ...
    }

    void MusicSDL2::stop()
    {
        m_isPlaying = false;
        // Stop ...
    }

    void MusicSDL2::pause()
    {
        if (m_isPlaying)
        {
            m_isPlaying = false;
            // Pause ...
        }
    }

    void MusicSDL2::resume()
    {
        if (!m_isPlaying)
        {
            m_isPlaying = true;
            // Play ...
        }
    }

    bool MusicSDL2::isDone()
    {
        if (m_isPlaying)
        {
            // Check with SDL yo ...
        }

        return false;
    }
}
