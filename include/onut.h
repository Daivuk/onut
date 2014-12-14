#pragma once
#include <functional>
#include <memory>

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

using namespace DirectX::SimpleMath;

namespace onut {
	/**
		This should only be called once
		@param updateCallback Called at a fixed 60 times per seconds. Sometimes tho calls can be dropped and run slower if the game performs badly
		@param renderCallback Called once per frame. Draw your stuff in there
	*/
	void run(std::function<void()> initCallback, std::function<void(const TimeInfo&)> updateCallback, std::function<void()> renderCallback);

	/**
		Get Settings that allows you to set or get game settings like screen resolution, 
		name of the game, etc.
	*/
	std::shared_ptr<Settings> getSettings();

	/**
		Get the DX11 renderer.
	*/
	std::shared_ptr<Renderer> getRenderer();

	/**
		Get a global SpriteBatch object to draw your 2D.
		Note, you can create your own SpriteBatch objects if you like
	*/
	std::shared_ptr<SpriteBatch> getSpriteBatch();

	/**
		Get a global font. This is the default font set in the settings.
		If file not found it will return nullptr
	*/
	std::shared_ptr<BMFont> getDefaultFont();

	/**
		This is the same as getDefaultFont but gets a bigger version
	*/
	std::shared_ptr<BMFont> getDefaultFontBig();

	/**
		Get a gamepad for index (0 to 3)
	*/
	std::shared_ptr<GamePad> getGamePad(int index);

	/**
		Get a gamepad for index (0 to 3)
	*/
	std::shared_ptr<EventManager> getEventManager();

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
	void syncToMainLoop(const std::function<void()>& callback);
}
