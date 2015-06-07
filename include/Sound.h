#pragma once
#include <string>
#include <vector>
#include <memory>
namespace DirectX
{
    class SoundEffect;
    class SoundEffectInstance;
}

namespace onut
{
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
        void play(float volume = 1.f, float balance = 0.f);
        void stop();

    private:
        DirectX::SoundEffect* m_pSound = nullptr;
        std::vector<std::shared_ptr<DirectX::SoundEffectInstance>> m_instances;
        int m_maxInstance = -1;
    };
}

typedef onut::Sound OSound;
