#pragma once
#include "onut/ParticleSystem.h"
#include "onut/Pool.h"
#include "onut/SpriteBatch.h"
#include "onut/Texture.h"

#include "Particle.h"
#include "ParticleEmitter.h"

namespace onut
{
    class IParticleSystemManager
    {
    public:
        virtual Particle* allocParticle() = 0;
        virtual void deallocParticle(Particle* pParticle) = 0;
        virtual void renderParticle(Particle* pParticle, const Vector3& camRight, const Vector3& camUp) = 0;
    };

    template<bool TsortEmitters = false,
             uintptr_t TmaxPFX = 100,
             uintptr_t TmaxParticles = 2000>
    class ParticleSystemManager : public IParticleSystemManager
    {
    public:
        ParticleSystemManager()
        {
            m_pEmitterPool = OPool::create(sizeof(ParticleEmitter), TmaxPFX);
            m_pParticlePool = OPool::create(sizeof(Particle), TmaxParticles);
        }

        class EmitterInstance
        {
        public:
            void setTransform(const Vector3& pos, const Vector3& dir = Vector3::UnitY, const Vector3& up = Vector3::UnitZ)
            {
                Matrix transform = Matrix::CreateBillboard(pos, pos + up, dir);
                setTransform(transform);
            }

            void setRenderEnabled(bool renderEnabled)
            {
                if (m_pParticleSystemManager)
                {
                    auto len = m_pParticleSystemManager->m_pEmitterPool->size();
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        auto pEmitter = m_pParticleSystemManager->m_pEmitterPool->at<ParticleEmitter>(i);
                        if (m_pParticleSystemManager->m_pEmitterPool->isUsed(pEmitter))
                        {
                            if (pEmitter->getInstanceId() == m_id)
                            {
                                pEmitter->setRenderEnabled(renderEnabled);
                            }
                        }
                    }
                }
            }

            void setTransform(const Matrix& transform)
            {
                if (m_pParticleSystemManager)
                {
                    auto len = m_pParticleSystemManager->m_pEmitterPool->size();
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        auto pEmitter = m_pParticleSystemManager->m_pEmitterPool->at<ParticleEmitter>(i);
                        if (m_pParticleSystemManager->m_pEmitterPool->isUsed(pEmitter))
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
                    auto len = m_pParticleSystemManager->m_pEmitterPool->size();
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        auto pEmitter = m_pParticleSystemManager->m_pEmitterPool->at<ParticleEmitter>(i);
                        if (m_pParticleSystemManager->m_pEmitterPool->isUsed(pEmitter))
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
                    auto len = m_pParticleSystemManager->m_pEmitterPool->size();
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        auto pEmitter = m_pParticleSystemManager->m_pEmitterPool->at<ParticleEmitter>(i);
                        if (m_pParticleSystemManager->m_pEmitterPool->isUsed(pEmitter))
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
                    auto len = m_pParticleSystemManager->m_pEmitterPool->size();
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        auto pEmitter = m_pParticleSystemManager->m_pEmitterPool->at<ParticleEmitter>(i);
                        if (m_pParticleSystemManager->m_pEmitterPool->isUsed(pEmitter))
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

            void render()
            {
                if (m_pParticleSystemManager)
                {
                    bool bManageBatch = !oSpriteBatch->isInBatch();
                    if (bManageBatch) oSpriteBatch->begin();
                    auto len = m_pParticleSystemManager->m_pEmitterPool->size();
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        auto pEmitter = m_pParticleSystemManager->m_pEmitterPool->at<ParticleEmitter>(i);
                        if (m_pParticleSystemManager->m_pEmitterPool->isUsed(pEmitter))
                        {
                            if (pEmitter->getInstanceId() == m_id)
                            {
                                pEmitter->render();
                            }
                        }
                    }
                    if (bManageBatch) oSpriteBatch->end();
                }
            }

        private:
            friend class ParticleSystemManager;

            uint32_t m_id = 0;
            ParticleSystemManager* m_pParticleSystemManager = nullptr;
            bool m_bStopped = false;
        };

        EmitterInstance emit(const OParticleSystemRef& pParticleSystem, const Vector3& pos, const Vector3& dir = Vector3::UnitZ)
        {
            static uint32_t nextId = 1;
            EmitterInstance instance;
            instance.m_id = nextId++;
            instance.m_pParticleSystemManager = this;

            Matrix transform = Matrix::CreateBillboard(pos, pos + dir, Vector3::UnitY);
            auto& emitters = pParticleSystem->getEmitters();
            for (auto& emitter : emitters)
            {
                auto pEmitter = m_pEmitterPool->alloc<ParticleEmitter>(emitter, this, transform, instance.m_id);
                // Update the first frame right away
                if (pEmitter) pEmitter->update();
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

        bool hasAliveParticles() const
        {
            auto len = m_emitterPool.size();
            for (decltype(len) i = 0; i < len; ++i)
            {
                auto pEmitter = m_pEmitterPool->at<ParticleEmitter>(i);
                if (m_pEmitterPool->isUsed(pEmitter))
                {
                    return true;
                }
            }
            return false;
        }

        void render()
        {
            oSpriteBatch->begin();
            if (TsortEmitters)
            {
            }
            else
            {
                auto len = m_pEmitterPool->size();
                for (decltype(len) i = 0; i < len; ++i)
                {
                    auto pEmitter = m_pEmitterPool->at<ParticleEmitter>(i);
                    if (m_pEmitterPool->isUsed(pEmitter))
                    {
                        if (pEmitter->getRenderEnabled())
                        {
                            pEmitter->render();
                        }
                    }
                }
            }
            oSpriteBatch->end();
        }

        Particle* allocParticle() override
        {
            return m_pParticlePool->alloc<Particle>();
        }

        void deallocParticle(Particle* pParticle) override
        {
            m_pParticlePool->dealloc(pParticle);
        }

        void renderParticle(Particle* pParticle, const Vector3& camRight, const Vector3& camUp) override
        {
            float dim = static_cast<float>(std::max<decltype(pParticle->pTexture->getSize().x)>(pParticle->pTexture->getSize().x, pParticle->pTexture->getSize().y));
            oSpriteBatch->drawSprite(pParticle->pTexture, pParticle->position, pParticle->color.value, pParticle->angle.value, pParticle->size.value / dim);
        }

    private:
        friend class EmitterInstance;

        void updateEmitters()
        {
            auto len = m_pEmitterPool->size();
            for (decltype(len) i = 0; i < len; ++i)
            {
                auto pEmitter = m_pEmitterPool->at<ParticleEmitter>(i);
                if (m_pEmitterPool->isUsed(pEmitter))
                {
                    if (pEmitter->isAlive())
                    {
                        pEmitter->update();
                        if (!pEmitter->isAlive())
                        {
                            m_pEmitterPool->dealloc(pEmitter);
                        }
                    }
                    else
                    {
                        m_pEmitterPool->dealloc(pEmitter);
                    }
                }
            }
        }

        OPoolRef m_pEmitterPool;
        OPoolRef m_pParticlePool;
        Vector3 m_camRight;
        Vector3 m_camUp;
    };
}

// For quick stuff, we have shortcuts outside of the namespace
extern onut::ParticleSystemManager<>*   OParticles;

/*
inline void OLoadPFX(const char* pName)
{
OContentManager->addResource<OPfx>(pName);
}
*/
inline auto OEmitPFX(const char* pName, const Vector3& position, const Vector3& dir = Vector3::UnitZ) -> decltype(OParticles->emit(nullptr, position, dir))
{
    auto pPfx = OGetParticleSystem(pName);
    return OParticles->emit(pPfx, position, dir);
}

using OEmitterInstance = onut::ParticleSystemManager<>::EmitterInstance;
