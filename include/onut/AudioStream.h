#ifndef AUDIOSTREAM_H_INCLUDED
#define AUDIOSTREAM_H_INCLUDED

// Onut
#include <onut/Vector3.h>

// Forward declaration
#include <onut/ForwardDeclaration.h>
OForwardDeclare(AudioStream)

namespace onut
{
    class AudioEngine;

    class AudioStream
    {
    public:
        virtual ~AudioStream() {}

        void set3D(bool enabled = false, const Vector3& position = {0, 0, 0}, float radius = 1.0f)
        {
            m_positioning.enabled = enabled;
            m_positioning.pos = position;
            m_positioning.radius = radius;
        }

    protected:
        friend class AudioEngine;

        // Returns false when completed
        virtual bool progress(int frameCount, int sampleRate, int channelCount, float* pOut, float volume = 1.0f, float balance = 0.0f, float pitch = 1.0f) = 0;

        virtual void onStopped() {};

        struct Positioning
        {
            bool enabled = false;
            Vector3 pos = {0, 0, 0};
            float radius = 0.0f;
        } m_positioning;
    };
}

#endif
