#include <cassert>
#include <queue>
#include <mutex>

#include "onut.h"
#include "Random.h"
#include "Window.h"
#include "Audio.h"

using namespace DirectX;

// Our engine services
onut::Window*               g_pWindow = nullptr;
onut::Renderer*             ORenderer = nullptr;
onut::Settings*             OSettings = new onut::Settings();
onut::SpriteBatch*          OSB = nullptr;
onut::GamePad*              g_gamePads[4] = { nullptr };
onut::EventManager*         OEvent = nullptr;
onut::ContentManager*       OContentManager = nullptr;
AudioEngine*                g_pAudioEngine = nullptr;
onut::TimeInfo              g_timeInfo;

// Default resources
onut::BMFont*               g_pDefaultFont = nullptr;
onut::BMFont*               g_pDefaultFont64 = nullptr;

// So commonly used stuff
float                       ODT = 0.f;

namespace onut {
    std::vector<Color> palPinkLovers = { OColorHex(A59C98), OColorHex(F0C8D0), OColorHex(CC879C), OColorHex(D13162), OColorHex(322C2A) };
    std::vector<Color> palPeonyJoy = { OColorHex(002F43), OColorHex(0EACAB), OColorHex(A4BEBF), OColorHex(E27379), OColorHex(D3215D) };
    std::vector<Color> palWinterSun = { OColorHex(280617), OColorHex(38213E), OColorHex(745A8A), OColorHex(EF848A), OColorHex(FFEFBC) };
    std::vector<Color> palHeartDesire = { OColorHex(0C375B), OColorHex(456E86), OColorHex(6C9AA2), OColorHex(C35D61), OColorHex(FBF9AB) };
    std::vector<Color> palNatureWalk = { OColorHex(CFB590), OColorHex(9E9A41), OColorHex(758918), OColorHex(564334), OColorHex(49281F) };

    // For mainloop sync
    std::queue<ICallback*>   g_syncToMainLoopCallbacks;
    std::mutex               g_syncToMainLoopMutex;
    void synchronize() {
        std::lock_guard<std::mutex> lock(g_syncToMainLoopMutex);
        while (!g_syncToMainLoopCallbacks.empty()) {
            auto pCallback = g_syncToMainLoopCallbacks.front();
            if (pCallback) {
                pCallback->call();
                delete pCallback;
            }
            g_syncToMainLoopCallbacks.pop();
        }
    }

    // Main loop
    void run(std::function<void()> initCallback, std::function<void()> updateCallback, std::function<void()> renderCallback) {
        // Make sure we run just once
        static bool alreadyRan = false;
        assert(!alreadyRan);
        alreadyRan = true;

        //-------------------------- Create our services --------------------------------
        // 
        // Random
        randomizeSeed();

        // Events
        OEvent = new EventManager();

        // Window
        g_pWindow = new Window(OSettings->getResolution());

        // DirectX
        ORenderer = new Renderer(*g_pWindow);

        // SpriteBatch
        OSB = new SpriteBatch();

        // Content
        OContentManager = new ContentManager();

        // Fonts
        if (!OSettings->getDefaultFont().empty()) {
            const auto& fntFilename = OSettings->getDefaultFont();
            g_pDefaultFont = BMFont::createFromFile(fntFilename);
            g_pDefaultFont64 = BMFont::createFromFile(fntFilename.substr(0, fntFilename.find_last_of('.')) + "64.fnt");
        }

        // Gamepads
        for (int i = 0; i < 4; ++i) {
            g_gamePads[i] = new GamePad(i);
        }

        // Audio
#ifdef WIN32
        CoInitializeEx(nullptr, COINIT_MULTITHREADED);
#endif
        AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
        eflags = eflags | AudioEngine_Debug;
#endif
        g_pAudioEngine = new AudioEngine(eflags);
        //-------------------------------------------------------------------------------

        // Call the user defined init
        if (initCallback) {
            initCallback();
        }

        // Main loop
        MSG msg = { 0 };
        while (true) {
            if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                if (msg.message == WM_QUIT) {
                    break;
                }
            }

            // Sync to main callbacks
            synchronize();

            // Update
            g_pAudioEngine->Update();
            auto framesToUpdate = g_timeInfo.update();
            ODT = onut::getTimeInfo().getDeltaTime();
            while (framesToUpdate--) {
                for (auto& gamePad : g_gamePads) {
                    gamePad->update();
                }
                AnimManager::getGlobalManager()->update();
                OEvent->update();
                if (updateCallback) {
                    updateCallback();
                }
            }

            // Render
            ORenderer->beginFrame();
            if (renderCallback) {
                renderCallback();
            }
            ORenderer->endFrame();
        }
    }

    BMFont* getDefaultFont() {
        return g_pDefaultFont;
    }

    BMFont* getDefaultFontBig() {
        return g_pDefaultFont64;
    }

    GamePad* getGamePad(int index) {
        assert(index >= 0 && index <= 3);
        return g_gamePads[index];
    }

    const TimeInfo& getTimeInfo() {
        return g_timeInfo;
    }

    void syncCallbackToMainLoop(ICallback* pCallback) {
        g_syncToMainLoopCallbacks.push(pCallback);
    }
}
