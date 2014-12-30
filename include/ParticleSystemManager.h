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
        void emit(ParticleSystem* pParticleSystem, const Vector3& pos, const Vector3& dir = Vector3::UnitZ)
        {
            Matrix transform = Matrix::CreateBillboard(pos, pos + dir, Vector3::UnitY);
            for (auto& emitter : pParticleSystem->emitters)
            {
                auto pEmitter = m_emitterPool.alloc<ParticleEmitter>(&emitter, this, transform);
                // Update the first frame right away
                pEmitter->update();
            }
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
            OSB->begin();
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
            OSB->end();
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
            OSB->drawSprite(pParticle->pTexture, pParticle->position, pParticle->color.value, pParticle->angle.value, pParticle->size.value / dim);
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

        Pool<sizeof(ParticleEmitter), TmaxPFX, sizeof(uintptr_t), false>    m_emitterPool;
        Pool<sizeof(Particle), TmaxParticles, sizeof(uintptr_t), false>     m_particlePool;
        Vector3                                                             m_camRight;
        Vector3                                                             m_camUp;
        
        // Sprite batch stuff
        ID3D11Buffer*               m_pVertexBuffer = nullptr;
        ID3D11Buffer*               m_pIndexBuffer = nullptr;
        D3D11_MAPPED_SUBRESOURCE    m_pMappedVertexBuffer;
    };
}
