#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "TimingUtils.h"

namespace DirectX
{
    class SoundEffect;
    class SoundEffectInstance;
}

namespace onut
{
    class SoundInstance;

    class Sound
    {
    public:
        template<typename TcontentManagerType>
        static Sound* createFromFile(const std::string& filename, TcontentManagerType* pContentManager)
        {
            return Sound::createFromFile(filename);
        }
        static Sound* createFromFile(const std::string& filename);

        virtual ~Sound();

        void setMaxInstance(int maxInstance = -1) { m_maxInstance = maxInstance; }
        void play(float volume = 1.f, float balance = 0.f, float pitch = 1.f);
        void stop();

        SoundInstance* createInstance() const;

    private:
        DirectX::SoundEffect* m_pSound = nullptr;
        std::vector<std::shared_ptr<DirectX::SoundEffectInstance>> m_instances;
        int m_maxInstance = -1;
    };

    class SoundInstance
    {
    public:
        bool isPlaying() const;

        void setLoop(bool bLoop);
        void setVolume(float volume);
        void setBalance(float balance);
        void setPitch(float pitch);

        void play();
        void pause();
        void stop();

    private:
        friend class Sound;

        DirectX::SoundEffect* m_pSound = nullptr;
        std::shared_ptr<DirectX::SoundEffectInstance> m_instance;
        bool m_isPlaying = false;
        bool m_isLooping = false;
        bool m_isPaused = false;
    };

    class SoundCue
    {
    public:
        template<typename TcontentManagerType>
        static SoundCue* createFromFile(const std::string& filename, TcontentManagerType* pContentManager)
        {
            return SoundCue::createFromFile(filename, [pContentManager](const char* pFilename)
            {
                return pContentManager->getResource<Sound>(pFilename);
            });
        }
        static SoundCue* createFromFile(const std::string& filename, const std::function<Sound*(const char*)>& getSound);

        void play(float volume = 1.0f, float balance = 0.f, float pitch = 1.f);

    private:
        struct sSound
        {
            Sound* pSound = nullptr;

            float volume = 1.f;
            float balance = 0.f;
            float pitch = 1.f;
            float delay = 0.f;
            float chance = 0.f;
        };

        struct sPlay
        {
            std::vector<sSound> sounds;
            float totalChances = 0.f;

            OTimer playTimer;
        };

        std::vector<sPlay> m_plays;
    };
}

using OSound = onut::Sound;
using OSoundInstance = onut::SoundInstance;
using OSoundCue = onut::SoundCue;
