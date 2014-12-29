#pragma once
#include "Particle.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "Pool.h"

namespace onut
{
    class IParticleSystemManager
    {
    public:
        virtual Particle* allocParticle() = 0;
        virtual Particle* deallocParticle(Particle* pParticle) = 0;
    };

    template<uintptr_t TmaxPFX = 100, 
             uintptr_t TmaxParticles = 2000>
    class ParticleSystemManager : public IParticleSystemManager
    {
    public:
        ParticleEmitter* emit(ParticleSystem* pParticleSystem, const Vector3& pos, const Vector3& dir = Vector3::UnitZ)
        {
            return m_emitterPool.alloc<ParticleEmitter>(pParticleSystem, this);
        }

        void clear()
        {
            m_emitterPool.clear();
            m_particlePool.clear();
        }

        void update()
        {
            updateEmitters();
            updateParticles();
        }

        void render()
        {
            auto len = m_emitterPool.size();
            for (decltype(len) i = 0; i < len; ++i)
            {
                auto pEmitter = m_emitterPool.at<ParticleEmitter>(i);
                if (m_emitterPool.isUsed(pEmitter))
                {
                    pEmitter->render();
                }
            }
        }

        Particle* allocParticle() override
        {
            return m_particlePool.alloc<Particle>();
        }

        Particle* deallocParticle(Particle* pParticle) override
        {
            auto pRet = pParticle->pNext;
            m_particlePool.dealloc(pParticle);
            return pRet;
        }

    private:
        void updateEmitters()
        {
            auto len = m_emitterPool.size();
            for (decltype(len) i = 0; i < len; ++i)
            {
                auto pEmitter = m_emitterPool.at<ParticleEmitter>(i);
                if (m_emitterPool.isUsed(pEmitter))
                {
                    if (pEmitter->isAlive())
                    {
                        pEmitter->update();
                        if (!pEmitter->isAlive())
                        {
                            m_emitterPool.dealloc(pEmitter);
                        }
                    }
                    else
                    {
                        m_emitterPool.dealloc(pEmitter);
                    }
                }
            }
        }

        void updateParticles()
        {
            auto len = m_particlePool.size();
            for (decltype(len) i = 0; i < len; ++i)
            {
                auto pParticle = m_particlePool.at<Particle>(i);
                if (m_particlePool.isUsed(pParticle))
                {
                    pParticle->update();
                }
            }
        }

        Pool<sizeof(ParticleEmitter), TmaxPFX, sizeof(uintptr_t), false>    m_emitterPool;
        Pool<sizeof(Particle), TmaxParticles, sizeof(uintptr_t), false>     m_particlePool;
    };
}
