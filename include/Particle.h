#pragma once
#include "onut/Maths.h"
#include "onut/ParticleSystem.h"

#include "Anim.h"

#include "onut/ForwardDeclaration.h"
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

            void update(float t)
            {
                value = lerp(from, to, t);
            }
        };

        void update();

        bool isAlive() const { return life > 0.f; }

        OParticleEmitterDescRef pDesc;
        float life;
        float delta;
        ParticleEmitter* pEmitter = nullptr;

        Vector3 position;
        Vector3 velocity;

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
