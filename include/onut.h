#pragma once
#include "Asynchronous.h"
#include "BMFont.h"
#include "ContentManager.h"
#include "DefineHelpers.h"
#include "EventManager.h"
#include "GamePad.h"
#include "ParticleSystemManager.h"
#include "RectUtils.h"
#include "Renderer.h"
#include "Settings.h"
#include "Sound.h"
#include "SpriteBatch.h"
#include "State.h"
#include "Synchronous.h"
#include "TimingUtils.h"
#include "UINodeNav.h"

typedef onut::Anim<float>       OAnimf;
typedef onut::Anim<int>         OAnimi;
typedef onut::Anim<Vector2>     OAnim2;
typedef onut::Anim<Vector3>     OAnim3;
typedef onut::Anim<Vector4>     OAnim4;
typedef onut::Anim<std::string> OAnimStr;
typedef onut::Anim<Color>       OAnimc;
typedef onut::Timer<float>      OTimer;
typedef std::vector<Color>      OPal;

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
    Get a global font. This is the default font set in the settings.
    If file not found it will return nullptr
    */
    BMFont* getDefaultFont();

    /**
    This is the same as getDefaultFont but gets a bigger version
    */
    BMFont* getDefaultFontBig();

    /**
    Get a gamepad for index (0 to 3)
    */
    GamePad* getGamePad(int index);

    /**
    Debug tool to draw a palette and show it's index in it
    */
    void drawPal(const OPal& pal);
}

// For quick stuff, we have shortcuts outside of the namespace
extern onut::Renderer*                  ORenderer;
extern onut::SpriteBatch*               OSB;
extern onut::Settings*                  OSettings;
extern onut::EventManager*              OEvent;
extern onut::ParticleSystemManager<>*   OParticles;

//--- Resource shortcuts
extern onut::ContentManager<>* OContentManager;

inline OTexture* OGetTexture(const char* pName)
{
    return OContentManager->getResource<OTexture>(pName);
}

inline OFont* OGetBMFont(const char* pName)
{
    return OContentManager->getResource<OFont>(pName);
}

inline OSound* OGetSound(const char* pName)
{
    return OContentManager->getResource<OSound>(pName);
}

inline OPfx* OGetPFX(const char* pName)
{
    return OContentManager->getResource<OPfx>(pName);
}

inline void OPlaySound(const char* pName)
{
    OGetSound(pName)->play();
}

//--- Game pads
inline onut::GamePad* OGamePad(int index)
{
    return onut::getGamePad(index);
}

inline bool OPressed(onut::GamePad::eGamePad button, int gamePadIndex = 0)
{
    return OGamePad(gamePadIndex)->isPressed(button);
}

inline bool OJustPressed(onut::GamePad::eGamePad button, int gamePadIndex = 0)
{
    return OGamePad(gamePadIndex)->isJustPressed(button);
}

inline bool OJustReleased(onut::GamePad::eGamePad button, int gamePadIndex = 0)
{
    return OGamePad(gamePadIndex)->isJustReleased(button);
}

inline const Vector2& OLThumb(int gamePadIndex = 0)
{
    return OGamePad(gamePadIndex)->getLeftThumb();
}

inline const Vector2& ORThumb(int gamePadIndex = 0)
{
    return OGamePad(gamePadIndex)->getRightThumb();
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
    extern onut::Synchronous<onut::Pool<>> g_mainSync;
    g_mainSync.sync(callback, args...);
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
