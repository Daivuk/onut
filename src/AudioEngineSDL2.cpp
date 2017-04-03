// onut
#include <onut/Log.h>

// Private
#include "AudioEngineSDL2.h"

// STL
#include <cassert>

static void my_audio_callback(void *userdata, Uint8 *stream, int len)
{
    auto pAudioEngine = (onut::AudioEngineSDL2*)userdata;
    pAudioEngine->audioCallback(stream, len);
}

namespace onut
{
    OAudioEngineRef AudioEngine::create()
    {
        return std::shared_ptr<AudioEngine>(new AudioEngineSDL2());
    }

    AudioEngineSDL2::AudioEngineSDL2()
    {
        auto ret = SDL_Init(SDL_INIT_AUDIO);
        if (ret < 0)
        {
            OLog("Failed to SDL_Init(SDL_INIT_AUDIO)");
            assert(false);
            return;
        }

        memset(&m_audioSpec, 0, sizeof(m_audioSpec));
        m_audioSpec.freq = 44100;
        m_audioSpec.format = AUDIO_F32;
        m_audioSpec.channels = 2;
        m_audioSpec.samples = 512;
        m_audioSpec.callback = my_audio_callback;
        m_audioSpec.userdata = this;

        if (SDL_OpenAudio(&m_audioSpec, NULL) < 0)
        {
            OLog("Failed to SDL_OpenAudio");
            assert(false);
            return;
        }

        SDL_PauseAudio(0);
    }

    AudioEngineSDL2::~AudioEngineSDL2()
    {
        SDL_CloseAudio();
    }

    void AudioEngineSDL2::audioCallback(Uint8 *stream, int len)
    {
        progressInstances(len / sizeof(float) / (int)m_audioSpec.channels, 
                          (int)m_audioSpec.freq, 
                          (int)m_audioSpec.channels, 
                          (float*)stream);
    }

    void AudioEngineSDL2::update()
    {
    }

    int AudioEngineSDL2::getSampleRate() const
    {
        return (int)m_audioSpec.freq;
    }

    int AudioEngineSDL2::getChannels() const
    {
        return (int)m_audioSpec.channels;
    }
};
