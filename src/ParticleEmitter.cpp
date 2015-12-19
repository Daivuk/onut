#include "onut.h"
#include "ParticleSystemManager.h"

namespace onut
{
    ParticleEmitter::ParticleEmitter(sEmitterDesc* pEmitterDesc,
                                     IParticleSystemManager* pParticleSystemManager, 
                                     const Matrix& transform,
                                     uint32_t instanceId) :
        m_pDesc(pEmitterDesc),
        m_pParticleSystemManager(pParticleSystemManager),
        m_transform(transform),
        m_isAlive(true),
        m_instanceId(instanceId)
    {
        if (m_pDesc->type == eEmitterType::BURST)
        {
            // Spawn them all!
            for (decltype(m_pDesc->count) i = 0; i < m_pDesc->count; ++i)
            {
                spawnParticle();
            }
        }
    }

    ParticleEmitter::~ParticleEmitter()
    {
        auto pParticle = m_pParticles;
        while (pParticle)
        {
            pParticle = m_pParticleSystemManager->deallocParticle(pParticle);
        }
    }

    void ParticleEmitter::stop()
    {
        m_isStopped = true;
    }

    void ParticleEmitter::update()
    {
        // Update current particles
        for (auto pParticle = m_pParticles; pParticle; )
        {
            pParticle->update();
            if (!pParticle->isAlive())
            {
                if (m_pParticles == pParticle)
                {
                    pParticle = m_pParticleSystemManager->deallocParticle(pParticle);
                }
                else
                {
                    m_pParticles = pParticle = m_pParticleSystemManager->deallocParticle(pParticle);
                }
                continue;
            }
            pParticle = pParticle->pNext;
        }

        // Spawn at rate
        if (m_pDesc->type == eEmitterType::CONTINOUS && m_pDesc->rate > 0 && !m_isStopped)
        {
            m_rateProgress += ODT;
            auto rate = 1.0f / static_cast<float>(m_pDesc->rate);
            while (m_rateProgress >= rate)
            {
                m_rateProgress -= rate;
                spawnParticle();
            }
        }

        // Kill self if done
        if (m_pDesc->type == eEmitterType::BURST && !m_pParticles)
        {
            m_isAlive = false;
        }
    }

    void ParticleEmitter::render()
    {
        auto& camDir = ORenderer->getCameraDir();
        auto& camUp = ORenderer->getCameraUp();
        auto camRight = camDir.Cross(camUp);

        for (auto pParticle = m_pParticles; pParticle; pParticle = pParticle->pNext)
        {
            m_pParticleSystemManager->renderParticle(pParticle, camRight, camUp);
        }
    }

    void ParticleEmitter::setTransform(const Matrix& transform)
    {
        m_transform = transform;
    }

    Particle* ParticleEmitter::spawnParticle()
    {
        auto pParticle = m_pParticleSystemManager->allocParticle();
        if (pParticle)
        {
            Vector3 spawnPos = m_transform.Translation();
            Vector3 up = m_transform.AxisZ();
            Vector3 right = m_transform.AxisX();

            auto randomAngleX = m_pDesc->spread.generateFrom() * .5f;
            auto randomAngleZ = randf(0, 360.f);

            Matrix rotX = Matrix::CreateFromAxisAngle(right, DirectX::XMConvertToRadians(randomAngleX));
            Matrix rotZ = Matrix::CreateFromAxisAngle(up, DirectX::XMConvertToRadians(randomAngleZ));

            up = Vector3::Transform(up, rotX);
            up = Vector3::Transform(up, rotZ);

            pParticle->pDesc = m_pDesc;

            pParticle->position = spawnPos + m_pDesc->position.generate();
            pParticle->velocity = up * m_pDesc->speed.generate();

            pParticle->color.to = m_pDesc->color.generateTo(pParticle->color.from = m_pDesc->color.generateFrom());
            pParticle->angle.to = m_pDesc->angle.generateTo(pParticle->angle.from = m_pDesc->angle.generateFrom());
            pParticle->size.to = m_pDesc->size.generateTo(pParticle->size.from = m_pDesc->size.generateFrom());
            pParticle->image_index.to = m_pDesc->image_index.generateTo(pParticle->image_index.from = m_pDesc->image_index.generateFrom());

            if (!pParticle->pDesc->textures.empty())
            {
                pParticle->pTexture = pParticle->pDesc->textures[static_cast<decltype(pParticle->pDesc->textures.size())>(pParticle->image_index.value)];
            }

            pParticle->life = 1.f;
            pParticle->delta = 1.f / m_pDesc->life.generate();

            if (m_pParticles)
            {
                for (auto pAliveParticle = m_pParticles; pAliveParticle; pAliveParticle = pAliveParticle->pNext)
                {
                    if (!pAliveParticle->pNext)
                    {
                        pAliveParticle->pNext = pParticle;
                        break;
                    }
                }
            }
            else
            {
                m_pParticles = pParticle;
            }
        }
        return pParticle;
    }
}
