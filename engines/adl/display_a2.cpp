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

#include "common/stream.h"
#include "common/rect.h"
#include "common/system.h"
#include "common/str.h"
#include "common/config-manager.h"

#include "graphics/surface.h"
#include "graphics/palette.h"
#include "graphics/thumbnail.h"

#include "engines/util.h"

#include "adl/display_a2.h"
#include "adl/adl.h"

namespace Adl {

#define COLOR_PALETTE_ENTRIES 8
static const byte colorPalette[COLOR_PALETTE_ENTRIES * 3] = {
	0x00, 0x00, 0x00,
	0xff, 0xff, 0xff,
	0xc7, 0x34, 0xff,
	0x38, 0xcb, 0x00,
	0x00, 0x00, 0x00,
	0xff, 0xff, 0xff,
	0x0d, 0xa1, 0xff,
	0xf2, 0x5e, 0x00
};

// Opacity of the optional scanlines (percentage)
#define SCANLINE_OPACITY 75

// Corresponding color in second palette
#define PAL2(X) ((X) | 0x04)

// Alternate color for odd pixel rows (for scanlines)
#define ALTCOL(X) ((X) | 0x08)

// Green monochrome palette
#define MONO_PALETTE_ENTRIES 2
static const byte monoPalette[MONO_PALETTE_ENTRIES * 3] = {
	0x00, 0x00, 0x00,
	0x00, 0xc0, 0x01
};

// Uppercase-only Apple II font (manually created).
static const byte font[64][5] = {
	{ 0x7c, 0x82, 0xba, 0xb2, 0x9c }, { 0xf8, 0x24, 0x22, 0x24, 0xf8 }, // @A
	{ 0xfe, 0x92, 0x92, 0x92, 0x6c }, { 0x7c, 0x82, 0x82, 0x82, 0x44 }, // BC
	{ 0xfe, 0x82, 0x82, 0x82, 0x7c }, { 0xfe, 0x92, 0x92, 0x92, 0x82 }, // DE
	{ 0xfe, 0x12, 0x12, 0x12, 0x02 }, { 0x7c, 0x82, 0x82, 0xa2, 0xe2 }, // FG
	{ 0xfe, 0x10, 0x10, 0x10, 0xfe }, { 0x00, 0x82, 0xfe, 0x82, 0x00 }, // HI
	{ 0x40, 0x80, 0x80, 0x80, 0x7e }, { 0xfe, 0x10, 0x28, 0x44, 0x82 }, // JK
	{ 0xfe, 0x80, 0x80, 0x80, 0x80 }, { 0xfe, 0x04, 0x18, 0x04, 0xfe }, // LM
	{ 0xfe, 0x08, 0x10, 0x20, 0xfe }, { 0x7c, 0x82, 0x82, 0x82, 0x7c }, // NO
	{ 0xfe, 0x12, 0x12, 0x12, 0x0c }, { 0x7c, 0x82, 0xa2, 0x42, 0xbc }, // PQ
	{ 0xfe, 0x12, 0x32, 0x52, 0x8c }, { 0x4c, 0x92, 0x92, 0x92, 0x64 }, // RS
	{ 0x02, 0x02, 0xfe, 0x02, 0x02 }, { 0x7e, 0x80, 0x80, 0x80, 0x7e }, // TU
	{ 0x3e, 0x40, 0x80, 0x40, 0x3e }, { 0xfe, 0x40, 0x30, 0x40, 0xfe }, // VW
	{ 0xc6, 0x28, 0x10, 0x28, 0xc6 }, { 0x06, 0x08, 0xf0, 0x08, 0x06 }, // XY
	{ 0xc2, 0xa2, 0x92, 0x8a, 0x86 }, { 0xfe, 0xfe, 0x82, 0x82, 0x82 }, // Z[
	{ 0x04, 0x08, 0x10, 0x20, 0x40 }, { 0x82, 0x82, 0x82, 0xfe, 0xfe }, // \]
	{ 0x20, 0x10, 0x08, 0x10, 0x20 }, { 0x80, 0x80, 0x80, 0x80, 0x80 }, // ^_
	{ 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0xbe, 0x00, 0x00 }, //  !
	{ 0x00, 0x0e, 0x00, 0x0e, 0x00 }, { 0x28, 0xfe, 0x28, 0xfe, 0x28 }, // "#
	{ 0x48, 0x54, 0xfe, 0x54, 0x24 }, { 0x46, 0x26, 0x10, 0xc8, 0xc4 }, // $%
	{ 0x6c, 0x92, 0xac, 0x40, 0xa0 }, { 0x00, 0x00, 0x0e, 0x00, 0x00 }, // &'
	{ 0x38, 0x44, 0x82, 0x00, 0x00 }, { 0x00, 0x00, 0x82, 0x44, 0x38 }, // ()
	{ 0x44, 0x28, 0xfe, 0x28, 0x44 }, { 0x10, 0x10, 0x7c, 0x10, 0x10 }, // *+
	{ 0x00, 0x80, 0x60, 0x00, 0x00 }, { 0x10, 0x10, 0x10, 0x10, 0x10 }, // ,-
	{ 0x00, 0x00, 0x80, 0x00, 0x00 }, { 0x40, 0x20, 0x10, 0x08, 0x04 }, // ./
	{ 0x7c, 0xa2, 0x92, 0x8a, 0x7c }, { 0x00, 0x84, 0xfe, 0x80, 0x00 }, // 01
	{ 0xc4, 0xa2, 0x92, 0x92, 0x8c }, { 0x42, 0x82, 0x92, 0x9a, 0x66 }, // 23
	{ 0x30, 0x28, 0x24, 0xfe, 0x20 }, { 0x4e, 0x8a, 0x8a, 0x8a, 0x72 }, // 45
	{ 0x78, 0x94, 0x92, 0x92, 0x62 }, { 0x02, 0xe2, 0x12, 0x0a, 0x06 }, // 67
	{ 0x6c, 0x92, 0x92, 0x92, 0x6c }, { 0x8c, 0x92, 0x92, 0x52, 0x3c }, // 89
	{ 0x00, 0x00, 0x28, 0x00, 0x00 }, { 0x00, 0x80, 0x68, 0x00, 0x00 }, // :;
	{ 0x10, 0x28, 0x44, 0x82, 0x00 }, { 0x28, 0x28, 0x28, 0x28, 0x28 }, // <=
	{ 0x00, 0x82, 0x44, 0x28, 0x10 }, { 0x04, 0x02, 0xb2, 0x0a, 0x04 }  // >?
};


static byte processColorBits(uint16 &bits, bool &odd, bool secondPal) {
	byte color = 0;

	switch (bits & 0x7) {
	case 0x3: // 011 (white)
	case 0x6: // 110
	case 0x7: // 111
		color = 1;
		break;
	case 0x2: // 010 (color)
		color = 2 + odd;
		break;
	case 0x5: // 101 (color)
		color = 2 + !odd;
	}

	if (secondPal)
		color = PAL2(color);

	odd = !odd;
	bits >>= 1;

	return color;
}

static void renderPixelRowColor(byte *dst, byte *src) {
	uint16 bits = (src[0] & 0x7f) << 1;
	byte pal = src[0] >> 7;

	if (pal != 0)
		*dst++ = 0;

	bool odd = false;

	for (uint i = 0; i < Display_A2::kGfxPitch; ++i) {
		if (i != Display_A2::kGfxPitch - 1) {
			bits |= (src[i + 1] & 0x7f) << 8;
			pal |= (src[i + 1] >> 7) << 1;
		}

		// For the first 6 bits in the block we draw two pixels
		for (uint j = 0; j < 6; ++j) {
			byte color = processColorBits(bits, odd, pal & 1);
			*dst++ = color;
			*dst++ = color;
		}

		// Last bit of the block, draw one, two or three pixels
		byte color = processColorBits(bits, odd, pal & 1);

		// Draw the first pixel
		*dst++ = color;

		switch (pal) {
			case 0x0:
			case 0x3:
				// If palette stays the same, draw a second pixel
				*dst++ = color;
				break;
			case 0x2:
				// If we're moving from first to second palette,
				// draw a second pixel, and a third in the second
				// palette.
				*dst++ = color;
				*dst++ = PAL2(color);
		}

		pal >>= 1;
	}
}

static void renderPixelRowMono(byte *dst, byte *src) {
	byte pal = src[0] >> 7;

	if (pal != 0)
		*dst++ = 0;

	for (uint i = 0; i < Display_A2::kGfxPitch; ++i) {
		if (i != Display_A2::kGfxPitch - 1)
			pal |= (src[i + 1] >> 7) << 1;

		for (uint j = 0; j < 6; ++j) {
			bool color = src[i] & (1 << j);
			*dst++ = color;
			*dst++ = color;
		}

		bool color = src[i] & (1 << 6);

		*dst++ = color;

		switch (pal) {
			case 0x0:
			case 0x3:
				*dst++ = color;
				break;
			case 0x2:
				*dst++ = color;
				*dst++ = color;
		}

		pal >>= 1;
	}
}

static void copyEvenSurfaceRows(Graphics::Surface &surf) {
	byte *src = (byte *)surf.getPixels();

	for (uint y = 0; y < surf.h / 2u; ++y) {
		byte *dst = src + surf.pitch;
		for (uint x = 0; x < surf.w; ++x)
			dst[x] = ALTCOL(src[x]);
		src += surf.pitch * 2;
	}
}

class Display_A2_Monitor : public Display_A2 {
public:
	Display_A2_Monitor();
	~Display_A2_Monitor();

	enum {
		kSplitHeight = 64
	};

	void init() override;
	void renderText() override;
	void renderGraphics() override;

private:
	void updateTextSurface();
	void updateGfxSurface();
	void drawChar(byte c, int x, int y);
	void createFont();
	void showScanlines(bool enable);
	void createSurfaces(uint gfxWidth, uint gfxHeight);

	Graphics::Surface *_textSurface;
	Graphics::Surface *_gfxSurface;
	Graphics::Surface *_font;
	bool _scanlines;
	bool _monochrome;
};

Display_A2_Monitor::Display_A2_Monitor() :
		_textSurface(nullptr),
		_gfxSurface(nullptr),
		_font(nullptr),
		_scanlines(false),
		_monochrome(false) { }

Display_A2_Monitor::~Display_A2_Monitor() {
	if (_font) {
		_font->free();
		delete _font;
	}

	_textSurface->free();
	delete _textSurface;

	_gfxSurface->free();
	delete _gfxSurface;
}

void Display_A2_Monitor::createSurfaces(uint gfxWidth, uint gfxHeight) {
	_gfxSurface = new Graphics::Surface;
	_gfxSurface->create(gfxWidth, gfxHeight, Graphics::PixelFormat::createFormatCLUT8());
	_textSurface = new Graphics::Surface;
	_textSurface->create(gfxWidth, gfxHeight, Graphics::PixelFormat::createFormatCLUT8());
}

void Display_A2_Monitor::init() {
	Display_A2::init();

	// We need 2x scaling to properly render the half-pixel shift
	// of the second palette
	createSurfaces(Display_A2::kGfxWidth * 2, Display_A2::kGfxHeight * 2);

	_monochrome = !ConfMan.getBool("color");
	_scanlines = ConfMan.getBool("scanlines");

	if (_monochrome)
		g_system->getPaletteManager()->setPalette(monoPalette, 0, MONO_PALETTE_ENTRIES);
	else
		g_system->getPaletteManager()->setPalette(colorPalette, 0, COLOR_PALETTE_ENTRIES);

	createFont();

	showScanlines(_scanlines);
}

void Display_A2_Monitor::updateGfxSurface() {
	byte *src = _frameBuf;
	byte *dst = (byte *)_gfxSurface->getPixels();

	for (uint i = 0; i < Display_A2::kGfxHeight; ++i) {
		if (_monochrome)
			renderPixelRowMono(dst, src);
		else
			renderPixelRowColor(dst, src);
		src += Display_A2::kGfxPitch;
		dst += _gfxSurface->pitch * 2;
	}

	copyEvenSurfaceRows(*_gfxSurface);
}

void Display_A2_Monitor::updateTextSurface() {
	for (uint row = 0; row < 24; ++row)
		for (uint col = 0; col < Display_A2::kTextWidth; ++col) {
			uint charPos = row * Display_A2::kTextWidth + col;
			char c = _textBuf[row * Display_A2::kTextWidth + col];

			if (charPos == _cursorPos && _showCursor)
				c = (c & 0x3f) | 0x40;

			Common::Rect r(7 * 2, 8 * 2);
			r.translate(((c & 0x3f) % 16) * 7 * 2, (c & 0x3f) / 16 * 8 * 2);

			if (!(c & 0x80)) {
				if (!(c & 0x40) || ((g_system->getMillis() / 270) & 1))
					r.translate(0, 4 * 8 * 2);
			}

			_textSurface->copyRectToSurface(*_font, col * 7 * 2, row * 8 * 2, r);
		}
}

void Display_A2_Monitor::renderText() {
	updateTextSurface();

	if (_mode == Display::kModeText)
		g_system->copyRectToScreen(_textSurface->getPixels(), _textSurface->pitch, 0, 0, _textSurface->w, _textSurface->h);
	else if (_mode == Display::kModeMixed)
		g_system->copyRectToScreen(_textSurface->getBasePtr(0, _textSurface->h - kSplitHeight), _textSurface->pitch, 0, _textSurface->h - kSplitHeight, _textSurface->w, kSplitHeight);

	g_system->updateScreen();
}

void Display_A2_Monitor::renderGraphics() {
	updateGfxSurface();

	if (_mode == kModeGraphics)
		g_system->copyRectToScreen(_gfxSurface->getPixels(), _gfxSurface->pitch, 0, 0, _gfxSurface->w, _gfxSurface->h);
	else if (_mode == kModeMixed)
		g_system->copyRectToScreen(_gfxSurface->getPixels(), _gfxSurface->pitch, 0, 0, _gfxSurface->w, _gfxSurface->h - kSplitHeight);

	g_system->updateScreen();
}

void Display_A2_Monitor::drawChar(byte c, int x, int y) {
	byte *buf = (byte *)_font->getPixels() + y * _font->pitch + x;

	for (uint row = 0; row < 8; ++row) {
		for (uint col = 1; col < 6; ++col) {
			if (font[c][col - 1] & (1 << row)) {
				buf[col * 2] = 1;
				buf[col * 2 + 1] = 1;
			}
		}

		buf += 2 * _font->pitch;
	}
}

void Display_A2_Monitor::createFont() {
	_font = new Graphics::Surface;
	_font->create(16 * 7 * 2, 4 * 8 * 2 * 2, Graphics::PixelFormat::createFormatCLUT8());

	for (uint i = 0; i < 4; ++i)
		for (uint j = 0; j < 16; ++j)
			drawChar(i * 16 + j, j * 7 * 2, i * 8 * 2);

	// Create inverted font
	byte *buf = (byte *)_font->getPixels();
	byte *bufInv = buf + (_font->h / 2) * _font->pitch;

	for (uint row = 0; row < _font->h / 2u; row += 2) {
		for (uint col = 0; col < _font->w; ++col)
			bufInv[col] = (buf[col] ? 0 : 1);

		buf += _font->pitch * 2;
		bufInv += _font->pitch * 2;
	}

	copyEvenSurfaceRows(*_font);
}

void Display_A2_Monitor::showScanlines(bool enable) {
	byte pal[COLOR_PALETTE_ENTRIES * 3];

	g_system->getPaletteManager()->grabPalette(pal, 0, COLOR_PALETTE_ENTRIES);

	if (enable) {
		for (uint i = 0; i < ARRAYSIZE(pal); ++i)
			pal[i] = pal[i] * (100 - SCANLINE_OPACITY) / 100;
	}

	g_system->getPaletteManager()->setPalette(pal, COLOR_PALETTE_ENTRIES, COLOR_PALETTE_ENTRIES);
}

Display_A2::Display_A2() : _frameBuf(nullptr), _showCursor(false) {
	initGraphics(Display_A2::kGfxWidth * 2, Display_A2::kGfxHeight * 2);
}

Display_A2::~Display_A2() {
	delete[] _frameBuf;
}

void Display_A2::init() {
	createTextBuffer(Display_A2::kTextWidth, Display_A2::kTextHeight);

	_frameBuf = new byte[Display_A2::kGfxSize];
	memset(_frameBuf, 0, Display_A2::kGfxSize);
}

void Display_A2::loadFrameBuffer(Common::ReadStream &stream, byte *dst) {
	for (uint j = 0; j < 8; ++j) {
		for (uint i = 0; i < 8; ++i) {
			stream.read(dst, Display_A2::kGfxPitch);
			dst += Display_A2::kGfxPitch * 64;
			stream.read(dst, Display_A2::kGfxPitch);
			dst += Display_A2::kGfxPitch * 64;
			stream.read(dst, Display_A2::kGfxPitch);
			stream.readUint32LE();
			stream.readUint32LE();
			dst -= Display_A2::kGfxPitch * 120;
		}
		dst -= Display_A2::kGfxPitch * 63;
	}

	if (stream.eos() || stream.err())
		error("Failed to read frame buffer");
}

void Display_A2::loadFrameBuffer(Common::ReadStream &stream) {
	loadFrameBuffer(stream, _frameBuf);
}

void Display_A2::putPixel(const Common::Point &p, byte color) {
	byte offset = p.x / 7;
	byte mask = 0x80 | (1 << (p.x % 7));

	// Since white and black are in both palettes, we leave
	// the palette bit alone
	if ((color & 0x7f) == 0x7f || (color & 0x7f) == 0)
		mask &= 0x7f;

	// Adjust colors starting with bits '01' or '10' for
	// odd offsets
	if (offset & 1) {
		byte c = color << 1;
		if (c >= 0x40 && c < 0xc0)
			color ^= 0x7f;
	}

	writeFrameBuffer(p, color, mask);
}

void Display_A2::setPixelByte(const Common::Point &p, byte color) {
	assert(p.x >= 0 && p.x < Display_A2::kGfxWidth && p.y >= 0 && p.y < Display_A2::kGfxHeight);

	_frameBuf[p.y * Display_A2::kGfxPitch + p.x / 7] = color;
}

void Display_A2::setPixelBit(const Common::Point &p, byte color) {
	writeFrameBuffer(p, color, 1 << (p.x % 7));
}

void Display_A2::setPixelPalette(const Common::Point &p, byte color) {
	writeFrameBuffer(p, color, 0x80);
}

byte Display_A2::getPixelByte(const Common::Point &p) const {
	assert(p.x >= 0 && p.x < Display_A2::kGfxWidth && p.y >= 0 && p.y < Display_A2::kGfxHeight);

	return _frameBuf[p.y * Display_A2::kGfxPitch + p.x / 7];
}

bool Display_A2::getPixelBit(const Common::Point &p) const {
	assert(p.x >= 0 && p.x < Display_A2::kGfxWidth && p.y >= 0 && p.y < Display_A2::kGfxHeight);

	byte *b = _frameBuf + p.y * Display_A2::kGfxPitch + p.x / 7;
	return *b & (1 << (p.x % 7));
}

void Display_A2::clear(byte color) {
	byte val = 0;

	byte c = color << 1;
	if (c >= 0x40 && c < 0xc0)
		val = 0x7f;

	for (uint i = 0; i < Display_A2::kGfxSize; ++i) {
		_frameBuf[i] = color;
		color ^= val;
	}
}

// FIXME: This does not currently update the surfaces
void Display_A2::printChar(char c) {
	if (c == Display_A2::asciiToNative('\r'))
		_cursorPos = (_cursorPos / Display_A2::kTextWidth + 1) * Display_A2::kTextWidth;
	else if (c == Display_A2::asciiToNative('\a')) {
		renderText();
		static_cast<AdlEngine *>(g_engine)->bell();
	} else if ((byte)c < 0x80 || (byte)c >= 0xa0) {
		setCharAtCursor(c);
		++_cursorPos;
	}

	if (_cursorPos == Display_A2::kTextWidth * Display_A2::kTextHeight)
		scrollUp();
}

void Display_A2::showCursor(bool enable) {
	_showCursor = enable;
}

void Display_A2::writeFrameBuffer(const Common::Point &p, byte color, byte mask) {
	assert(p.x >= 0 && p.x < Display_A2::kGfxWidth && p.y >= 0 && p.y < Display_A2::kGfxHeight);

	byte *b = _frameBuf + p.y * Display_A2::kGfxPitch + p.x / 7;
	color ^= *b;
	color &= mask;
	*b ^= color;
}

Display_A2 *Display_A2_create() { return new Display_A2_Monitor(); }

} // End of namespace Adl
