// Onut
#include <onut/ContentManager.h>
#include <onut/Strings.h>

// Internal
#include "MusicRPI.h"

// Third party
#include <mp3/Mp3.h>

// STL
#include <cassert>

namespace onut
{
    OMusicRef Music::createFromFile(const std::string& filename, const OContentManagerRef& pContentManager)
    {
        auto pRet = std::shared_ptr<MusicRPI>(new MusicRPI());

        return pRet;
    }

    MusicRPI::MusicRPI()
    {
    }

    void MusicRPI::play(bool loop)
    {
        if (m_isPlaying) return;
        m_loop = loop;
        setVolume(m_volume);

        // play ...

        m_isPlaying = true;
    }

    void MusicRPI::setVolume(float volume)
    {
        float decibelDenominator = 4000.f;
        if (volume < 0.1f)
            decibelDenominator = 10000.f;
        m_volume = volume;

        // set volume ...
    }

    void MusicRPI::stop()
    {
        m_isPlaying = false;
        // Stop ...
    }

    void MusicRPI::pause()
    {
        if (m_isPlaying)
        {
            m_isPlaying = false;
            // Pause ...
        }
    }

    void MusicRPI::resume()
    {
        if (!m_isPlaying)
        {
            m_isPlaying = true;
            // Play ...
        }
    }

    bool MusicRPI::isDone()
    {
        if (m_isPlaying)
        {
            // Check with SDL yo ...
        }

        return false;
    }
}
