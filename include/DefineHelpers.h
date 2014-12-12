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

// Alignment with screen
// px,py = Sprite x,y padding
#define ORectFullScreen							{0, 0, OScreenWf, OScreenHf}
#define ORectAlignTopLeft(px, py, w, h)			{px, py, w, h}
#define ORectAlignTop(px, py, w, h)				{(OScreenWf - (w)) * .5f + (px), py, w, h}
#define ORectAlignTopRight(px, py, w, h)		{OScreenWf - (w) - (px), (py), w, h}
#define ORectAlignLeft(px, py, w, h)			{px, (OScreenHf - (h)) * .5f + (py), w, h}
#define ORectAlignCenter(px, py, w, h)			{(OScreenWf - (w)) * .5f + (px), (OScreenHf - (h)) * .5f + (py), w, h}
#define ORectAlignRight(px, py, w, h)			{OScreenWf - (w) - (px), (OScreenHf - (h)) * .5f + (py), w, h}
#define ORectAlignBottomLeft(px, py, w, h)		{px, OScreenHf - (h) - (py), w, h}
#define ORectAlignBottom(px, py, w, h)			{(OScreenWf - (w)) * .5f + (px), OScreenHf - (h) - (py), w, h}
#define ORectAlignBottomRight(px, py, w, h)		{OScreenWf - (w) - (px), OScreenHf - (h) - (py), w, h}

// Grid alignment
#define ORectGrid(cols, rows, ix, iy, px, py)	{OScreenWf/(cols)*(ix)+((ix>0)?px*.5f:px),OScreenHf/(rows)*(iy)+((iy>0)?py*.5f:py),OScreenWf/(cols)-((ix<=0||ix>=cols-1)?px*1.5f:px),OScreenHf/(rows)-((iy<=0||iy>=rows-1)?py*1.5f:py)}

// Color stuff
#define OColorHex(hex)							Color::fromHexRGB(0x ## hex)

// Gradient
#define OGradientV(top, bottom)					{top, bottom, bottom, top}

// Font alignment quicky
#define OAlignTopLeft							onut::Align::TOP_LEFT
#define OAlignTop								onut::Align::TOP
#define OAlignTopRight							onut::Align::TOP_RIGHT
#define OAlignLeft								onut::Align::LEFT
#define OAlignCenter							onut::Align::CENTER
#define OAlignRight								onut::Align::RIGHT
#define OAlignBottomLeft						onut::Align::BOTTOM_LEFT
#define OAlignBottom							onut::Align::BOTTOM
#define OAlignBottomRight						onut::Align::BOTTOM_RIGHT

// Palettes
namespace onut {
	extern std::vector<Color> palPinkLovers;
	extern std::vector<Color> palPeonyJoy;
	extern std::vector<Color> palWinterSun;
	extern std::vector<Color> palHeartDesire;
	extern std::vector<Color> palNatureWalk;
}
