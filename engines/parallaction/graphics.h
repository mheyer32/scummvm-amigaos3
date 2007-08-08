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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#ifndef PARALLACTION_GRAPHICS_H
#define PARALLACTION_GRAPHICS_H

#include "common/rect.h"
#include "common/stream.h"

#include "graphics/surface.h"

#include "parallaction/defs.h"


namespace Parallaction {


#define BASE_PALETTE_COLORS		32
#define FIRST_BASE_COLOR		0
#define LAST_BASE_COLOR			(FIRST_BASE_COLOR+BASE_PALETTE_COLORS-1)

#define EHB_PALETTE_COLORS		32										// extra half-brite colors for amiga
#define FIRST_EHB_COLOR			(LAST_BASE_COLOR+1)
#define LAST_EHB_COLOR			(FIRST_EHB_COLOR+EHB_PALETTE_COLORS-1)

#define PALETTE_COLORS			(BASE_PALETTE_COLORS+EHB_PALETTE_COLORS)

#define BASE_PALETTE_SIZE		BASE_PALETTE_COLORS*3
#define PALETTE_SIZE			PALETTE_COLORS*3

#define MOUSEARROW_WIDTH		16
#define MOUSEARROW_HEIGHT		16

#define MOUSECOMBO_WIDTH		32	// sizes for cursor + selected inventory item
#define MOUSECOMBO_HEIGHT		32

#include "common/pack-start.h"	// START STRUCT PACKING

struct PaletteFxRange {

	uint16	_timer;
	uint16	_step;
	uint16	_flags;
	byte	_first;
	byte	_last;

} PACKED_STRUCT;

#include "common/pack-end.h"	// END STRUCT PACKING

class Font {

protected:
	byte _color;


public:
	Font() {}
	virtual ~Font() {}

	virtual void setColor(byte color) {
		_color = color;
	}
	virtual uint32 getStringWidth(const char *s) = 0;
	virtual uint16 height() = 0;

	virtual void drawString(byte* buffer, uint32 pitch, const char *s) = 0;


};

struct Cnv {
	uint16	_count; 	// # of frames
	uint16	_width; 	//
	uint16	_height;	//
	byte**	field_8;	// unused
	byte*	_data;

public:
	Cnv() {
		_width = _height = _count = 0;
		_data = NULL;
	}

	Cnv(uint16 numFrames, uint16 width, uint16 height, byte* data) : _count(numFrames), _width(width), _height(height), _data(data) {

	}

	~Cnv() {
		if (_count == 0 || _data == NULL) return;
		free(_data);
	}

	byte* getFramePtr(uint16 index) {
		if (index >= _count)
			return NULL;
		return &_data[index * _width * _height];
	}
};


#define NUM_BUFFERS 4

class Parallaction;

struct DoorData;
struct GetData;

enum Fonts {
	kFontDialogue = 0,
	kFontLabel = 1,
	kFontMenu = 2
};

struct MaskBuffer {
	// handles a 2-bit depth buffer used for z-buffering

	uint16	w;
	uint16  internalWidth;
	uint16	h;
	uint	size;
	byte	*data;

public:
	MaskBuffer() : w(0), internalWidth(0), h(0), size(0), data(0) {
	}

	void create(uint16 width, uint16 height) {
		w = width;
		internalWidth = w >> 2;
		h = height;
		size = (internalWidth * h);
		data = (byte*)calloc(size, 1);
	}

	void free() {
		if (data)
			::free(data);
		data = 0;
		w = 0;
		h = 0;
		internalWidth = 0;
		size = 0;
	}

	inline byte getValue(uint16 x, uint16 y) {
		byte m = data[(x >> 2) + y * internalWidth];
		uint n = (x & 3) << 1;
		return ((3 << n) & m) >> n;
	}

};

class Gfx {

public:
	typedef byte Palette[PALETTE_SIZE];

	enum Buffers {
		// bit buffers
		kBitFront,
		kBitBack,
		kBit2
	};

public:

	// balloons and text
	void showLocationComment(const char *text, bool end = false);
	void drawBalloon(const Common::Rect& r, uint16 arg_8);
	void displayString(uint16 x, uint16 y, const char *text, byte color);
	void displayCenteredString(uint16 y, const char *text);
	bool displayWrappedString(char *text, uint16 x, uint16 y, byte color, int16 wrapwidth = -1);
	uint16 getStringWidth(const char *text);
	void getStringExtent(char *text, uint16 maxwidth, int16* width, int16* height);
	void makeCnvFromString(Graphics::Surface *cnv, char *text);

	// cut/paste
	void flatBlitCnv(Graphics::Surface *cnv, int16 x, int16 y, Gfx::Buffers buffer);
	void flatBlitCnv(Cnv *cnv, uint16 frame, int16 x, int16 y, Gfx::Buffers buffer);
	void blitCnv(Graphics::Surface *cnv, int16 x, int16 y, uint16 z, Gfx::Buffers buffer);
	void restoreBackground(const Common::Rect& r);
	void backupDoorBackground(DoorData *data, int16 x, int16 y);
	void restoreDoorBackground(const Common::Rect& r, byte *data, byte* background);
	void backupGetBackground(GetData *data, int16 x, int16 y);
	void restoreGetBackground(const Common::Rect& r, byte *data);

	// hacks for Nippon Safes
	void intGrottaHackMask();
	void fillMaskRect(const Common::Rect& r, byte color);
	void zeroMaskValue(uint16 x, uint16 y, byte color);

	// low level surfaces
	void clearScreen(Gfx::Buffers buffer);
	void copyScreen(Gfx::Buffers srcbuffer, Gfx::Buffers dstbuffer);
	void copyRect(Gfx::Buffers dstbuffer, const Common::Rect& r, byte *src, uint16 pitch);
	void grabRect(byte *dst, const Common::Rect& r, Gfx::Buffers srcbuffer, uint16 pitch);
	void floodFill(Gfx::Buffers buffer, const Common::Rect& r, byte color);

	// palette
	void setPalette(Palette palette, uint32 first = FIRST_BASE_COLOR, uint32 num = BASE_PALETTE_COLORS);
	void setBlackPalette();
	void animatePalette();
	void fadePalette(Palette palette, Palette target, uint step);			// fades palette to target palette, with specified step
	void makeGrayscalePalette(Palette palette);		// transform palette into black and white

	// amiga specific
	void setHalfbriteMode(bool enable);

	// misc
	int16 queryMask(int16 v);
	void setMousePointer(int16 index);
	void setFont(Fonts name);
	void swapBuffers();
	void updateScreen();
	void setBackground(Graphics::Surface *surf);
	void setMask(MaskBuffer *buffer);

	// init
	Gfx(Parallaction* vm);
	virtual ~Gfx();


public:
	Common::Point		_labelPosition[2];
	uint16				_bgLayers[4];
	PaletteFxRange		_palettefx[6];
	Palette				_palette;

protected:
	Parallaction*		_vm;
	Graphics::Surface	*_buffers[NUM_BUFFERS];
	MaskBuffer			*_depthMask;
	static byte			_mouseArrow[256];
	Graphics::Surface			*_mouseComposedArrow;
	Font				*_font;
	Font				*_fonts[3];
	bool				_halfbrite;

protected:
	void copyRect(uint width, uint height, byte *dst, uint dstPitch, byte *src, uint srcPitch);
	void flatBlit(const Common::Rect& r, byte *data, Gfx::Buffers buffer, byte transparentColor);
	void blit(const Common::Rect& r, uint16 z, byte *data, Gfx::Buffers buffer);
	void initMouse(uint16 arg_0);
	void screenClip(Common::Rect& r, Common::Point& p);
	void initFonts();
};


} // Parallaction


#endif



