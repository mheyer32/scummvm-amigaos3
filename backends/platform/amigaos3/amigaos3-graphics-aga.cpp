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

#include "backends/fs/amigaos3/amigaos3-fs-node.h"
#include "backends/fs/fs-factory.h"
#include "backends/platform/amigaos3/amigaos3-modular.h"
#include "common/config-manager.h"
#include "common/debug.h"
#include "common/scummsys.h"
#include "common/textconsole.h"
#include "graphics/scaler/aspect.h"

#include <proto/commodities.h>

static const OSystem::GraphicsMode s_supportedGraphicsModes[] = {{"1x", "Normal", GFX_NORMAL}, {0, 0, 0}};
static const OSystem::GraphicsMode s_noStretchModes[] = {{"NONE", "Normal", 0}, {nullptr, nullptr, 0 }};

static UWORD emptypointer[] = {
  0x0000, 0x0000, /* reserved, must be NULL */
  0x0000, 0x0000, /* 1 row of image data */
  0x0000, 0x0000  /* reserved, must be NULL */
};

#define OSYSAGA OSystemAGA

OSYSAGA::OSystemAGA() {
	// gDebugLevel = 11;

	_inited = false;

	_fsFactory = NULL;
	_eventManager = NULL;
	_savefileManager = NULL;
	_timerManager = NULL;

	_mixerManager = NULL;
	_audiocdManager = NULL;

	_eventSource = NULL;

	_debugLogger = NULL;
	_warningLogger = NULL;
	_errorLogger = NULL;

	_hardwareWindow = NULL;
	_hardwareScreen = NULL;
	memset(_hardwareScreenBuffer, 0, sizeof(_hardwareScreenBuffer));
	_currentScreenBuffer = 0;

	_currentPalette = NULL,
	_currentShakePos = 0;

	_screenDirty = false;
	_overlayDirty = false;

	_overlayVisible = true;
	_overlayColorMap = NULL;
}

OSYSAGA::~OSystemAGA() {
#ifndef NDEBUG
	debug(1, "OSystem_AmigaOS3::~OSystem_AmigaOS3()");
#endif

	/*if (_splashSurface) {
			_splashSurface->free();
			delete _splashSurface;
			_splashSurface = NULL;
	}*/

	unloadGFXMode();

	if (_agaPalette) {
		free(_agaPalette);
		_agaPalette = NULL;
	}

	if (_overlayPalette) {
		free(_overlayPalette);
		_overlayPalette = NULL;
	}

	if (_gamePalette) {
		free(_gamePalette);
		_gamePalette = NULL;
	}

	if (_currentPalette) {
		free(_currentPalette);
		_currentPalette = NULL;
	}

	_mouseCursor.surface.free();
	_mouseCursorMask.surface.free();

	if (_overlayColorMap) {
		free(_overlayColorMap);
		_overlayColorMap = NULL;
	}

	if (_audiocdManager) {
		delete _audiocdManager;
		_audiocdManager = NULL;
	}

	if (_mixerManager) {
		delete _mixerManager;
		_mixerManager = NULL;
	}

	if (_savefileManager) {
		delete _savefileManager;
		_savefileManager = NULL;
	}

	if (_eventManager) {
		delete _eventManager;
		_eventManager = NULL;
	}

	if (_eventSource) {
		delete _eventSource;
		_eventSource = NULL;
	}

	if (_timerManager) {
		delete _timerManager;
		_timerManager = NULL;
	}

	if (_debugLogger) {
		delete _debugLogger;
		_debugLogger = NULL;
	}

	if (_warningLogger) {
		delete _warningLogger;
		_warningLogger = NULL;
	}

	if (_errorLogger) {
		delete _errorLogger;
		_errorLogger = NULL;
	}

	if (_fsFactory) {
		delete _fsFactory;
		_fsFactory = NULL;
	}
}

bool OSYSAGA::hasFeature(OSystem::Feature f) {
	if (f == OSystem::kFeatureFullscreenMode) {
		return true;
	}

	return false;
}

void OSYSAGA::setFeatureState(OSystem::Feature f, bool enable) {
#ifndef NDEBUG
	debug(4, "OSystem_AmigaOS3::setFeatureState()");
#endif
}

bool OSYSAGA::getFeatureState(OSystem::Feature f) {
#ifndef NDEBUG
	debug(4, "OSystem_AmigaOS3::getFeatureState()");

	assert(_transactionMode == kTransactionNone);
#endif

	switch (f) {
	case OSystem::kFeatureFullscreenMode:
		return true;

	default:
		return false;
	}
}

const OSystem::GraphicsMode *OSYSAGA::getSupportedGraphicsModes() const {
	return s_supportedGraphicsModes;
}

const OSystem::GraphicsMode *OSYSAGA::getSupportedStretchModes() const {
	return s_noStretchModes;
}

int OSYSAGA::getDefaultGraphicsMode() const {
	return GFX_NORMAL;
}

void OSYSAGA::resetGraphicsScale() {
	setGraphicsMode(GFX_NORMAL);
}

void OSYSAGA::beginGFXTransaction() {
#ifndef NDEBUG
	debug(4, "OSystem_AmigaOS3::beginGFXTransaction()");

	assert(_transactionMode == kTransactionNone);
#endif

	_transactionMode = kTransactionActive;

	_transactionDetails.sizeChanged = false;

	// Store the current mode in the old mode.
	_oldVideoMode = _videoMode;
}

OSystem::TransactionError OSYSAGA::endGFXTransaction() {
#ifndef NDEBUG
	debug(4, "OSystem_AmigaOS3::endGFXTransaction()");

	assert(_transactionMode != kTransactionNone);
#endif

	int errors = OSystem::kTransactionSuccess;

	if (_transactionMode == kTransactionRollback) {
		errors |= OSystem::kTransactionSizeChangeFailed;

		// Revert to the last (working) mode.
		_videoMode.screenWidth = _oldVideoMode.screenWidth;
		_videoMode.screenHeight = _oldVideoMode.screenHeight;

		// Stop an endless loop if loadGFXMode() also fails with the old mode.
		_oldVideoMode.setup = false;
	}

	if (_transactionDetails.sizeChanged) {
		unloadGFXMode();
		if (!loadGFXMode()) {
			if (_oldVideoMode.setup) {
				_transactionMode = kTransactionRollback;
				errors |= endGFXTransaction();
			} else {
				error("Could not switch to video mode (%d x %d)", _videoMode.screenWidth, _videoMode.screenHeight);
			}
		}

		// Success.
		SetPointer(_hardwareWindow, emptypointer, 1, 16, 0, 0);

		// Set current cursor position.
		_mouseCursor.x = _hardwareWindow->MouseX;
		_mouseCursor.y = _hardwareWindow->MouseY;

		if (!_overlayPalette) {
			_overlayPalette = (byte *)calloc(3 * 256, sizeof(byte));
			loadOverlayPalette();
		}

		setPalette((byte *)_overlayPalette, 0, 256);

		if (!_overlayColorMap) {
			_overlayColorMap = (byte *)calloc(65536, sizeof(byte));
			loadOverlayColorMap();
		}

		_videoMode.setup = true;

		_screenChangeCount++;
	}

	_transactionMode = kTransactionNone;

	return (OSystem::TransactionError)errors;
}

bool OSYSAGA::setGraphicsMode(int mode) {
#ifndef NDEBUG
	debug(4, "OSystem_AmigaOS3::setGraphicsMode(%d)", mode);
#endif

	assert(_transactionMode == kTransactionActive);

	if (_oldVideoMode.setup && _oldVideoMode.mode == mode) {
		return true;
	}

	if (mode != GFX_NORMAL) {
		warning("Unsupported GFX mode %d", mode);
		return false;
	}

	_videoMode.mode = mode;

	return true;
}

int OSYSAGA::getGraphicsMode() const {
	assert(_transactionMode == kTransactionNone);

	return _videoMode.mode;
}

void OSYSAGA::initSize(uint w, uint h, const Graphics::PixelFormat *format) {
#ifndef NDEBUG
	debug(4, "OSystem_AmigaOS3::initSize(%d, %d)", w, h);

	assert(_transactionMode == kTransactionActive);
#endif

	// Avoid redundant res changes
	/*if (_videoMode.aspectRatioCorrectionRequested) {
			if (w == _videoMode.screenWidth && real2Aspect(h) == _videoMode.scaledHeight) {
			return;
			}
	} else {*/
	if (w == _videoMode.screenWidth && h == _videoMode.screenHeight) {
		return;
	}
	//}

	_videoMode.screenWidth = w;
	_videoMode.screenHeight = h;

	_transactionDetails.sizeChanged = true;
}

bool OSYSAGA::loadGFXMode() {
#ifndef NDEBUG
	debug(4, "OSystem_AmigaOS3::loadGFXMode()");

	debug(5, "New video mode requested - width = %d, height = %d", _videoMode.screenWidth, _videoMode.screenHeight);
#endif

	// Reset.
	//_videoMode.aspectRatioCorrectionInUse = false;

	// Only aspect scale 200 pixel heights.
	/*if (_videoMode.aspectRatioCorrectionRequested && _videoMode.screenHeight == 200) {
		debug(5, "Aspect ratio correction requested and allowed");
		_videoMode.aspectRatioCorrectionInUse = true;

		// Adjust screen height.
				//_videoMode.screenHeight = real2Aspect(_videoMode.screenHeight);
				_videoMode.scaledHeight = real2Aspect(_videoMode.screenHeight);
				debug(5, "Aspect corrected video mode - width = %d, height = %d", _videoMode.screenWidth,
		_videoMode.screenHeight);
		}*/

	// Default overlay size to the same as the (game) screen.
	_videoMode.overlayScreenHeight = _videoMode.screenHeight;

	// Increase for a bigger overlay.
	if (_videoMode.overlayScreenHeight < 256) {
		_videoMode.overlayScreenHeight = 256;
	}
	// Hack - should come from tooltypes
	// FIXME: what is this whole overlay confuzzle? Why do we have multiple members
	// with the same data?
	_videoMode.overlayWidth = _videoMode.screenWidth;
	_videoMode.overlayHeight = _videoMode.overlayScreenHeight;

	_overlayWidth = _videoMode.overlayWidth;
	_overlayHeight = _videoMode.overlayHeight;
	// Create the hardware screen.
	_hardwareScreen = createHardwareScreen(_videoMode.screenWidth, _videoMode.overlayScreenHeight);
	if (!_hardwareScreen) {
		return false;
	}


	// Setup double/triple buffering.
	memset(_hardwareScreenBuffer, 0, sizeof(_hardwareScreenBuffer));
	for (unsigned s = 0; s < NUM_SCREENBUFFERS; ++s) {
		_hardwareScreenBuffer[s] = AllocScreenBuffer(_hardwareScreen, NULL, s == 0 ? SB_SCREEN_BITMAP : 0);
		if (!_hardwareScreenBuffer[s]) {
			// FIXME: free all the stuff that has been allocated!
			return false;
		}
		// WriteChunkyPixels wants a RastPort structure, so create one for each buffer
		InitRastPort(&_screenRastPorts[s]);
		_screenRastPorts[s].BitMap = _hardwareScreenBuffer[s]->sb_BitMap;
	}
	debug(4, "OSystem_AmigaOS3::loadGFXMode() 3");

	// 0 is shown, 1 is the new backbuffer
	_currentScreenBuffer = 1;

	// Create the hardware window.
	_hardwareWindow = createHardwareWindow(_videoMode.screenWidth, _videoMode.overlayScreenHeight, _hardwareScreen);
	if (!_hardwareWindow) {
		return false;
	}

	// Create the surface that contains the 8 bit game data
	_screen.create(_videoMode.screenWidth, _videoMode.screenHeight, Graphics::PixelFormat::createFormatCLUT8());

	// Create the screen used by the scaler/shaker.
	_tmpscreen.create(_videoMode.screenWidth, _videoMode.screenHeight, Graphics::PixelFormat::createFormatCLUT8());

	// Create the 8bit overlay surface
	_overlayscreen8.create(_videoMode.screenWidth, _videoMode.overlayScreenHeight,
						   Graphics::PixelFormat::createFormatCLUT8());

	// Create the 16bit overlay surface
	_overlayscreen16.create(_videoMode.screenWidth, _videoMode.overlayScreenHeight, _overlayFormat);

	return true;
}

struct Screen *OSYSAGA::createHardwareScreen(uint16 width, uint16 height) {
	// Create the hardware screen.
	struct Screen *screen = NULL;
	ULONG modeId = INVALID_ID;

	modeId = BestModeID(BIDTAG_NominalWidth, width, BIDTAG_NominalHeight, height, BIDTAG_DesiredWidth, width,
						BIDTAG_DesiredHeight, height, BIDTAG_Depth, AGA_VIDEO_DEPTH, BIDTAG_MonitorID, PAL_MONITOR_ID,
						TAG_END);

	ULONG errorCode = 0;

	if (modeId != INVALID_ID) {
		screen = OpenScreenTags(NULL, SA_Depth, AGA_VIDEO_DEPTH, SA_DisplayID, modeId, SA_Width, width, SA_Height,
								height, SA_Type, CUSTOMSCREEN, SA_Quiet, TRUE, SA_ShowTitle, FALSE, SA_Draggable, FALSE,
								SA_Exclusive, TRUE, SA_AutoScroll, FALSE, SA_ErrorCode, (Tag)&errorCode, TAG_END);
	}

	return screen;
}

struct Window *OSYSAGA::createHardwareWindow(uint16 width, uint16 height, struct Screen *screen) {
	return OpenWindowTags(NULL, WA_Left, 0, WA_Top, 0, WA_Width, width, WA_Height, height, SA_AutoScroll, FALSE,
						  WA_CustomScreen, (Tag)screen, WA_Backdrop, TRUE, WA_Borderless, TRUE, WA_DragBar, FALSE,
						  WA_Activate, TRUE, WA_SimpleRefresh, TRUE, WA_NoCareRefresh, TRUE, WA_ReportMouse, TRUE,
						  WA_RMBTrap, TRUE, WA_IDCMP, IDCMP_RAWKEY | IDCMP_MOUSEMOVE | IDCMP_MOUSEBUTTONS, TAG_END);
}

void OSYSAGA::unloadGFXMode() {
	_screen.free();
	_tmpscreen.free();

	_overlayscreen8.free();
	_overlayscreen16.free();

	if (_hardwareWindow) {
		ClearPointer(_hardwareWindow);
		CloseWindow(_hardwareWindow);
		_hardwareWindow = NULL;
	}

	if (_hardwareScreenBuffer[0] != NULL) {
		ChangeScreenBuffer(_hardwareScreen, _hardwareScreenBuffer[0]);
		WaitTOF();
		WaitTOF();
		FreeScreenBuffer(_hardwareScreen, _hardwareScreenBuffer[0]);
		_hardwareScreenBuffer[0] = NULL;
	}

	for (unsigned s = 1; s < NUM_SCREENBUFFERS; ++s) {
		if (_hardwareScreenBuffer[s]) {
			FreeScreenBuffer(_hardwareScreen, _hardwareScreenBuffer[s]);
			_hardwareScreenBuffer[s] = NULL;
		}
	}

	if (_hardwareScreen) {
		CloseScreen(_hardwareScreen);
		_hardwareScreen = NULL;
	}
}

void OSYSAGA::setPalette(const byte *colors, uint start, uint num) {
#ifndef NDEBUG
	debug(4, "OSystem_AmigaOS3:setPalette()");

	debug(5, "setPalette() - start = %d", start);
	debug(5, "setPalette() - num = %d", num);

	assert(colors);
#endif

	byte *dst = (byte *)(_currentPalette + (3 * start));
	CopyMem((byte *)colors, dst, (num * 3));

	if (start < _paletteDirtyStart) {
		_paletteDirtyStart = start;
	}

	if (start + num > _paletteDirtyEnd) {
		_paletteDirtyEnd = start + num;
	}
}

void OSYSAGA::grabPalette(byte *colors, uint start, uint num) const {
#ifndef NDEBUG
	assert(colors);
#endif

	CopyMem(_currentPalette + (3 * start), colors, 3 * num);
}

void OSYSAGA::updatePalette() {
#ifndef NDEBUG
	debug(4, "updatePalette()");

	debug(5, "updatePalette() - _paletteDirtyStart = %d", _paletteDirtyStart);
	debug(5, "updatePalette() - _paletteDirtyEnd = %d", _paletteDirtyEnd);
#endif

	uint j = 1;
	byte *color = (byte *)(_currentPalette + 3 * _paletteDirtyStart);

	for (uint16 i = _paletteDirtyStart; i < _paletteDirtyEnd; i++) {
		_agaPalette[j] = color[0] << 24;
		_agaPalette[j + 1] = color[1] << 24;
		_agaPalette[j + 2] = color[2] << 24;

		j += 3;
		color += 3;
	}

	uint16 numberOfEntries = (_paletteDirtyEnd - _paletteDirtyStart);

	_agaPalette[0] = (numberOfEntries << 16) + _paletteDirtyStart;

	// Terminator: NEEDED
	_agaPalette[((numberOfEntries * 3) + 1)] = 0x00000000;

	LoadRGB32(&_hardwareScreen->ViewPort, _agaPalette);

	// Reset.
	_paletteDirtyStart = 256;
	_paletteDirtyEnd = 0;
}

void OSYSAGA::copyRectToScreen(const void *buf, int pitch, int x, int y, int w, int h) {
#ifndef NDEBUG
	debug(4, "copyRectToScreen()");
	debug(5, "copyRectToScreen() - pitch = %d", pitch);
	debug(5, "copyRectToScreen() - x = %d", x);
	debug(5, "copyRectToScreen() - y = %d", y);
	debug(5, "copyRectToScreen() - w = %d", w);
	debug(5, "copyRectToScreen() - h = %d", h);

	assert(_transactionMode == kTransactionNone);
	assert(buf);

	assert(x >= 0 && x < _videoMode.screenWidth);
	assert(y >= 0 && y < _videoMode.screenHeight);
	assert(h > 0 && y + h <= _videoMode.screenHeight);
	assert(w > 0 && x + w <= _videoMode.screenWidth);
#endif

	byte *dst = (byte *)_screen.getBasePtr(x, y);

	if (_videoMode.screenWidth == pitch && pitch == w) {
		CopyMemQuick((byte *)buf, dst, (unsigned)w * h);
	} else {
		const byte *src = (const byte *)buf;
		do {
			CopyMem((void *)src, dst, w);
			src += pitch;
			dst += _videoMode.screenWidth;  //_screen.pitch;
		} while (--h);
	}
}

void OSYSAGA::fillScreen(uint32 col) {
	if (_screen.getPixels()) {
		memset(_screen.getPixels(), (int)col, ((unsigned)_videoMode.screenWidth * _videoMode.screenHeight));
	}
}

void OSYSAGA::updateScreen() {
#ifndef NDEBUG
	debug(9, "OSystem_AmigaOS3::updateScreen()");
#endif

	static UBYTE *src;

	if (_mouseCursor.visible) {
		drawMouse();
	}

	if (_overlayVisible) {
		src = (UBYTE *)_overlayscreen8.getPixels();
		assert(_videoMode.overlayWidth <= _videoMode.screenWidth);
		assert(_videoMode.overlayHeight <= _videoMode.overlayScreenHeight);
		WriteChunkyPixels(&_screenRastPorts[_currentScreenBuffer], 0, 0, _videoMode.overlayWidth - 1,
						  _videoMode.overlayHeight - 1, src, _videoMode.overlayWidth);
	} else {
		if (_currentShakePos != _newShakePos) {
			// Set the 'dirty area' to black.
			memset(_tmpscreen.getBasePtr(0, (_videoMode.screenHeight - _newShakePos)), 0,
				   (_videoMode.screenWidth * _newShakePos));

			src = (UBYTE *)_screen.getBasePtr(0, _newShakePos);
			byte *dst = (byte *)_tmpscreen.getBasePtr(0, 0);

			CopyMemQuick(src, dst, (_videoMode.screenWidth * (_videoMode.screenHeight - _newShakePos)));

			// Reset.
			_currentShakePos = _newShakePos;

			src = (UBYTE *)_tmpscreen.getPixels();
		} else {
			src = (UBYTE *)_screen.getPixels();
		}

		WriteChunkyPixels(&_screenRastPorts[_currentScreenBuffer], 0, 0, _videoMode.screenWidth - 1,
						  _videoMode.screenHeight - 1, src, _videoMode.screenWidth);
	}

	// Check whether the palette was changed.
	if (_paletteDirtyEnd != 0) {
		updatePalette();
	}

	if (_mouseCursor.visible) {
		undrawMouse();
	}

	if (ChangeScreenBuffer(_hardwareScreen, _hardwareScreenBuffer[_currentScreenBuffer])) {
		// Flip.
		_currentScreenBuffer = (_currentScreenBuffer + 1) % NUM_SCREENBUFFERS;
	}
}

void OSYSAGA::setShakePos(int shakeXOffset, int shakeYOffset) {
#ifndef NDEBUG
	assert(_transactionMode == kTransactionNone);
#endif

	_newShakePos = shakeYOffset;
}

#pragma mark -
#pragma mark--- Overlays ---
#pragma mark -

void OSYSAGA::loadOverlayPalette() {
	// Load overlay palette file.
	FILE *paletteFile;

	paletteFile = fopen("overlay.pal", "r");
	if (paletteFile == NULL) {
		error("Could not load the palette file");
	} else {
		// Skip forward 3 rows.
		char temp[100];
		fgets(temp, 100, paletteFile);
		fgets(temp, 100, paletteFile);
		fgets(temp, 100, paletteFile);

		// Read the palette data.
		int red, green, blue;

		byte *color = _overlayPalette;

		while (fscanf(paletteFile, "%d %d %d", &red, &green, &blue) != EOF) {
			color[0] = red;
			color[1] = green;
			color[2] = blue;

			color += 3;
		}

		fclose(paletteFile);
	}
}

void OSYSAGA::loadOverlayColorMap() {
#ifndef NDEBUG
	debug(4, "generateOverlayColorMap()");
#endif

	int color8;

	// Load overlay map file.
	FILE *mapFile;

	mapFile = fopen("overlay.map", "r");
	if (mapFile == NULL) {
		error("Could not load the overlay map file");
	}

	int i = 0;

	while (fscanf(mapFile, "%d", &color8) != EOF) {
		_overlayColorMap[i] = color8;
		i++;
	}

	fclose(mapFile);
}

void OSYSAGA::showOverlay() {
#ifndef NDEBUG
	assert(_transactionMode == kTransactionNone);
#endif

	if (_overlayVisible) {
		return;
	}

	_overlayVisible = true;

	// Make a backup of the current game palette.
	memcpy(_gamePalette, _currentPalette, PALETTE_SIZE);

	// Set the overlay palette.
	setPalette((byte *)_overlayPalette, 0, 256);
}

void OSYSAGA::hideOverlay() {
#ifndef NDEBUG
	assert(_transactionMode == kTransactionNone);
#endif

	if (!_overlayVisible) {
		return;
	}

	clearOverlay();

	UBYTE *src = (UBYTE *)_overlayscreen8.getPixels();

	// since the overlay is taller than the game screen height, need to make sure
	// we're clearing all the screens (in particular the bottoms)
	for (unsigned s = 0; s < NUM_SCREENBUFFERS; ++s) {
		ClearScreen(&_screenRastPorts[_currentScreenBuffer]);
		ChangeScreenBuffer(_hardwareScreen, _hardwareScreenBuffer[_currentScreenBuffer]);
		_currentScreenBuffer = (_currentScreenBuffer + 1) % NUM_SCREENBUFFERS;
	}

	_overlayVisible = false;

	// Reset the game palette.
	setPalette((byte *)_gamePalette, 0, 256);
}

void OSYSAGA::clearOverlay() {
	if (!_overlayVisible) {
		return;
	}

	// Set the background to black.
	byte *src = (byte *)_overlayscreen8.getPixels();
	memset(src, 0, (_videoMode.screenWidth * _videoMode.overlayScreenHeight));
}

void OSYSAGA::grabOverlay(void *buf, int pitch) {
#ifndef NDEBUG
	assert(_transactionMode == kTransactionNone);
#endif

	// Grab the overlay.
	memcpy(buf, _overlayscreen16.getPixels(),
		   (_videoMode.screenWidth * _videoMode.overlayScreenHeight) * _overlayscreen16.format.bytesPerPixel);
}

void OSYSAGA::copyRectToOverlay(const void *buf, int _pitch, int _x, int _y, int _w, int _h) {
#ifndef NDEBUG
	debug(4, "copyRectToOverlay()");

	assert(_transactionMode == kTransactionNone);
#endif

	//FIXME: we're not using pitch
	uint16 pitch = _pitch, x = _x, y = _y, w = _w, h = _h;

	// Clip the coordinates
	if (x + w > _videoMode.screenWidth) {
		w = _videoMode.screenWidth - x;
	}

	if (y + h > _videoMode.overlayScreenHeight) {
		h = _videoMode.overlayScreenHeight - y;
	}

	if (w == 0 || h == 0) {
		return;
	}

	const OverlayColor *src = (const OverlayColor *)buf;
	byte *dst = (byte *)_overlayscreen8.getBasePtr(x, y);

	OverlayColor color16;
	byte color8;

	for (uint16 r = 0; r < h; r++) {
		for (uint16 c = 0; c < w; c++) {
			color16 = *src;

			color8 = _overlayColorMap[color16];

			*dst = color8;

			// Add a column.
			src++;
			dst++;
		}

		// add a row.
		dst += (_videoMode.screenWidth - w);
		src += (_videoMode.screenWidth - w);
	}
}

#pragma mark -
#pragma mark--- Mouse ---
#pragma mark -

bool OSYSAGA::showMouse(bool visible) {
	if (_mouseCursor.visible == visible) {
		return visible;
	}

	bool last = _mouseCursor.visible;
	_mouseCursor.visible = visible;

	return last;
}

void OSYSAGA::warpMouse(int x, int y) {
	struct InputEvent ie;
	struct IEPointerPixel pp;

	ie.ie_NextEvent = NULL;
	ie.ie_Class = IECLASS_NEWPOINTERPOS;
	ie.ie_SubClass = IESUBCLASS_PIXEL;
	ie.ie_Code = 0;
	ie.ie_Qualifier = 0;
	ie.ie_EventAddress = &pp;

	pp.iepp_Screen = _hardwareScreen;
	pp.iepp_Position.X = x;
	pp.iepp_Position.Y = y;

	AddIEvents(&ie);
}

void OSYSAGA::setMouseCursor(const void *buf, uint w, uint h, int hotspot_x, int hotspot_y, uint32 keycolor,
									  bool dontScale, const Graphics::PixelFormat *format) {
#ifndef NDEBUG
	debug(4, "OSystem_AmigaOS3::setMouseCursor(w = %d, h = %d)", w, h);
#endif

	// Sanity check.
	if (w == 0 || h == 0 || !buf) {
		return;
	}

	// Check to see if we need to recreate the surfaces.
	if (w != _mouseCursor.w || h != _mouseCursor.h) {
		_mouseCursor.surface.create(w, h, Graphics::PixelFormat::createFormatCLUT8());
		_mouseCursorMask.surface.create(w, h, Graphics::PixelFormat::createFormatCLUT8());
	}

	_mouseCursor.w = w;
	_mouseCursor.h = h;
	_mouseCursor.hotX = hotspot_x;
	_mouseCursor.hotY = hotspot_y;
	_mouseCursor.keyColor = keycolor;

	CopyMem((void *)buf, _mouseCursor.surface.getPixels(), (unsigned)w * h);
}

void OSYSAGA::setMouseCursorPosition(uint16 x, uint16 y) {
	_mouseCursor.x = x;
	_mouseCursor.y = y;
}

void OSYSAGA::drawMouse() {
#ifndef NDEBUG
	debug(4, "OSystem_AmigaOS3::drawMouse()");
#endif

	int16 w = _mouseCursor.w;
	int16 h = _mouseCursor.h;

	int16 x = (_mouseCursor.x - _mouseCursor.hotX);
	int16 y = (_mouseCursor.y - _mouseCursor.hotY);

	byte *mousePixels = (byte *)_mouseCursor.surface.getPixels();

	// Clip the coordinates
	if (x < 0) {
		w += x;
		mousePixels -= x;
		x = 0;
	}

	if (y < 0) {
		h += y;
		mousePixels -= ((unsigned)y * _mouseCursor.surface.pitch);
		y = 0;
	}

	if (x + w > _videoMode.screenWidth) {
		w = _videoMode.screenWidth - x;
	}

	if (_overlayVisible) {
		if (y + h > _videoMode.overlayScreenHeight) {
			h = _videoMode.overlayScreenHeight - y;
		}
	} else {
		if (y + h > _videoMode.screenHeight) {
			h = _videoMode.screenHeight - y;
		}
	}

	if (w <= 0 || h <= 0) {
		// Nothing to do.
		return;
	}

	// Setup the cursor mask.
	_mouseCursorMask.x = x;
	_mouseCursorMask.y = y;
	_mouseCursorMask.w = w;
	_mouseCursorMask.h = h;

	byte *maskPixels = (byte *)_mouseCursorMask.surface.getPixels();

	// Set the starting point of the screen we will be drawing to.
	byte *screenPixels = NULL;

	if (_overlayVisible) {
		screenPixels = (byte *)_overlayscreen8.getBasePtr(x, y);
	} else {
		screenPixels = (byte *)_screen.getBasePtr(x, y);
	}

	// Draw it.
	byte color;

	do {
		// Save a copy of this row before it's overwritten.
		CopyMem(screenPixels, maskPixels, w);

		for (uint16 c = 0; c < w; c++) {
			color = *mousePixels;

			if (color != _mouseCursor.keyColor) {
				// Set the color.
				*screenPixels = color;
			}

			// Add a column.
			mousePixels++;
			screenPixels++;
		}

		// add a row.
		maskPixels += w;
		mousePixels += (_mouseCursor.surface.pitch - w);
		screenPixels += (_videoMode.screenWidth - w);
	} while (--h);
}

void OSYSAGA::undrawMouse() {
#ifndef NDEBUG
	debug(4, "OSystem_AmigaOS3::undrawMouse()");
#endif

	byte *dst = NULL;

	if (_overlayVisible) {
		dst = (byte *)_overlayscreen8.getBasePtr(_mouseCursorMask.x, _mouseCursorMask.y);
	} else {
		dst = (byte *)_screen.getBasePtr(_mouseCursorMask.x, _mouseCursorMask.y);
	}

	byte *src = (byte *)_mouseCursorMask.surface.getPixels();

	for (uint16 i = 0; i < _mouseCursorMask.h; i++) {
		CopyMem(src, dst, _mouseCursorMask.w);
		dst += _videoMode.screenWidth;
		src += _mouseCursorMask.w;
	}
}

struct Window *OSYSAGA::getHardwareWindow() {
	return _hardwareWindow;
}

void OSYSAGA::displayActivityIconOnOSD(const Graphics::Surface *icon) {
	// TODO - UNIMPLEMENTED
}
