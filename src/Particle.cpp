// Onut
#include <onut/ParticleSystem.h>
#include <onut/Timing.h>

// Private
#include "Particle.h"
#include "ParticleEmitter.h"

namespace onut
{
    void Particle::update()
    {
        float dt = ODT;
        float t = 1 - life;
        life -= delta * dt;
        if (life < 0.f) life = 0.f;

        // Animate position with velocity
        position += velocity * dt;
        velocity += pDesc->gravity * dt;
        angle.from += rotation.value * dt;
        angle.to += rotation.value * dt;

        // Acceleration
        if (pDesc->accelType == OParticleEmitterDesc::AccelType::Gravity)
        {
            auto radial = position - pEmitter->getPosition();
            radial.Normalize();
            auto tagent = Vector3(radial.y, -radial.x, 0);

            radial *= radialAccel.value;
            tagent *= tangentAccel.value;

            auto accel = pDesc->gravity + radial + tagent;
            velocity += accel * dt;
        }
        //else if (pEmitter->getDesc()->accelType == sEmitterDesc::AccelType::Radial)
        //{
        //    // FIX 2
        //    // Update the angle of the particle from the sourcePosition and the radius.  This is only done if the particles are rotating
        //    currentParticle->m_rAngle -= currentParticle->m_degreesPerSecond * in_delta;
        //    currentParticle->m_radius += currentParticle->m_radiusDelta * in_delta;

        //    Vector2f tmp;
        //    tmp.x = currentParticle->m_sourcePosition.x - cosf(currentParticle->m_rAngle) * currentParticle->m_radius;
        //    tmp.y = currentParticle->m_sourcePosition.y - sinf(currentParticle->m_rAngle) * currentParticle->m_radius;
        //    currentParticle->SetPosition(tmp);
        //}

        // Animate constant properties
        color.update(t);
        angle.update(t);
        size.update(t);
        image_index.update(t);
        rotation.update(t);
        radialAccel.update(t);
        tangentAccel.update(t);
    }
}
