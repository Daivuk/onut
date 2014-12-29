#include "Particle.h"
#include "ParticleSystem.h"
#include "TimingUtils.h"

namespace onut
{
    void Particle::update()
    {
        position += velocity * ODT;
    }
}
