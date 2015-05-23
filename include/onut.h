#pragma once
#include "Asynchronous.h"
#include "BMFont.h"
#include "ContentManager.h"
#include "crypto.h"
#include "DefineHelpers.h"
#include "EventManager.h"
#include "http.h"
#include "Input.h"
#include "GamePad.h"
#include "ParticleSystemManager.h"
#include "PrimitiveBatch.h"
#include "RectUtils.h"
#include "Renderer.h"
#include "RTS.h"
#include "Settings.h"
#include "Sound.h"
#include "SpriteBatch.h"
#include "StateManager.h"
#include "Synchronous.h"
#include "TiledMap.h"
#include "TimingUtils.h"
#include "onutUI.h"
#include "UINodeNav.h"
#include "Window.h"

typedef onut::Anim<float>       OAnimf;
typedef onut::Anim<int>         OAnimi;
typedef onut::Anim<Vector2>     OAnim2;
typedef onut::Anim<Vector3>     OAnim3;
typedef onut::Anim<Vector4>     OAnim4;
typedef onut::Anim<std::string> OAnimStr;
typedef onut::Anim<Color>       OAnimc;
typedef onut::Timer<float>      OTimer;
typedef std::vector<Color>      OPal;

// For quick stuff, we have shortcuts outside of the namespace
extern onut::Renderer*                  ORenderer;
extern onut::SpriteBatch*               OSB;
extern onut::PrimitiveBatch*            OPB;
extern onut::Settings*                  OSettings;
extern onut::EventManager*              OEvent;
extern onut::ParticleSystemManager<>*   OParticles;
extern Vector2                          OMousePos;
extern onut::Input*                     OInput;
extern onut::Window*                    OWindow;

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

    /**
    Get a gamepad for index (0 to 3)
    */
    GamePad* getGamePad(int index);

    /**
    Debug tool to draw a palette and show it's index in it
    */
    void drawPal(const OPal& pal, OFont* pFont = nullptr);
}

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
