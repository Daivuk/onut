#pragma once
#include "Typedefs.h"

//--- Some shortcuts
#define ORun				onut::run
#define ORenderer			onut::getRenderer()
#define OSpriteBatch		onut::getSpriteBatch()
#define OSettings			onut::getSettings()
#define OFont				onut::getDefaultFont()
#define OFontBig			onut::getDefaultFontBig()

//--- Resource creation shortcuts
#define OTextureFromFile	onut::Texture::createFromFile
#define OFontFromFile		onut::BMFont::createFromFile

//--- Resolution helpers
#define OScreen				onut::getRenderer()->getResolution()
#define OScreenf			{static_cast<float>(onut::getRenderer()->getResolution().x), static_cast<float>(onut::getRenderer()->getResolution().y)}
#define OScreenW			onut::getRenderer()->getResolution().x
#define OScreenH			onut::getRenderer()->getResolution().y;
#define OScreenWf			static_cast<float>(onut::getRenderer()->getResolution().x)
#define OScreenHf			static_cast<float>(onut::getRenderer()->getResolution().y)
#define OScreenCenter		(onut::getRenderer()->getResolution() / 2)
#define OScreenCenterf		{static_cast<float>(onut::getRenderer()->getResolution().x / 2), static_cast<float>(onut::getRenderer()->getResolution().y / 2)}
#define OScreenCenterX		(onut::getRenderer()->getResolution().x / 2)
#define OScreenCenterY		(onut::getRenderer()->getResolution().y / 2)
#define OScreenCenterXf		static_cast<float>(onut::getRenderer()->getResolution().x / 2)
#define OScreenCenterYf		static_cast<float>(onut::getRenderer()->getResolution().y / 2)

//--- Rect helpers for drawing
// Centered origin sprite
// cx,cy = Sprite center position
#define ORectCenteredOrigin(cx, cy, w, h)		{(cx) - (w) * .5f, (cy) - (h) * .5f, w, h}

// Rectangle helpers
#define ORectFullScreen							{0, 0, OScreenWf, OScreenHf}
#define ORectGrid(cols, rows, ix, iy, px, py)	{OScreenWf/(cols)*(ix)+((ix>0)?px*.5f:px),OScreenHf/(rows)*(iy)+((iy>0)?py*.5f:py),OScreenWf/(cols)-((ix<=0||ix>=cols-1)?px*1.5f:px),OScreenHf/(rows)-((iy<=0||iy>=rows-1)?py*1.5f:py)}
#define ORectLayout(left, top, right, bottom)	{(left), (top), (right) - (left), (bottom) - (top)}
#define ORectAlign								onut::alignedRect

// Color stuff
#define OColorHex(hex)							Color::fromHexRGB(0x ## hex)

// Gradient
#define OGradientH(left, right)					{left, left, right, right}
#define OGradientV(top, bottom)					{top, bottom, bottom, top}

// Alignment
#define OTopLeft								onut::Align::TOP_LEFT
#define OTop									onut::Align::TOP
#define OTopRight								onut::Align::TOP_RIGHT
#define OLeft									onut::Align::LEFT
#define OCenter									onut::Align::CENTER
#define ORight									onut::Align::RIGHT
#define OBottomLeft								onut::Align::BOTTOM_LEFT
#define OBottom									onut::Align::BOTTOM
#define OBottomRight							onut::Align::BOTTOM_RIGHT

// Alignement of 1D position
#define OPosAbsX(x, p)							((x) + (p))
#define OPosAbsY(y, p)							((y) + (p))
#define OPosRelX(x, p)							(OScreenWf - (x) - (p))
#define OPosRelY(y, p)							(OScreenHf - (y) - (p))

namespace onut {
	// Palettes
	extern std::vector<Color> palPinkLovers;
	extern std::vector<Color> palPeonyJoy;
	extern std::vector<Color> palWinterSun;
	extern std::vector<Color> palHeartDesire;
	extern std::vector<Color> palNatureWalk;

	// Rect alignement helper templated class
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
}
