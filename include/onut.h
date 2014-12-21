#pragma once
#include <functional>
#include <memory>
#include <future>
#include <queue>

#include "GamePad.h"
#include "BMFont.h"
#include "DefineHelpers.h"
#include "Random.h"
#include "Renderer.h"
#include "Settings.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "EventManager.h"
#include "TimeInfo.h"
#include "Anim.h"
#include "ContentManager.h"
#include "Sound.h"
#include "Callback.h"

using namespace DirectX::SimpleMath;

namespace onut {
    /**
        This should only be called once
        @param updateCallback Called at a fixed 60 times per seconds. Sometimes tho calls can be dropped and run slower if the game performs badly
        @param renderCallback Called once per frame. Draw your stuff in there
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
        Get the time info for the current frame
    */
    const TimeInfo& getTimeInfo();

    /**
        Rect alignement helper
    */
    template<Align Talign = Align::TOP_LEFT>
    Rect alignedRect(float xOffset, float yOffset, float width, float height, float padding = 0, Align align = Talign) {
        switch (align) {
        case Align::TOP_LEFT:
            xOffset += padding;
            yOffset += padding;
            width -= padding * 1.5f;
            height -= padding * 1.5f;
            return std::move(Rect{ xOffset, yOffset, width, height });
        case Align::TOP:
            yOffset += padding;
            width -= padding;
            height -= padding * 1.5f;
            return std::move(Rect{ OScreenCenterXf - width * .5f + xOffset, yOffset, width, height });
        case Align::TOP_RIGHT:
            xOffset += padding;
            yOffset += padding;
            width -= padding * 1.5f;
            height -= padding * 1.5f;
            return std::move(Rect{ OScreenWf - xOffset - width, yOffset, width, height });
        case Align::LEFT:
            xOffset += padding;
            width -= padding * 1.5f;
            height -= padding;
            return std::move(Rect{ xOffset, OScreenCenterYf - height * .5f + yOffset, width, height });
        case Align::CENTER:
            width -= padding;
            height -= padding;
            return std::move(Rect{ OScreenCenterXf - width * .5f + xOffset, OScreenCenterYf - height * .5f + yOffset, width, height });
        case Align::RIGHT:
            xOffset += padding;
            width -= padding * 1.5f;
            height -= padding;
            return std::move(Rect{ OScreenWf - xOffset - width, OScreenCenterYf - height * .5f + yOffset, width, height });
        case Align::BOTTOM_LEFT:
            xOffset += padding;
            yOffset += padding;
            width -= padding * 1.5f;
            height -= padding * 1.5f;
            return std::move(Rect{ xOffset, OScreenHf - yOffset - height, width, height });
        case Align::BOTTOM:
            yOffset += padding;
            width -= padding;
            height -= padding * 1.5f;
            return std::move(Rect{ OScreenCenterXf - width * .5f + xOffset, OScreenHf - yOffset - height, width, height });
        case Align::BOTTOM_RIGHT:
            xOffset += padding;
            yOffset += padding;
            width -= padding * 1.5f;
            height -= padding * 1.5f;
            return std::move(Rect{ OScreenWf - xOffset - width, OScreenHf - yOffset - height, width, height });
        }
        return std::move(Rect());
    }

    template<Align Talign = Align::TOP_LEFT>
    Rect alignedRect(const Rect& rect, float padding = 0, Align align = Talign) {
        alignedRect(rect.x, rect.y, rect.z, rect.w, padding, align);
    }


    /**
        Sync to the main loop. Call this from threads.
        If called from main loop, it will be called the next frame.
    */
    void syncCallbackToMainLoop(ICallback* pCallback);
    template<typename Tfn, typename ... Targs>
    void syncToMainLoop(Tfn callback, Targs... args) {
        auto pCallback = new Callback<Tfn, Targs...>(callback, args...);
        syncCallbackToMainLoop(pCallback);
    }
}

// For quick stuff, we have shortcuts outside of the namespace
extern onut::Renderer*      ORenderer;
extern onut::SpriteBatch*   OSpriteBatch;
extern onut::Settings*      OSettings;
extern onut::EventManager*  OEvent;

//--- Resource shortcuts
extern onut::ContentManager* OContentManager;

typedef onut::Texture   OTexture;
typedef onut::BMFont    OFont;
typedef onut::Sound     OSound;

inline OTexture* OGetTexture(const char* pName) {
    return OContentManager->getResource<OTexture>(pName);
}

inline OFont* OGetBMFont(const char* pName) {
    return OContentManager->getResource<OFont>(pName);
}

inline OSound* OGetSound(const char* pName) {
    return OContentManager->getResource<OSound>(pName);
}

inline void OPlaySound(const char* pName) {
    OGetSound(pName)->play();
}

//--- Game pads
inline onut::GamePad* OGamePad(int index) {
    return onut::getGamePad(index);
}

inline bool OPressed(onut::GamePad::eGamePad button, int gamePadIndex = 0) {
    return OGamePad(gamePadIndex)->isPressed(button);
}

inline bool OJustPressed(onut::GamePad::eGamePad button, int gamePadIndex = 0) {
    return OGamePad(gamePadIndex)->isJustPressed(button);
}

inline bool OJustReleased(onut::GamePad::eGamePad button, int gamePadIndex = 0) {
    return OGamePad(gamePadIndex)->isJustReleased(button);
}

inline const Vector2& OLThumb(int gamePadIndex = 0) {
    return OGamePad(gamePadIndex)->getLeftThumb();
}

inline const Vector2& ORThumb(int gamePadIndex = 0) {
    return OGamePad(gamePadIndex)->getRightThumb();
}

//--- Anims
typedef onut::Anim<float> OAnimf;
typedef onut::Anim<int> OAnimi;
typedef onut::Anim<Vector2> OAnim2;
typedef onut::Anim<Vector3> OAnim3;
typedef onut::Anim<Vector4> OAnim4;
typedef onut::Anim<std::string> OAnimStr;

//--- Timing
extern float ODT;

template<typename TtimeType>
inline void OSleep(const TtimeType& ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

template<typename Tfn, typename ... Targs>
inline auto OAsync(Tfn callback, Targs... args) -> decltype(std::async(callback, args...)) {
    return std::async(std::launch::async, callback, args...);
}

template<typename Tfn, typename ... Targs>
inline auto OSync(Tfn callback, Targs... args) -> decltype(callback(args...)) {
    onut::syncToMainLoop(callback, args...);
}
