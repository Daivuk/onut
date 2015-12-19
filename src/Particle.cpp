#include "Particle.h"
#include "ParticleSystem.h"
#include "TimingUtils.h"

namespace onut
{
    void Particle::update()
    {
        float t = 1 - life;
        life -= delta * ODT;
        if (life < 0.f) life = 0.f;

        // Animate position with velocity
        position += velocity * ODT;
        velocity += pDesc->gravity * ODT;
        angle.from += rotation.value * ODT;
        angle.to += rotation.value * ODT;

        // Animate constant properties
        color.update(t);
        angle.update(t);
        size.update(t);
        image_index.update(t);
        rotation.update(t);
    }
}
