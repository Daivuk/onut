#pragma once
#include <string>
namespace DirectX
{
    class SoundEffect;
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

        void play();
        void stop();

    private:
        DirectX::SoundEffect* m_pSound = nullptr;
    };
}

typedef onut::Sound OSound;
