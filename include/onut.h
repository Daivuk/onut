#pragma once
#include <functional>
#include <memory>

#include "Renderer.h"
#include "Settings.h"

namespace onut {
	/**
		This should only be called once
		@param updateCallback Called at a fixed 60 times per seconds. Sometimes tho calls can be dropped and run slower if the game performs badly
		@param renderCallback Called once per frame. Draw your stuff in there
	*/
	void run(std::function<void()> updateCallback, std::function<void()> renderCallback);

	/**
		Get Settings that allows you to set or get game settings like screen resolution, 
		name of the game, etc.
	*/
	std::shared_ptr<Settings> getSettings();

	/**
		Get the DX11 renderer.
	*/
	std::shared_ptr<Renderer> getRenderer();
}
