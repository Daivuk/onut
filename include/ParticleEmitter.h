#pragma once
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

namespace onut
{
    class Particle;
    class ParticleSystem;
    class IParticleSystemManager;
    struct sEmitterDesc;

    class ParticleEmitter
    {
    public:
        ParticleEmitter(sEmitterDesc* pEmitterDesc, IParticleSystemManager* pParticleSystemManager, const Matrix& transform);
        ~ParticleEmitter();

        bool isAlive() const { return m_isAlive; }

        void update();
        void render();

    private:
        Particle* spawnParticle();

        Particle*               m_pParticles;
        IParticleSystemManager* m_pParticleSystemManager;
        bool                    m_isAlive = false;
        Matrix                  m_transform;
        sEmitterDesc*           m_pDesc;
    };
}
