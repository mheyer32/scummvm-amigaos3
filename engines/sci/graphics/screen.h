/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef SCI_GRAPHICS_SCREEN_H
#define SCI_GRAPHICS_SCREEN_H

#include "sci/sci.h"
#include "sci/graphics/helpers.h"
#include "sci/graphics/view.h"

#include "graphics/sjis.h"

namespace Sci {

enum {
	SCI_SCREEN_UPSCALEDMAXHEIGHT = 200,
	SCI_SCREEN_UPSCALEDMAXWIDTH  = 320
};

enum GfxScreenUpscaledMode {
	GFX_SCREEN_UPSCALED_DISABLED	= 0,
	GFX_SCREEN_UPSCALED_480x300     = 1,
	GFX_SCREEN_UPSCALED_640x400		= 2,
	GFX_SCREEN_UPSCALED_640x440		= 3,
	GFX_SCREEN_UPSCALED_640x480		= 4
};

enum GfxScreenMasks {
	GFX_SCREEN_MASK_VISUAL		= 1,
	GFX_SCREEN_MASK_PRIORITY	= 2,
	GFX_SCREEN_MASK_CONTROL		= 4,
	GFX_SCREEN_MASK_DISPLAY		= 8, // not official sierra sci, only used internally
	GFX_SCREEN_MASK_ALL			= GFX_SCREEN_MASK_VISUAL|GFX_SCREEN_MASK_PRIORITY|GFX_SCREEN_MASK_CONTROL
};

enum {
	DITHERED_BG_COLORS_SIZE = 256
};

/**
 * Screen class, actually creates 3 (4) screens internally:
 * - visual/display (for the user),
 * - priority (contains priority information) and
 * - control (contains control information).
 * Handles all operations to it and copies parts of visual/display screen to
 * the actual screen, so the user can really see it.
 */
class GfxScreen {
public:
	GfxScreen(ResourceManager *resMan);
	~GfxScreen();

	uint16 getWidth() { return _width; }
	uint16 getHeight() { return _height; }
	uint16 getScriptWidth() { return _scriptWidth; }
	uint16 getScriptHeight() { return _scriptHeight; }
	uint16 getDisplayWidth() { return _displayWidth; }
	uint16 getDisplayHeight() { return _displayHeight; }
	byte getColorWhite() { return _colorWhite; }
	byte getColorDefaultVectorData() { return _colorDefaultVectorData; }

	void clearForRestoreGame();
	void copyToScreen();
	void copyFromScreen(byte *buffer);
	void kernelSyncWithFramebuffer();
	void copyRectToScreen(const Common::Rect &rect);
	void copyDisplayRectToScreen(const Common::Rect &rect);
	void copyRectToScreen(const Common::Rect &rect, int16 x, int16 y);

	// Vector drawing
private:
	void vectorPutLinePixel(int16 x, int16 y, byte drawMask, byte color, byte priority, byte control);
	void vectorPutLinePixel480x300(int16 x, int16 y, byte drawMask, byte color, byte priority, byte control);

public:
	void vectorAdjustLineCoordinates(int16 *left, int16 *top, int16 *right, int16 *bottom, byte drawMask, byte color, byte priority, byte control);
	byte vectorIsFillMatch(int16 x, int16 y, byte screenMask, byte checkForColor, byte checkForPriority, byte checkForControl, bool isEGA);

	byte getDrawingMask(byte color, byte prio, byte control);
	void drawLine(Common::Point startPoint, Common::Point endPoint, byte color, byte prio, byte control);
	void drawLine(int16 left, int16 top, int16 right, int16 bottom, byte color, byte prio, byte control) {
		drawLine(Common::Point(left, top), Common::Point(right, bottom), color, prio, control);
	}

	GfxScreenUpscaledMode getUpscaledHires() const {
		return _upscaledHires;
	}

	bool isUnditheringEnabled() const {
		return _unditheringEnabled;
	}
	void enableUndithering(bool flag);

	void putKanjiChar(Graphics::FontSJIS *commonFont, int16 x, int16 y, uint16 chr, byte color);

	int bitsGetDataSize(Common::Rect rect, byte mask);
	void bitsSave(Common::Rect rect, byte mask, byte *memoryPtr);
	void bitsGetRect(byte *memoryPtr, Common::Rect *destRect);
	void bitsRestore(byte *memoryPtr);

	void scale2x(const SciSpan<const byte> &src, SciSpan<byte> &dst, int16 srcWidth, int16 srcHeight, byte bytesPerPixel = 1);

	void adjustToUpscaledCoordinates(int16 &y, int16 &x);
	void adjustBackUpscaledCoordinates(int16 &y, int16 &x);

	void dither(bool addToFlag);

	// Force a color combination as a dithered color
	void ditherForceDitheredColor(byte color);
	int16 *unditherGetDitheredBgColors();

	void debugShowMap(int mapNo);

	int _picNotValid; // possible values 0, 1 and 2
	int _picNotValidSci11; // another variable that is used by kPicNotValid in sci1.1

	int16 kernelPicNotValid(int16 newPicNotValid);
	void kernelShakeScreen(uint16 shakeCount, uint16 direction);

	void setFontIsUpscaled(bool isUpscaled) { _fontIsUpscaled = isUpscaled; }
	bool fontIsUpscaled() const { return _fontIsUpscaled; }

private:
	uint16 _width;
	uint16 _height;
	uint _pixels;
	uint16 _scriptWidth;
	uint16 _scriptHeight;
	uint16 _displayWidth;
	uint16 _displayHeight;
	uint _displayPixels;

	byte _colorWhite;
	byte _colorDefaultVectorData;

	void bitsRestoreScreen(Common::Rect rect, byte *&memoryPtr, byte *screen, uint16 screenWidth);
	void bitsRestoreDisplayScreen(Common::Rect rect, byte *&memoryPtr);
	void bitsSaveScreen(Common::Rect rect, byte *screen, uint16 screenWidth, byte *&memoryPtr);
	void bitsSaveDisplayScreen(Common::Rect rect, byte *&memoryPtr);

	void setShakePos(uint16 shakeXOffset, uint16 shakeYOffset);
	void  REGPARM putPixel320(int16 x, int16 y, byte drawMask, byte color, byte priority, byte control) const;
	void  REGPARM putPixelGeneric(int16 x, int16 y, byte drawMask, byte color, byte priority, byte control) const;
	void  REGPARM putPixel480x300(int16 x, int16 y, byte drawMask, byte color, byte priority, byte control) const;
	void  REGPARM putPixel480x300Worker(int16 x, int16 y, int offset, byte *screen, byte byteToSet) const;
	void  REGPARM vectorPutPixelGeneric(int16 x, int16 y, byte drawMask, byte color, byte priority, byte control) const;

	// Vector related public code - in here, so that it can be inlined
	byte REGPARM getPixel320(const byte *screen, int16 x, int16 y) const {
		return screen[((uint)y  << 8)  + ((uint)y << 6) + x];
	}

	/**
	 * If this flag is true, undithering is enabled, otherwise disabled.
	 */
	bool _unditheringEnabled;
	int16 _ditheredPicColors[DITHERED_BG_COLORS_SIZE];


	typedef byte REGPARM (GfxScreen::*GetPixelFunc)(const byte *screen, int16 x, int16) const;
	GetPixelFunc _getPixelFunc = &GfxScreen::getPixel320;
	GetPixelFunc _vectorGetPixelFunc = &GfxScreen::getPixel320;

	typedef void  REGPARM  (GfxScreen::*PutPixelFunc)(int16 x, int16 y, byte drawMask, byte color, byte priority, byte control)  const;
	PutPixelFunc _putPixelFunc = &GfxScreen::putPixel320;
	PutPixelFunc _vectorPutPixelFunc = &GfxScreen::putPixel320;

	// These screens have the real resolution of the game engine (320x200 for
	// SCI0/SCI1/SCI11 games, 640x480 for SCI2 games). SCI0 games will be
	// dithered in here at any time.
	byte *_visualScreen;
	byte *_priorityScreen;
	byte *_controlScreen;

	/**
	 * This screen is the one, where pixels are copied out of into the frame buffer.
	 * It may be 640x400 for japanese SCI1 games. SCI0 games may be undithered in here.
	 * Only read from this buffer for Save/ShowBits usage.
	 */
	byte *_displayScreen;

	ResourceManager *_resMan;

	/**
	 * Pointer to the currently active screen (changing only required for
	 * debug purposes, to show for example the priority screen).
	 */
	byte *_activeScreen;

	/**
	 * This variable defines, if upscaled hires is active and what upscaled mode
	 * is used.
	 */
	GfxScreenUpscaledMode _upscaledHires;

	/**
	 * This here holds a translation for vertical+horizontal coordinates between native
	 * (visual) and actual (display) screen.
	 */
	int16 _upscaledHeightMapping[SCI_SCREEN_UPSCALEDMAXHEIGHT + 1];
	int16 _upscaledWidthMapping[SCI_SCREEN_UPSCALEDMAXWIDTH + 1];

	/**
	 * This defines whether or not the font we're drawing is already scaled
	 * to the screen size (and we therefore should not upscale it ourselves).
	 */
	bool _fontIsUpscaled;

	// pixel related code, in header so that it can be inlined for performance
public:

	FORCEINLINE void REGPARM putPixel(int16 x, int16 y, byte drawMask, byte color, byte priority, byte control) const {
		(this->*_putPixelFunc)(x, y, drawMask, color, priority, control);
	}

	// This is called from vector drawing to put a pixel at a certain location
	FORCEINLINE void REGPARM vectorPutPixel(int16 x, int16 y, byte drawMask, byte color, byte priority, byte control) const {
		(this->*_vectorPutPixelFunc)(x, y, drawMask, color, priority, control);
	}

	/**
	 * This will just change a pixel directly on displayscreen. It is supposed to be
	 * only used on upscaled-Hires games where hires content needs to get drawn ONTO
	 * the upscaled display screen (like japanese fonts, hires portraits, etc.).
	 */
	FORCEINLINE void REGPARM putPixelOnDisplay(int16 x, int16 y, byte color) const {
		int offset = y * _displayWidth + x;
		_displayScreen[offset] = color;
	}

	// Upscales a pixel and puts it on display screen only
	void REGPARM putScaledPixelOnDisplay(int16 x, int16 y, byte color) const;

	/**
	 * This is used to put font pixels onto the screen - we adjust differently, so that we won't
	 *  do triple pixel lines in any case on upscaled hires. That way the font will not get distorted
	 *  Sierra SCI didn't do this
	 */
	void REGPARM putFontPixel(int16 startingY, int16 x, int16 y, byte color) const;


	FORCEINLINE byte REGPARM getPixel(const byte *screen, int16 x, int16 y) const {
		return (this->*_getPixelFunc)(screen, x, y);
	}

	FORCEINLINE byte REGPARM vectorGetPixel(const byte *screen, int16 x, int16 y) const {
		return (this->*_vectorGetPixelFunc)(screen, x, y);
	}

	byte REGPARM getPixelGeneric(const byte *screen, int16 x, int16 y) const;

	byte REGPARM getVisual(int16 x, int16 y) const {
		return getPixel(_visualScreen, x, y);
	}
	byte REGPARM getPriority(int16 x, int16 y) const {
		return getPixel(_priorityScreen, x, y);
	}
	byte REGPARM getControl(int16 x, int16 y) const {
		return getPixel(_controlScreen, x, y);
	}

	// Vector related public code - in here, so that it can be inlined
	byte REGPARM vectorGetPixelGeneric(const byte *screen, int16 x, int16 y) const {
		return screen[y * _width + x];
	}

	byte REGPARM vectorGetVisual(int16 x, int16 y) const {
		return vectorGetPixel(_visualScreen, x, y);
	}
	byte REGPARM vectorGetPriority(int16 x, int16 y) const {
		return vectorGetPixel(_priorityScreen, x, y);
	}
	byte REGPARM vectorGetControl(int16 x, int16 y) const {
		return vectorGetPixel(_controlScreen, x, y);
	}

	void REGPARM vectorAdjustCoordinate(int16 *x, int16 *y) const {
		switch (_upscaledHires) {
		case GFX_SCREEN_UPSCALED_480x300:
			*x = (*x * 3) / 2;
			*y = (*y * 3) / 2;
			break;
		default:
			break;
		}
	}
};

} // End of namespace Sci

#endif
