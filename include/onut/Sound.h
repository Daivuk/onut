#pragma once
// Onut
#include <onut/Resource.h>
#include <onut/Timer.h>

// STL
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ContentManager);
OForwardDeclare(Sound);
OForwardDeclare(SoundCue);
OForwardDeclare(SoundInstance);
namespace DirectX
{
    class SoundEffect;
    class SoundEffectInstance;
}

namespace onut
{
    class SoundInstance final
    {
    public:
        void play();
        void pause();
        void stop();

        bool isPlaying() const;
        bool isPaused() const;

        bool getLoop() const;
        void setLoop(bool loop);

        float getVolume() const;
        void setVolume(float volume);

        float getBalance() const;
        void setBalance(float balance);

        float getPitch() const;
        void setPitch(float pitch);

    private:
        friend class Sound;

        using DXSoundEffectRef = std::shared_ptr<DirectX::SoundEffect>;
        using DXSoundEffectInstanceRef = std::shared_ptr<DirectX::SoundEffectInstance>;

        DXSoundEffectRef m_pSound;
        DXSoundEffectInstanceRef m_pInstance;

        bool m_isPaused = false;
        bool m_loop = false;
        float m_volume = 1.f;
        float m_balance = 0.f;
        float m_pitch = 1.f;
    };

    class Sound final : public Resource
    {
    public:
        static OSoundRef createFromFile(const std::string& filename, const OContentManagerRef& pContentManager = nullptr);

        void setMaxInstance(int maxInstance = -1) { m_maxInstance = maxInstance; }
        void play(float volume = 1.f, float balance = 0.f, float pitch = 1.f);
        void stop();

        OSoundInstanceRef createInstance();

    private:
        using DXSoundEffectRef = std::shared_ptr<DirectX::SoundEffect>;
        using DXSoundEffectInstanceRef = std::shared_ptr<DirectX::SoundEffectInstance>;
        using Instances = std::vector<DXSoundEffectInstanceRef>;

        DXSoundEffectRef m_pSound;
        Instances m_instances;
        int m_maxInstance = -1;
    };
    
    class SoundCue final : public Resource
    {
    public:
        static OSoundCueRef createFromFile(const std::string& filename, const OContentManagerRef& pContentManager = nullptr);

        void play(float volume = 1.0f, float balance = 0.f, float pitch = 1.f);

    private:
        struct Sound
        {
            OSoundRef pSound;

            float volume = 1.f;
            float balance = 0.f;
            float pitch = 1.f;
            float delay = 0.f;
            float chance = 0.f;
        };

        struct Play
        {
            std::vector<Sound> sounds;
            float totalChances = 0.f;

            OTimer playTimer;
        };

        using Plays = std::vector<Play>;

        Plays m_plays;
    };
}

OSoundRef OGetSound(const std::string& name);
void OPlaySound(const std::string& name, float volume = 1.f, float balance = 0.f, float pitch = 1.f);
void OPlayRandomSound(const std::vector<std::string>& sounds, float volume = 1.0f, float balance = 0.f, float pitch = 1.f);
OSoundInstanceRef OCreateSoundInstance(const std::string& name);
OSoundCueRef OGetSoundCue(const std::string& name);
void OPlaySoundCue(const std::string& name, float volume = 1.f, float balance = 0.f, float pitch = 1.f);
