#pragma once

// Forward declaration
#include <onut/ForwardDeclaration.h>
OForwardDeclare(AudioStream)

namespace onut
{
    class AudioEngine;

    class AudioStream
    {
    protected:
        friend class AudioEngine;

        // Returns false when completed
        virtual bool progress(int frameCount, int sampleRate, int channelCount, float* pOut) = 0;
    };
}
