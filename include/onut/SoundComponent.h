#ifndef SOUNDCOMPONENT_H_INCLUDED
#define SOUNDCOMPONENT_H_INCLUDED


// Onut includes
#include <onut/Component.h>
#include <onut/Maths.h>

// Forward declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Sound);
OForwardDeclare(SoundComponent);
OForwardDeclare(SoundInstance);

namespace onut
{
    class SoundComponent final : public Component
    {
    public:
        SoundComponent();

        void setSound(const OSoundRef& pSound);
        const OSoundRef& getSound() const;

        void setVolume(float volume);
        const float getVolume() const;

        void setPitch(float pitch);
        const float getPitch() const;

        void setLoop(bool loop);
        const bool getLoop() const;

        void play();
        void stop();

    private:
        void onUpdate() override;

        OSoundRef m_pSound;
        float m_volume = 1.0f;
        float m_pitch = 1.0f;
        OSoundInstanceRef m_pSoundInstance;
        bool m_isLoop = false;
        bool m_volumeDirty = false;
        bool m_pitchDirty = false;
    };
};

#endif
