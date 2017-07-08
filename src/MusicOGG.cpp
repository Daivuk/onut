// Onut
#include <onut/AudioEngine.h>
#include <onut/ContentManager.h>
#include <onut/Strings.h>

// STL
#include <algorithm>
#include <cassert>
#include <chrono>

// Internal (Have to include that last)
#include "MusicOGG.h"

// 5 seconds pre-buffer
#define MUSIC_BUFFER_COUNT 5

namespace onut
{
    OMusicRef Music::createFromFile(const std::string& filename, const OContentManagerRef& pContentManager)
    {
        auto pRet = std::shared_ptr<MusicOGG>(new MusicOGG());

        pRet->m_filename = filename;

        return pRet;
    }

    MusicOGG::MusicOGG()
        : m_volume(1.f)
        , m_isPlaying(false)
        , m_loop(false)
    {
    }

    MusicOGG::~MusicOGG()
    {
        for (auto pBuffer : m_buffers) delete pBuffer;
        m_buffers.clear();
        stop();
        if (m_thread.joinable()) m_thread.join();
    }

    void MusicOGG::play(bool loop)
    {
        if (m_isPlaying) return;
        m_loop = loop;

        for (auto pBuffer : m_buffers) delete pBuffer;
        m_buffers.clear();
        m_bufferCount = 0;

        m_pStream = stb_vorbis_open_filename((char*)m_filename.c_str(), NULL, NULL);
        if (!m_pStream) return;

        m_info = stb_vorbis_get_info(m_pStream);
        m_sampleCount = stb_vorbis_stream_length_in_samples(m_pStream);

        m_isPlaying = true;
        m_paused = false;

        // We will buffer 5 seconds
        m_engineChannelCount = oAudioEngine->getChannels();
        m_bufferMax = oAudioEngine->getSampleRate() * m_engineChannelCount;
        for (int i = 0; i < MUSIC_BUFFER_COUNT; ++i)
        {
            auto pBuffer = new Buffer();
            pBuffer->data.assign(m_bufferMax, 0.0f);
            m_buffers.push_back(pBuffer);
        }
        m_bufferCount = 0;

        if (m_thread.joinable()) m_thread.join();
        m_thread = std::thread(std::bind(&MusicOGG::run, this));

        oAudioEngine->addInstance(OThis);
    }

    void MusicOGG::setVolume(float volume)
    {
        m_volume = volume;
    }

    void MusicOGG::stop()
    {
        if (!m_isPlaying) return;
        m_isPlaying = false;
        m_paused = false;

        if (oAudioEngine) oAudioEngine->removeInstance(OThis);

        m_thread.join();

        stb_vorbis_close(m_pStream);

        m_pStream = nullptr;

        for (auto pBuffer : m_buffers) delete pBuffer;
        m_buffers.clear();
        m_bufferCount = 0;
    }

    void MusicOGG::pause()
    {
        if (!m_paused && m_isPlaying)
        {
            m_paused = true;
            oAudioEngine->removeInstance(OThis);
        }
    }

    void MusicOGG::resume()
    {
        if (m_paused && m_isPlaying)
        {
            m_paused = false;
            oAudioEngine->addInstance(OThis);
        }
    }

    bool MusicOGG::isDone()
    {
        return !m_isPlaying && m_done;
    }

    void MusicOGG::run()
    {
        while (m_isPlaying)
        {
            if (m_bufferCount < MUSIC_BUFFER_COUNT)
            {
                m_mutex.lock();
                auto pBuffer = m_buffers.back();
                m_buffers.pop_back();
                m_mutex.unlock();
                pBuffer->count = stb_vorbis_get_samples_float_interleaved(m_pStream, m_engineChannelCount, pBuffer->data.data(), m_bufferMax) * m_engineChannelCount;
                pBuffer->offset = 0;
                if (pBuffer->count)
                {
                    m_mutex.lock();
                    m_buffers.insert(m_buffers.begin() + m_bufferCount, pBuffer);
                    ++m_bufferCount;
                    m_mutex.unlock();
                }
                else
                {
                    if (m_loop)
                    {
                        stb_vorbis_seek_start(m_pStream);
                        m_mutex.lock();
                        m_buffers.push_back(pBuffer);
                        m_mutex.unlock();
                        continue;
                    }
                    else
                    {
                        // Done playing!
                        stb_vorbis_close(m_pStream);
                        m_pStream = nullptr;
                        m_done = true;
                        break;
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    bool MusicOGG::progress(int frameCount, int sampleRate, int channelCount, float* pOut)
    {
        if (!m_isPlaying) return true;

        float volume = m_volume;
        int bufferCount = m_bufferCount;
        int sampleFed = 0;
        int requestSamples = frameCount * channelCount;

        while (sampleFed < requestSamples && bufferCount > 0)
        {
            m_mutex.lock();
            auto pBuffer = m_buffers.front();
            m_mutex.unlock();
            int count = std::min(pBuffer->count - pBuffer->offset, requestSamples - sampleFed);
            if (count == 0) break;
            float* pData = pBuffer->data.data() + pBuffer->offset;
            for (int i = 0; i < count; ++i)
            {
                pOut[i] += pData[i] * volume;
            }
            pOut += count;
            sampleFed += count;
            pBuffer->offset += count;
            if (pBuffer->offset >= pBuffer->count)
            {
                // Remove that buffer
                m_mutex.lock();
                m_buffers.erase(m_buffers.begin());
                m_buffers.push_back(pBuffer);
                --m_bufferCount;
                bufferCount = m_bufferCount;
                m_mutex.unlock();
            }

            if (bufferCount <= 0) 
            {
                if (m_done)
                {
                    m_isPlaying = false;
                    return false;
                }
                break;
            }
        }

        return true;
    }
}
