// Onut
#include <onut/ParticleSystem.h>
#include <onut/ParticleSystemManager.h>
#include <onut/Timing.h>

// Private
#include "Particle.h"
#include "ParticleEmitter.h"

namespace onut
{
    ParticleEmitter::ParticleEmitter(const OParticleEmitterDescRef& pEmitterDesc,
                                     const OParticleSystemManagerRef& pParticleSystemManager,
                                     const Matrix& transform,
                                     uint32_t instanceId) :
        m_pDesc(pEmitterDesc),
        m_pParticleSystemManager(pParticleSystemManager),
        m_transform(transform),
        m_isAlive(true),
        m_instanceId(instanceId)
    {
        m_duration = m_pDesc->duration.generate();
        if (m_pDesc->type == ParticleEmitterDesc::Type::BURST)
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
        for (auto pParticle : m_particles)
        {
            m_pParticleSystemManager->deallocParticle(pParticle);
        }
        m_particles.clear();
    }

    void ParticleEmitter::stop()
    {
        m_isStopped = true;
    }

    void ParticleEmitter::update()
    {
        // Update current particles
        for (decltype(m_particles.size()) i = 0; i < m_particles.size();)
        {
            auto pParticle = m_particles[i];
            pParticle->update();
            if (!pParticle->isAlive())
            {
                m_pParticleSystemManager->deallocParticle(pParticle);
                m_particles.erase(m_particles.begin() + i);
                continue;
            }
            ++i;
        }

        // Spawn at rate
        if (m_pDesc->type == ParticleEmitterDesc::Type::CONTINOUS && m_pDesc->rate > 0 && !m_isStopped)
        {
            m_rateProgress += ODT;
            auto rate = 1.0f / m_pDesc->rate;
            while (m_rateProgress >= rate)
            {
                m_rateProgress -= rate;
                spawnParticle();
            }
        }

        if (m_pDesc->type == ParticleEmitterDesc::Type::CONTINOUS && m_isStopped)
        {
            if (m_particles.empty()) m_isAlive = false;
        }

        if (m_pDesc->type == ParticleEmitterDesc::Type::FINITE && m_pDesc->rate > 0 && !m_isStopped && m_duration > 0.f)
        {
            m_duration -= ODT;
            m_rateProgress += ODT;
            auto rate = 1.0f / m_pDesc->rate;
            while (m_rateProgress >= rate)
            {
                m_rateProgress -= rate;
                spawnParticle();
            }
        }

        if (m_pDesc->type == ParticleEmitterDesc::Type::FINITE && (m_isStopped || m_duration <= 0.f))
        {
            if (m_particles.empty()) m_isAlive = false;
        }

        // Kill self if done
        if (m_pDesc->type == ParticleEmitterDesc::Type::BURST && m_particles.empty())
        {
            m_isAlive = false;
        }
    }

    void ParticleEmitter::render()
    {
        auto camDir = -Vector3::UnitZ;
        auto camUp = -Vector3::UnitY;
        auto camRight = camDir.Cross(camUp);

        for (decltype(m_particles.size()) i = 0; i < m_particles.size(); ++i)
        {
            auto pParticle = m_particles[i];
            if (pParticle->delay <= 0)
            {
                m_pParticleSystemManager->renderParticle(pParticle, camRight, camUp);
            }
        }
    }

    void ParticleEmitter::setTransform(const Matrix& transform)
    {
        m_transform = transform;
    }

    void ParticleEmitter::setRenderEnabled(bool renderEnabled)
    {
        m_renderEnabled = renderEnabled;
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

            Matrix rotX = Matrix::CreateFromAxisAngle(right, OConvertToRadians(randomAngleX));
            Matrix rotZ = Matrix::CreateFromAxisAngle(up, OConvertToRadians(randomAngleZ));

            up = Vector3::Transform(up, rotX);
            up = Vector3::Transform(up, rotZ);
            if (m_pDesc->dir.from.LengthSquared() != 0)
            {
                Matrix rotDir = Matrix::CreateFromAxisAngle(Vector3(m_pDesc->dir.from.y, m_pDesc->dir.from.x, 0), OConvertToRadians(90));
                up = Vector3::Transform(up, rotDir);
            }

            pParticle->pDesc = m_pDesc;
            pParticle->pEmitter = this;

            pParticle->position = spawnPos + m_pDesc->position.generate();

            pParticle->velocity.from = up * m_pDesc->speed.generateFrom();
            pParticle->velocity.to = up * m_pDesc->speed.generateTo();
            pParticle->color.to = m_pDesc->color.generateTo(pParticle->color.from = m_pDesc->color.generateFrom());
            pParticle->angle.to = m_pDesc->angle.generateTo(pParticle->angle.from = m_pDesc->angle.generateFrom());
            pParticle->size.to = m_pDesc->size.generateTo(pParticle->size.from = m_pDesc->size.generateFrom());
            pParticle->image_index.to = m_pDesc->image_index.generateTo(pParticle->image_index.from = m_pDesc->image_index.generateFrom());
            pParticle->rotation.to = m_pDesc->rotation.generateTo(pParticle->rotation.from = m_pDesc->rotation.generateFrom());
            pParticle->radialAccel.to = m_pDesc->radialAccel.generateTo(pParticle->radialAccel.from = m_pDesc->radialAccel.generateFrom());
            pParticle->tangentAccel.to = m_pDesc->tangentAccel.generateTo(pParticle->tangentAccel.from = m_pDesc->tangentAccel.generateFrom());
            pParticle->gravity.to = m_pDesc->gravity.generateTo(pParticle->gravity.from = m_pDesc->gravity.generateFrom());

            pParticle->velocity.tween = m_pDesc->speed.tween;
            pParticle->gravity.tween = m_pDesc->gravity.tween;
            pParticle->color.tween = m_pDesc->color.tween;
            pParticle->angle.tween = m_pDesc->angle.tween;
            pParticle->size.tween = m_pDesc->size.tween;
            pParticle->image_index.tween = m_pDesc->image_index.tween;
            pParticle->rotation.tween = m_pDesc->rotation.tween;
            pParticle->radialAccel.tween = m_pDesc->radialAccel.tween;
            pParticle->tangentAccel.tween = m_pDesc->tangentAccel.tween;

            pParticle->velocity.update(0);
            pParticle->gravity.update(0);
            pParticle->color.update(0);
            pParticle->angle.update(0);
            pParticle->size.update(0);
            pParticle->image_index.update(0);
            pParticle->rotation.update(0);
            pParticle->radialAccel.update(0);
            pParticle->tangentAccel.update(0);

            if (!pParticle->pDesc->textures.empty())
            {
                pParticle->pTexture = pParticle->pDesc->textures[static_cast<decltype(pParticle->pDesc->textures.size())>(pParticle->image_index.value)];
            }

            pParticle->life = 1.f;
            pParticle->delay = m_pDesc->delay.generate();
            pParticle->delta = 1.f / m_pDesc->life.generate();

            m_particles.push_back(pParticle);
        }
        return pParticle;
    }
}
