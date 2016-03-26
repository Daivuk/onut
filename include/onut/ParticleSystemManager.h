#pragma once
// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ParticleSystem)
OForwardDeclare(ParticleSystemManager)
OForwardDeclare(Pool)

namespace onut
{
    class Particle;

    class ParticleSystemManager : public std::enable_shared_from_this<ParticleSystemManager>
    {
    public:
        static OParticleSystemManagerRef create(uintptr_t TmaxPFX = 100, uintptr_t TmaxParticles = 2000, bool TsortEmitters = false);

        ParticleSystemManager(uintptr_t TmaxPFX = 100, uintptr_t TmaxParticles = 2000, bool TsortEmitters = false);

        class EmitterInstance
        {
        public:
            void setTransform(const Vector3& pos, const Vector3& dir = Vector3::UnitY, const Vector3& up = Vector3::UnitZ);
            void setRenderEnabled(bool renderEnabled);
            void setTransform(const Matrix& transform);
            void stop();
            bool isPlaying() const;
            bool isAlive() const;
            void render();

        private:
            friend class ParticleSystemManager;

            uint32_t m_id = 0;
            ParticleSystemManager* m_pParticleSystemManager = nullptr;
            bool m_bStopped = false;
        };

        EmitterInstance emit(const OParticleSystemRef& pParticleSystem, const Vector3& pos, const Vector3& dir = Vector3::UnitZ);
        void clear();
        void update();
        bool hasAliveParticles() const;
        void render();

        Particle* allocParticle();
        void deallocParticle(Particle* pParticle);
        void renderParticle(Particle* pParticle, const Vector3& camRight, const Vector3& camUp);

    private:
        friend class EmitterInstance;

        void updateEmitters();

        OPoolRef m_pEmitterPool;
        OPoolRef m_pParticlePool;
        Vector3 m_camRight;
        Vector3 m_camUp;
        bool m_sortEmitters;
    };
}

extern OParticleSystemManagerRef oParticleSystemManager;

using OEmitterInstance = OParticleSystemManager::EmitterInstance;

OEmitterInstance OEmitParticles(const char* pName, const Vector3& position, const Vector3& dir = Vector3::UnitZ);
