#include <cassert>

#include "onut.h"
#include "Random.h"
#include "Window.h"

namespace onut {
	std::vector<Color> palPinkLovers = { OColorHex(A59C98), OColorHex(F0C8D0), OColorHex(CC879C), OColorHex(D13162), OColorHex(322C2A) };
	std::vector<Color> palPeonyJoy = { OColorHex(002F43), OColorHex(0EACAB), OColorHex(A4BEBF), OColorHex(E27379), OColorHex(D3215D) };
	std::vector<Color> palWinterSun = { OColorHex(280617), OColorHex(38213E), OColorHex(745A8A), OColorHex(EF848A), OColorHex(FFEFBC) };
	std::vector<Color> palHeartDesire = { OColorHex(0C375B), OColorHex(456E86), OColorHex(6C9AA2), OColorHex(C35D61), OColorHex(FBF9AB) };
	std::vector<Color> palNatureWalk = { OColorHex(CFB590), OColorHex(9E9A41), OColorHex(758918), OColorHex(564334), OColorHex(49281F) };

	// Our engine services
	std::shared_ptr<Window> g_pWindow;
	std::shared_ptr<Renderer> g_pRenderer;
	std::shared_ptr<Settings> g_pSettings;
	std::shared_ptr<SpriteBatch> g_pSpriteBatch;
	std::shared_ptr<BMFont> g_pDefaultFont;
	std::shared_ptr<BMFont> g_pDefaultFont64;
	std::shared_ptr<GamePad> g_gamePads[4] = { nullptr };

	// Main loop
	void run(std::function<void()> initCallback, std::function<void()> updateCallback, std::function<void()> renderCallback) {
		// Validate parameters
		assert(initCallback);
		assert(updateCallback);
		assert(renderCallback);

		// Make sure we run just once
		static bool alreadyRan = false;
		assert(!alreadyRan);
		alreadyRan = true;

		// Create our services
		randomizeSeed();
		auto settings = getSettings();
		g_pWindow = std::make_shared<Window>(settings->getResolution());
		g_pRenderer = std::make_shared<Renderer>(*g_pWindow);
		g_pSpriteBatch = std::make_shared<SpriteBatch>();
		if (!settings->getDefaultFont().empty()) {
			const auto& fntFilename = settings->getDefaultFont();
			g_pDefaultFont = BMFont::createFromFile(fntFilename);
			g_pDefaultFont64 = BMFont::createFromFile(fntFilename.substr(0, fntFilename.find_last_of('.')) + "64.fnt");
		}
		for (int i = 0; i < 4; ++i) {
			g_gamePads[i] = std::make_shared<GamePad>(i);
		}

		initCallback();

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

			// Update
			for (auto& gamePad : g_gamePads) {
				gamePad->update();
			}
			updateCallback();

			// Render
			g_pRenderer->beginFrame();
			renderCallback();
			g_pRenderer->endFrame();
		}
	}

	// Getters
	std::shared_ptr<Settings> getSettings() {
		if (g_pSettings == nullptr) {
			g_pSettings = std::make_shared<Settings>();
		}
		return g_pSettings;
	}

	std::shared_ptr<Renderer> getRenderer() {
		assert(g_pRenderer); // You have to call onut::run before!
		return g_pRenderer;
	}

	std::shared_ptr<SpriteBatch> getSpriteBatch() {
		assert(g_pSpriteBatch); // You have to call onut::run before!
		return g_pSpriteBatch;
	}

	std::shared_ptr<BMFont> getDefaultFont() {
		return g_pDefaultFont;
	}

	std::shared_ptr<BMFont> getDefaultFontBig() {
		return g_pDefaultFont64;
	}

	std::shared_ptr<GamePad> getGamePad(int index) {
		assert(index >= 0 && index <= 3);
		return g_gamePads[index];
	}
}
