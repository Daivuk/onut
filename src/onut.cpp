#include <cassert>
#include <queue>
#include <mutex>

#include "onut.h"
#include "Random.h"
#include "Window.h"
#include "Audio.h"

using namespace DirectX;

namespace onut {
    std::vector<Color> palPinkLovers = { OColorHex(A59C98), OColorHex(F0C8D0), OColorHex(CC879C), OColorHex(D13162), OColorHex(322C2A) };
    std::vector<Color> palPeonyJoy = { OColorHex(002F43), OColorHex(0EACAB), OColorHex(A4BEBF), OColorHex(E27379), OColorHex(D3215D) };
    std::vector<Color> palWinterSun = { OColorHex(280617), OColorHex(38213E), OColorHex(745A8A), OColorHex(EF848A), OColorHex(FFEFBC) };
    std::vector<Color> palHeartDesire = { OColorHex(0C375B), OColorHex(456E86), OColorHex(6C9AA2), OColorHex(C35D61), OColorHex(FBF9AB) };
    std::vector<Color> palNatureWalk = { OColorHex(CFB590), OColorHex(9E9A41), OColorHex(758918), OColorHex(564334), OColorHex(49281F) };

    // For mainloop sync
    std::queue<std::function<void()>>   g_syncToMainLoopCallbacks;
    std::mutex                          g_syncToMainLoopMutex;
    void synchronize() {
        std::lock_guard<std::mutex> lock(g_syncToMainLoopMutex);
        while (!g_syncToMainLoopCallbacks.empty()) {
            auto& callback = g_syncToMainLoopCallbacks.front();
            if (callback) callback();
            g_syncToMainLoopCallbacks.pop();
        }
    }

    // Our engine services
    Window*             g_pWindow = nullptr;
    Renderer*           g_pRenderer = nullptr;
    Settings*           g_pSettings = nullptr;
    SpriteBatch*        g_pSpriteBatch = nullptr;
    BMFont*             g_pDefaultFont = nullptr;
    BMFont*             g_pDefaultFont64 = nullptr;
    GamePad*            g_gamePads[4] = { nullptr };
    EventManager*       g_pEventManager = nullptr;
    ContentManager*     g_pContentManager = nullptr;
    AudioEngine*        g_pAudioEngine = nullptr;

    // Main loop
    void run(std::function<void()> initCallback, std::function<void(const TimeInfo&)> updateCallback, std::function<void()> renderCallback) {
        // Make sure we run just once
        static bool alreadyRan = false;
        assert(!alreadyRan);
        alreadyRan = true;

        //-------------------------- Create our services --------------------------------
        // 
        // Random
        randomizeSeed();

        // Settings
        auto settings = getSettings();

        // Events
        g_pEventManager = new EventManager();

        // Window
        g_pWindow = new Window(settings->getResolution());

        // DirectX
        g_pRenderer = new Renderer(*g_pWindow);

        // SpriteBatch
        g_pSpriteBatch = new SpriteBatch();

        // Content
        g_pContentManager = new ContentManager();

        // Fonts
        if (!settings->getDefaultFont().empty()) {
            const auto& fntFilename = settings->getDefaultFont();
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

        TimeInfo timeInfo;

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
            auto framesToUpdate = timeInfo.update();
            while (framesToUpdate--) {
                for (auto& gamePad : g_gamePads) {
                    gamePad->update();
                }
                AnimManager::getGlobalManager()->update();
                g_pEventManager->update();
                if (updateCallback) {
                    updateCallback(timeInfo);
                }
            }

            // Render
            g_pRenderer->beginFrame();
            if (renderCallback) {
                renderCallback();
            }
            g_pRenderer->endFrame();
        }
    }

    Settings* getSettings() {
        if (g_pSettings == nullptr) {
            g_pSettings = new Settings();
        }
        return g_pSettings;
    }

    Renderer* getRenderer() {
        assert(g_pRenderer); // You have to call onut::run before!
        return g_pRenderer;
    }

    SpriteBatch* getSpriteBatch() {
        assert(g_pSpriteBatch); // You have to call onut::run before!
        return g_pSpriteBatch;
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

    EventManager* getEventManager() {
        return g_pEventManager;
    }

    ContentManager* getContentManager() {
        return g_pContentManager;
    }

    void syncToMainLoop(const std::function<void()>& callback) {
        std::lock_guard<std::mutex> lock(g_syncToMainLoopMutex);
        g_syncToMainLoopCallbacks.push(callback);
    }
}
