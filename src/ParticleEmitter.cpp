#include "ParticleSystemManager.h"

namespace onut
{
    ParticleEmitter::ParticleEmitter(ParticleSystem* pParticleSystem, IParticleSystemManager* pParticleSystemManager) :
        m_pParticleSystem(pParticleSystem),
        m_pParticleSystemManager(pParticleSystemManager)
    {
        m_isAlive = true;
    }

    ParticleEmitter::~ParticleEmitter()
    {
        auto pParticle = m_pParticles;
        while (pParticle)
        {
            pParticle = m_pParticleSystemManager->deallocParticle(pParticle);
        }
    }

    void ParticleEmitter::update()
    {
    }

    void ParticleEmitter::render()
    {
    }
}
