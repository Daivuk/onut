#pragma once
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

namespace onut
{
    class Texture;
    class ParticleSystem;

    class Particle
    {
    public:
        void update();

        Vector3             position;
        Vector3             velocity;
        Color               color;
        Texture*            pTexture = nullptr;
        Particle*           pNext = nullptr;
        ParticleSystem*     pParticleSystem = nullptr;
    };
}
