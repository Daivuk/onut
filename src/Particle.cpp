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

        // Animate constant properties
        color.update(t);
        angle.update(t);
        size.update(t);
        image_index.update(t);
        if (!pDesc->textures.empty())
        {
            pTexture = pDesc->textures[static_cast<decltype(pDesc->textures.size())>(image_index.value)];
        }
    }
}
