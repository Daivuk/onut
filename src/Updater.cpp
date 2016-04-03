// Onut
#include <onut/Updater.h>

OUpdaterRef oUpdater;

namespace onut
{
    UpdateTarget::UpdateTarget()
    {
    }

    UpdateTarget::UpdateTarget(const UpdateTarget& other)
    {
        if (other.m_isRegister)
        {
            other.m_pUpdater->registerTarget(this);
        }
    }

    UpdateTarget::~UpdateTarget()
    {
        if (m_isRegister && m_pUpdater)
        {
            m_pUpdater->unregisterTarget(this);
        }
    }

    bool UpdateTarget::isUpdateTargetRegistered() const
    {
        return m_isRegister;
    }

    const OUpdaterRef& UpdateTarget::getUpdater() const
    {
        return m_pUpdater;
    }

    OUpdaterRef Updater::create()
    {
        return std::shared_ptr<Updater>(new Updater());
    }

    void Updater::update()
    {
        for (auto& pTarget : m_toAdd) m_targets.push_back(pTarget);
        m_toAdd.clear();

        for (size_t i = 0; i < m_targets.size();)
        {
            auto pTarget = m_targets[i];
            if (pTarget)
            {
                pTarget->update();
            }
            else
            {
                m_targets.erase(m_targets.begin() + i);
                continue;
            }
            ++i;
        }

        for (auto& pTarget : m_toAdd)
        {
            if (!pTarget) continue;
            m_targets.push_back(pTarget);
        }
        m_toAdd.clear();
    }

    void Updater::registerTarget(OUpdateTarget* pTarget)
    {
        if (pTarget->m_isRegister)
        {
            if (pTarget->m_pUpdater.get() == this) return;
            pTarget->m_pUpdater->unregisterTarget(pTarget);
        }
        pTarget->m_pUpdater = shared_from_this();
        pTarget->m_isRegister = true;
        m_toAdd.push_back(pTarget);
    }

    void Updater::unregisterTarget(OUpdateTarget* pTarget)
    {
        if (!pTarget->m_isRegister) return;
        if (pTarget->m_pUpdater.get() != this) return;
        pTarget->m_pUpdater = nullptr;
        pTarget->m_isRegister = false;

        for (size_t i = 0; i < m_toAdd.size(); ++i)
        {
            if (m_toAdd[i] == pTarget)
            {
                m_toAdd[i] = nullptr;
            }
        }
        for (size_t i = 0; i < m_targets.size(); ++i)
        {
            if (m_targets[i] == pTarget)
            {
                m_targets[i] = nullptr;
            }
        }
    }
}
