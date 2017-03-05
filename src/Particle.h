#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

// Onut
#include <onut/Curve.h>
#include <onut/Maths.h>
#include <onut/Tween.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ParticleEmitterDesc);
OForwardDeclare(ParticleSystem);
OForwardDeclare(Texture);

namespace onut
{
    class ParticleEmitter;

    class Particle final
    {
    public:
        template<typename Ttype>
        struct Range
        {
            Ttype from;
            Ttype to;
            Ttype value;
            Tween tween;

            void update(float t)
            {
                t = OApplyTween(t, tween);
                value = OLerp(from, to, t);
            }
        };

        void update();

        bool isAlive() const { return life > 0.f; }

        OParticleEmitterDescRef pDesc;
        float life;
        float delay;
        float delta;
        ParticleEmitter* pEmitter = nullptr;

        Vector3 position;
        Vector3 vel;

        Range<Vector3> velocity;
        Range<Vector3> gravity;
        Range<Color> color;
        Range<float> angle;
        Range<float> size;
        Range<unsigned int> image_index;
        Range<float> rotation;
        Range<float> radialAccel;
        Range<float> tangentAccel;

        OTextureRef pTexture;
    };
}

#endif
