#include <cassert>

#include "onut.h"
#include "Random.h"
#include "Window.h"

namespace onut {
	// Our engine services
	std::shared_ptr<Window> g_pWindow;
	std::shared_ptr<Renderer> g_pRenderer;
	std::shared_ptr<Settings> g_pSettings;
	std::shared_ptr<SpriteBatch> g_pSpriteBatch;

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
}
