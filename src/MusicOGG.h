#ifndef MUSICOGG_H_INCLUDED
#define MUSICOGG_H_INCLUDED

// Onut
#include <onut/AudioStream.h>
#include <onut/Music.h>
#include <onut/Resource.h>

// STL
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(MusicOGG)

// Third party (Have to include this last)
#include <stb/stb_vorbis.c>

namespace onut
{
    class MusicOGG final : public Music, public AudioStream, public std::enable_shared_from_this<AudioStream>
    {
    public:
        MusicOGG();
        ~MusicOGG();
        
        void play(bool loop = false) override;
        void stop() override;
        void pause() override;
        void resume() override;
        void setVolume(float volume) override;

        bool isPlaying() const override { return m_isPlaying; }
        bool isPaused() const override { return m_paused && m_isPlaying; }
        bool isDone() override;

    protected:
        bool progress(int frameCount, int sampleRate, int channelCount, float* pOut) override;

    private:
        friend class Music;

        struct Buffer
        {
            int offset = 0;
            int count = 0;
            std::vector<float> data;
        };

        void run();

        std::mutex m_mutex;
        std::thread m_thread;
        std::atomic<float> m_volume;
        std::atomic<bool> m_isPlaying;
        std::atomic<bool> m_loop;
        std::vector<Buffer*> m_buffers;
        std::atomic<int> m_bufferCount;
        std::atomic<bool> m_done;
        float* m_pSamplings = nullptr;
        float m_samplingT = 0.0f;
        int m_musicSampleRate = 0;
        int m_musicChannelCount = 0;
        bool m_paused = false;
        unsigned int m_sampleCount = 0;
        int m_bufferMax;
        int m_engineChannelCount;
        std::string m_filename;
        stb_vorbis* m_pStream = nullptr;
        stb_vorbis_info m_info;
    };
}

#endif
