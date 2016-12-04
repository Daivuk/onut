#ifndef AUDIOSTREAM_H_INCLUDED
#define AUDIOSTREAM_H_INCLUDED


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

#endif
