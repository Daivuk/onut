// onut includes
#include <onut/Entity.h>
#include <onut/Sound.h>
#include <onut/SoundComponent.h>

namespace onut
{
    SoundComponent::SoundComponent()
        : Component(FLAG_UPDATABLE)
    {
    }

    void SoundComponent::setSound(const OSoundRef& pSound)
    {
        m_pSound = pSound;
    }

    const OSoundRef& SoundComponent::getSound() const
    {
        return m_pSound;
    }

    void SoundComponent::setVolume(float volume)
    {
        m_volume = volume;
        m_volumeDirty = m_volume != volume;
    }
    const float SoundComponent::getVolume() const
    {
        return m_volume;
    }

    void SoundComponent::setPitch(float pitch)
    {
        m_pitch = pitch;
        m_pitchDirty = m_pitch != pitch;
    }

    const float SoundComponent::getPitch() const
    {
        return m_pitch;
    }

    void SoundComponent::setLoop(bool loop)
    {
        m_isLoop = loop;
    }

    const bool SoundComponent::getLoop() const
    {
        return m_isLoop;
    }

    void SoundComponent::play()
    {
        m_pSoundInstance = m_pSound->createInstance();
        m_pSoundInstance->setLoop(m_isLoop);
        m_pSoundInstance->setVolume(m_volume);
        m_pSoundInstance->setPitch(m_pitch);
        m_pSoundInstance->play();
        m_volumeDirty = false;
        m_pitchDirty = false;
    }

    void SoundComponent::stop()
    {
        m_pSoundInstance->stop();
        m_pSoundInstance = nullptr;
    }

    void SoundComponent::onUpdate()
    {
        if (m_pSoundInstance && m_pSoundInstance->isPlaying())
        {
            if (m_volumeDirty)
            {
                m_pSoundInstance->setVolume(m_volume);
                m_volumeDirty = false;
            }
            if (m_pitchDirty)
            {
                m_pSoundInstance->setPitch(m_pitch);
                m_pitchDirty = false;
            }
        }
    }
};
