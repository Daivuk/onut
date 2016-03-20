#pragma once
#include "onut/ParticleSystem.h"

#include "DefineHelpers.h"
#include "onutUI.h"
#include "ParticleSystemManager.h"
#include "PrimitiveBatch.h"
#include "RectUtils.h"
#include "Synchronous.h"
#include "TimingUtils.h"
#include "Window.h"

// For quick stuff, we have shortcuts outside of the namespace
extern onut::PrimitiveBatch*            OPrimitiveBatch;
extern onut::ParticleSystemManager<>*   OParticles;
extern onut::Window*                    OWindow;
extern onut::UIControl*                 OUI;
extern onut::UIContext*                 OUIContext;

#define OPB OPrimitiveBatch

namespace onut
{
    /**
    This should only be called once
    @param initCallback Called once after the engine has been initialized
    @param updateCallback Called at a fixed 120 times per seconds. Sometimes tho calls can be dropped and run slower if the game performs badly
    @param renderCallback Called once per frame. Draw your stuff in there
    @retun This function never returns until the game shutsdown.
    */
    void run(std::function<void()> initCallback, std::function<void()> updateCallback, std::function<void()> renderCallback);

    /**
    * Call run with a State object
    * Any object that defines an init, update and render call will work also
    */
    template<typename TstateType>
    void run(TstateType& stateManager)
    {
        run(std::bind(&TstateType::init, &stateManager),
            std::bind(&TstateType::update, &stateManager),
            std::bind(&TstateType::render, &stateManager));
    }
}

#define ORun onut::run

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

inline onut::UIControl* OLoadUI(const std::string& name)
{
    std::string filename = "../../assets/ui/" + name;
    return new onut::UIControl(filename.c_str());
}

inline Vector4 ORectLocalToWorld(const Vector4& local, const Vector4& parent)
{
    auto ret = local;
    ret.x *= parent.z;
    ret.y *= parent.w;
    ret.x += parent.x;
    ret.y += parent.y;
    ret.z *= parent.z;
    ret.w *= parent.w;
    return std::move(ret);
}

inline Vector4 ORectWorldToLocal(const Vector4& world, const Vector4& parent)
{
    auto ret = world;
    ret.x -= parent.x;
    ret.y -= parent.y;
    ret.x /= parent.z;
    ret.y /= parent.w;
    ret.z /= parent.z;
    ret.w /= parent.w;
    return std::move(ret);
}

inline Vector4 OUVS(OTexture* pTexture, const Rect &rect)
{
    auto texDim = pTexture->getSizef();
    return{
        rect.x / texDim.x,
        rect.y / texDim.y,
        (rect.x + rect.z) / texDim.x,
        (rect.y + rect.w) / texDim.y
    };
}

inline void OQuit()
{
    PostQuitMessage(0);
}

inline onut::UIControl* OFindUI(const std::string& name)
{
    return OUI->getChild(name);
}

/**
Synchronize back to main thread. This can also be called from the main thread. It will just be delayed until the next frame.
@param callback Function or your usual lambda
@param args arguments
*/
template<typename Tfn,
    typename ... Targs>
    inline void OSync(Tfn callback, Targs... args)
{
    extern onut::Synchronous g_mainSync;
    g_mainSync.sync(callback, args...);
}
