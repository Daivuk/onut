#pragma once
#include "Particle.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "Pool.h"
#include "Texture.h"

namespace onut
{
    class IParticleSystemManager
    {
    public:
        virtual Particle* allocParticle() = 0;
        virtual Particle* deallocParticle(Particle* pParticle) = 0;
        virtual void renderParticle(Particle* pParticle, const Vector3& camRight, const Vector3& camUp) = 0;
    };

    template<bool TsortEmitters = false,
             uintptr_t TmaxPFX = 100,
             uintptr_t TmaxParticles = 2000>
    class ParticleSystemManager : public IParticleSystemManager
    {
    public:
        class EmitterInstance
        {
        public:
            void setTransform(const Vector3& pos, const Vector3& dir = Vector3::UnitZ)
            {
                Matrix transform = Matrix::CreateBillboard(pos, pos + dir, Vector3::UnitY);
                setTransform(transform);
            }

            void setTransform(const Matrix& transform)
            {
                if (m_pParticleSystemManager)
                {
                    auto len = m_pParticleSystemManager->m_emitterPool.size();
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        auto pEmitter = m_pParticleSystemManager->m_emitterPool.at<ParticleEmitter>(i);
                        if (m_pParticleSystemManager->m_emitterPool.isUsed(pEmitter))
                        {
                            if (pEmitter->getInstanceId() == m_id)
                            {
                                pEmitter->setTransform(transform);
                            }
                        }
                    }
                }
            }

            void stop()
            {
                if (m_pParticleSystemManager)
                {
                    auto len = m_pParticleSystemManager->m_emitterPool.size();
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        auto pEmitter = m_pParticleSystemManager->m_emitterPool.at<ParticleEmitter>(i);
                        if (m_pParticleSystemManager->m_emitterPool.isUsed(pEmitter))
                        {
                            if (pEmitter->getInstanceId() == m_id)
                            {
                                pEmitter->stop();
                            }
                        }
                    }
                }
                m_bStopped = true;
            }

            bool isPlaying() const
            {
                if (m_bStopped) return false;
                if (m_pParticleSystemManager)
                {
                    auto len = m_pParticleSystemManager->m_emitterPool.size();
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        auto pEmitter = m_pParticleSystemManager->m_emitterPool.at<ParticleEmitter>(i);
                        if (m_pParticleSystemManager->m_emitterPool.isUsed(pEmitter))
                        {
                            if (pEmitter->getInstanceId() == m_id)
                            {
                                if (pEmitter->isAlive()) return true;
                            }
                        }
                    }
                }
                return false;
            }

            bool isAlive() const
            {
                if (m_pParticleSystemManager)
                {
                    auto len = m_pParticleSystemManager->m_emitterPool.size();
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        auto pEmitter = m_pParticleSystemManager->m_emitterPool.at<ParticleEmitter>(i);
                        if (m_pParticleSystemManager->m_emitterPool.isUsed(pEmitter))
                        {
                            if (pEmitter->getInstanceId() == m_id)
                            {
                                if (pEmitter->isAlive()) return true;
                            }
                        }
                    }
                }
                return false;
            }

        private:
            friend class ParticleSystemManager;

            uint32_t m_id;
            ParticleSystemManager* m_pParticleSystemManager;
            bool m_bStopped = false;
        };

        EmitterInstance emit(ParticleSystem* pParticleSystem, const Vector3& pos, const Vector3& dir = Vector3::UnitZ)
        {
            static uint32_t nextId = 1;
            EmitterInstance instance;
            instance.m_id = nextId++;
            instance.m_pParticleSystemManager = this;

            Matrix transform = Matrix::CreateBillboard(pos, pos + dir, Vector3::UnitY);
            for (auto& emitter : pParticleSystem->emitters)
            {
                auto pEmitter = m_emitterPool.alloc<ParticleEmitter>(&emitter, this, transform, instance.m_id);
                // Update the first frame right away
                pEmitter->update();
            }

            return instance;
        }

        void clear()
        {
            m_emitterPool.clear();
            m_particlePool.clear();
        }

        void update()
        {
            updateEmitters();
        }

        void render()
        {
            OSpriteBatch->begin();
            if (TsortEmitters)
            {
            }
            else
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
            OSpriteBatch->end();
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

        void renderParticle(Particle* pParticle, const Vector3& camRight, const Vector3& camUp) override
        {
            float dim = static_cast<float>(std::max<decltype(pParticle->pTexture->getSize().x)>(pParticle->pTexture->getSize().x, pParticle->pTexture->getSize().y));
            OSpriteBatch->drawSprite(pParticle->pTexture, pParticle->position, pParticle->color.value, pParticle->angle.value, pParticle->size.value / dim);
        }

    private:
        friend class EmitterInstance;

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

        StaticPool<sizeof(ParticleEmitter), TmaxPFX, sizeof(uintptr_t), false> m_emitterPool;
        StaticPool<sizeof(Particle), TmaxParticles, sizeof(uintptr_t), false> m_particlePool;
        Vector3 m_camRight;
        Vector3 m_camUp;
    };
}
