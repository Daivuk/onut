// Onut
#include <onut/ParticleSystem.h>
#include <onut/ParticleSystemManager.h>
#include <onut/Pool.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>

// Private
#include "Particle.h"
#include "ParticleEmitter.h"

OParticleSystemManagerRef oParticleSystemManager;

namespace onut
{
    OParticleSystemManagerRef ParticleSystemManager::create(uintptr_t TmaxPFX, uintptr_t TmaxParticles, bool TsortEmitters)
    {
        return OMake<ParticleSystemManager>(TmaxPFX, TmaxParticles, false);
    }

    ParticleSystemManager::ParticleSystemManager(uintptr_t TmaxPFX, uintptr_t TmaxParticles, bool TsortEmitters)
        : m_sortEmitters(TsortEmitters)
    {
        m_pEmitterPool = OPool::create(sizeof(ParticleEmitter), TmaxPFX);
        m_pParticlePool = OPool::create(sizeof(Particle), TmaxParticles);
    }

    void ParticleSystemManager::EmitterInstance::setTransform(const Vector3& pos, const Vector3& dir, const Vector3& up)
    {
        Matrix transform = Matrix::CreateBillboard(pos, pos + up, dir);
        setTransform(transform);
    }

    void ParticleSystemManager::EmitterInstance::setRenderEnabled(bool renderEnabled)
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

    void ParticleSystemManager::EmitterInstance::setTransform(const Matrix& transform)
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

    void ParticleSystemManager::EmitterInstance::stop()
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

    bool ParticleSystemManager::EmitterInstance::isPlaying() const
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

    bool ParticleSystemManager::EmitterInstance::isAlive() const
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

    void ParticleSystemManager::EmitterInstance::render()
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

    ParticleSystemManager::EmitterInstance ParticleSystemManager::emit(const OParticleSystemRef& pParticleSystem, const Vector3& pos, const Vector3& dir)
    {
        static uint32_t nextId = 1;
        EmitterInstance instance;
        instance.m_id = nextId++;
        instance.m_pParticleSystemManager = this;

        Matrix transform = Matrix::CreateBillboard(pos, pos + dir, Vector3::UnitY);
        auto& emitters = pParticleSystem->getEmitters();
        for (auto& emitter : emitters)
        {
            auto pEmitter = m_pEmitterPool->alloc<ParticleEmitter>(emitter, OThis, transform, instance.m_id);
            // Update the first frame right away
            if (pEmitter) pEmitter->update();
        }

        return instance;
    }

    void ParticleSystemManager::clear()
    {
        m_pEmitterPool->clear();
        m_pParticlePool->clear();
    }

    void ParticleSystemManager::update()
    {
        updateEmitters();
    }

    bool ParticleSystemManager::hasAliveParticles() const
    {
        auto len = m_pEmitterPool->size();
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

    void ParticleSystemManager::render()
    {
        oSpriteBatch->begin();
        if (m_sortEmitters)
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

    Particle* ParticleSystemManager::allocParticle()
    {
        return m_pParticlePool->alloc<Particle>();
    }

    void ParticleSystemManager::deallocParticle(Particle* pParticle)
    {
        m_pParticlePool->dealloc(pParticle);
    }

    void ParticleSystemManager::renderParticle(Particle* pParticle, const Vector3& camRight, const Vector3& camUp)
    {
        float dim = static_cast<float>(std::max<decltype(pParticle->pTexture->getSize().x)>(pParticle->pTexture->getSize().x, pParticle->pTexture->getSize().y));
        oSpriteBatch->drawSprite(pParticle->pTexture, pParticle->position, pParticle->color.value, pParticle->angle.value, pParticle->size.value / dim);
    }

    void ParticleSystemManager::updateEmitters()
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
};

OEmitterInstance OEmitParticles(const char* pName, const Vector3& position, const Vector3& dir)
{
    auto pPfx = OGetParticleSystem(pName);
    return oParticleSystemManager->emit(pPfx, position, dir);
}
