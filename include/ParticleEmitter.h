#pragma once

namespace onut
{
    class Particle;
    class ParticleSystem;
    class IParticleSystemManager;

    class ParticleEmitter
    {
    public:
        ParticleEmitter(ParticleSystem* pParticleSystem, IParticleSystemManager* pParticleSystemManager);
        ~ParticleEmitter();

        bool isAlive() const { return m_isAlive; }

        void update();
        void render();

    private:
        Particle*               m_pParticles;
        ParticleSystem*         m_pParticleSystem;
        IParticleSystemManager* m_pParticleSystemManager;
        bool                    m_isAlive = false;
    };
}
