#pragma once
#include "Typedefs.h"

//--- Some shortcuts
#define ORun				onut::run
#define ORenderer			onut::getRenderer()
#define OSpriteBatch		onut::getSpriteBatch()
#define OSettings			onut::getSettings()
#define OTextureFromFile	onut::Texture::createFromFile

//--- Resolution helpers
#define OScreenW			onut::getRenderer()->getResolution().x
#define OScreenH			onut::getRenderer()->getResolution().y;
#define OScreenWf			static_cast<float>(onut::getRenderer()->getResolution().x)
#define OScreenHf			static_cast<float>(onut::getRenderer()->getResolution().y)
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
#define ORectGrid(cols, rows, ix, iy, px, py)	{OScreenWf/(cols)*(ix)+(px)*.5f,OScreenHf/(rows)*(iy)+(py)*.5f,OScreenWf/(cols)-(px),OScreenHf/(rows)-(py)}
