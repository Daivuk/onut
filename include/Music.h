#pragma once
#include <string>

class Mp3;

namespace onut
{
    class Music
    {
    public:
        Music();
        ~Music();

        void play(const std::string& filename);
        void stop();
        void pause();
        void resume();
        void setVolume(float volume);

        bool isPlaying() const { return m_isPlaying; }
        bool isDone();

    private:
        Mp3* m_pMp3 = nullptr;
        float m_volume = 1.f;
        bool m_isPlaying = false;
    };
}

