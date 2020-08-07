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
#include <proto/cybergraphics.h>
#include <proto/graphics.h>

#include <cybergraphx/cybergraphics.h>

#include <stdio.h>

extern struct Library *CyberGfxBase;

static UWORD emptypointer[] = {
  0x0000, 0x0000, /* reserved, must be NULL */
  0x0000, 0x0000, /* 1 row of image data */
  0x0000, 0x0000  /* reserved, must be NULL */
};

#define OSYSCGX OSystemCGX

/** Hardware screen */
static struct Screen *_hardwareGameScreen = NULL;

/** Hardware window */
static struct Window *_hardwareGameWindow = NULL;

/** Hardware screen */
static struct Screen *_hardwareOverlayScreen = NULL;

/** Hardware window */
static struct Window *_hardwareOverlayWindow = NULL;

#define CGX_VIDEO_DEPTH 8

static const OSystem::GraphicsMode s_supportedGraphicsModes[] = {{"1x", "Normal", GFX_NORMAL}, {0, 0, 0}};
static const OSystem::GraphicsMode s_noStretchModes[] = {{"NONE", "Normal", 0}, {nullptr, nullptr, 0 }};

OSYSCGX::OSystemCGX() {
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

OSYSCGX::~OSystemCGX() {
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

bool OSYSCGX::hasFeature(OSystem::Feature f) {
	/*if (f == OSystem::kFeatureAspectRatioCorrection) {
		return true;
		}*/

	if (f == OSystem::kFeatureFullscreenMode) {
		return true;
	}

	return false;
}

void OSYSCGX::setFeatureState(OSystem::Feature f, bool enable) {
#ifndef NDEBUG
	debug(4, "OSystem_AmigaOS3::setFeatureState()");
#endif

	/*switch (f) {
		case OSystem::kFeatureAspectRatioCorrection:
		_videoMode.aspectRatioCorrectionRequested = enable;
		break;
		}*/
}

bool OSYSCGX::getFeatureState(OSystem::Feature f) {
#ifndef NDEBUG
	debug(4, "OSystem_AmigaOS3::getFeatureState()");

	assert(_transactionMode == kTransactionNone);
#endif

	switch (f) {
	case OSystem::kFeatureFullscreenMode:
		return true;

		// case OSystem::kFeatureAspectRatioCorrection:
		//  return _videoMode.aspectRatioCorrectionRequested;

	default:
		return false;
	}
}

const OSystem::GraphicsMode *OSYSCGX::getSupportedGraphicsModes() const {
	return s_supportedGraphicsModes;
}

const OSystem::GraphicsMode *OSYSCGX::getSupportedStretchModes() const {
	return s_noStretchModes;
}

int OSYSCGX::getDefaultGraphicsMode() const {
	return GFX_NORMAL;
}

void OSYSCGX::resetGraphicsScale() {
	setGraphicsMode(getDefaultGraphicsMode());
}

void OSYSCGX::beginGFXTransaction() {
#ifndef NDEBUG
	debug(4, "OSystem_AmigaOS3::beginGFXTransaction()");

	assert(_transactionMode == kTransactionNone);
#endif

	_transactionMode = kTransactionActive;

	_transactionDetails.sizeChanged = false;

	// Store the current mode in the old mode.
	_oldVideoMode = _videoMode;
}

OSystem::TransactionError OSYSCGX::endGFXTransaction() {
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

		if (_overlayVisible) {
			ScreenToFront(_hardwareOverlayScreen);
			ActivateWindow(_hardwareOverlayWindow);

			// Set current cursor position.
			_mouseCursor.x = _hardwareOverlayWindow->MouseX;
			_mouseCursor.y = _hardwareOverlayWindow->MouseY;
		} else {
			ScreenToFront(_hardwareGameScreen);
			ActivateWindow(_hardwareGameWindow);

			// Set current cursor position.
			_mouseCursor.x = _hardwareGameWindow->MouseX;
			_mouseCursor.y = _hardwareGameWindow->MouseY;
		}

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

bool OSYSCGX::setGraphicsMode(int mode) {
#ifndef NDEBUG
	debug(4, "OSystem_AmigaOS3::setGraphicsMode(%d)", mode);
#endif

	printf("OSystem_AmigaOS3::setGraphicsMode\n");

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

int OSYSCGX::getGraphicsMode() const {
	assert(_transactionMode == kTransactionNone);

	return _videoMode.mode;
}

void OSYSCGX::initSize(uint w, uint h, const Graphics::PixelFormat *format) {
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

	_overlayWidth = 640;
	_overlayHeight = 480;

	_transactionDetails.sizeChanged = true;
}

bool OSYSCGX::loadGFXMode() {
#ifndef NDEBUG
	debug(4, "OSystem_AmigaOS3::loadGFXMode()");

	debug(5, "New video mode requested - width = %d, height = %d", _videoMode.screenWidth, _videoMode.screenHeight);
#endif

	printf("loadGFXMode()");

	// Hack - should come from tooltypes
	_videoMode.overlayWidth = _overlayWidth;
	_videoMode.overlayHeight = _overlayHeight;

	// Overlay cannot be smaller than the game screen.
	/*if ((_videoMode.overlayWidth < _videoMode.screenWidth) || (_videoMode.overlayHeight < _videoMode.screenHeight)) {
		_videoMode.overlayWidth = _videoMode.screenWidth;
		_videoMode.overlayHeight = _videoMode.screenHeight;
		}*/

	// Create the hardware screen.
	_hardwareOverlayScreen = createHardwareScreen(_videoMode.overlayWidth, _videoMode.overlayHeight);
	if (!_hardwareOverlayScreen) {
		return false;
	}
	uint pitch = GetCyberMapAttr(_hardwareOverlayScreen->RastPort.BitMap, CYBRMATTR_XMOD);
	debug("OVERLAY PITCH: %d\n", pitch);
	_videoMode.overlayBytesPerRow = pitch;

	// Create the hardware window.
	_hardwareOverlayWindow =
	  createHardwareWindow(_videoMode.overlayWidth, _videoMode.overlayHeight, _hardwareOverlayScreen);
	if (!_hardwareOverlayWindow) {
		return false;
	}

	SetPointer(_hardwareOverlayWindow, emptypointer, 1, 16, 0, 0);

	// Create the hardware screen.
	_hardwareGameScreen =
	  createHardwareScreen(_videoMode.screenWidth, (_videoMode.screenHeight < 240) ? 240 : _videoMode.screenHeight);
	if (!_hardwareGameScreen) {
		return false;
	}
	pitch = GetCyberMapAttr(_hardwareGameScreen->RastPort.BitMap, CYBRMATTR_XMOD);
	debug("GAME PITCH: %d\n", pitch);
	_videoMode.bytesPerRow = pitch;

	// Create the hardware window.
	_hardwareGameWindow = createHardwareWindow(
	  _videoMode.screenWidth, (_videoMode.screenHeight < 240) ? 240 : _videoMode.screenHeight, _hardwareGameScreen);
	if (!_hardwareGameWindow) {
		return false;
	}

	SetPointer(_hardwareGameWindow, emptypointer, 1, 16, 0, 0);

	// Create the surface that contains the 8 bit game data
	_screen.create(_videoMode.screenWidth, _videoMode.screenHeight, Graphics::PixelFormat::createFormatCLUT8());

	// Create the screen used by the scaler/shaker.
	_tmpscreen.create(_videoMode.screenWidth, _videoMode.screenHeight, Graphics::PixelFormat::createFormatCLUT8());

	// Create the 8bit overlay surface
	_overlayscreen8.create(_videoMode.overlayWidth, _videoMode.overlayHeight,
						   Graphics::PixelFormat::createFormatCLUT8());

	// Create the 16bit overlay surface
	_overlayscreen16.create(_videoMode.overlayWidth, _videoMode.overlayHeight, _overlayFormat);

	_screenDirty = true;
	_overlayDirty = true;

	return true;
}

struct Screen *OSYSCGX::createHardwareScreen(uint16 width, uint16 height) {
	// Create the hardware screen.
	struct Screen *screen = NULL;
	ULONG modeId = INVALID_ID;

	printf("OSystem_AmigaOS3::createHardwareScreen(%d,%d)\n", width, height);

	if (width == 0 || width > 4096 || height == 0 || height > 4096) {
		width = 320;
		height = 200;
	}

	// if (width==320) width=640;
	// if (height==240) height=480;

	modeId = BestCModeIDTags(CYBRBIDTG_Depth, CGX_VIDEO_DEPTH, CYBRBIDTG_NominalWidth, width, CYBRBIDTG_NominalHeight,
							 height, TAG_DONE);

	debug("modeId: %x\n", modeId);

	// Verify the mode choosen.
	if (modeId != INVALID_ID) {
		if (GetCyberIDAttr(CYBRIDATTR_DEPTH, modeId) != CGX_VIDEO_DEPTH) {
			modeId = INVALID_ID;
			debug("depth mismatch %d\n", GetCyberIDAttr(CYBRIDATTR_DEPTH, modeId));
		}

		/*if (GetCyberIDAttr(CYBRIDATTR_WIDTH, modeId) != width) {
			modeId = INVALID_ID;
			printf("width mismatch %d\n",GetCyberIDAttr(CYBRIDATTR_WIDTH, modeId));
		}

		if (GetCyberIDAttr(CYBRIDATTR_HEIGHT, modeId) != height) {
			modeId = INVALID_ID;
			printf("height mismatch %d\n", GetCyberIDAttr(CYBRIDATTR_HEIGHT, modeId));
		}*/
	}

	if (modeId == INVALID_ID) {
		debug("Couldn't find a Screen Mode for requested mode\n");
	}

	if (modeId != INVALID_ID) {
		debug("OpenScreenTags()\n");
		screen = OpenScreenTags(NULL, SA_Depth, CGX_VIDEO_DEPTH, SA_DisplayID, modeId, SA_Width, width, SA_Height,
								height, SA_Type, CUSTOMSCREEN, SA_Quiet, TRUE, SA_ShowTitle, FALSE, SA_Draggable, FALSE,
								SA_Exclusive, TRUE, SA_AutoScroll, FALSE, TAG_END);
	}

	return screen;
}

struct Window *OSYSCGX::createHardwareWindow(uint16 width, uint16 height, struct Screen *screen) {
	return OpenWindowTags(NULL, WA_Left, 0, WA_Top, 0, WA_Width, width, WA_Height, height, SA_AutoScroll, FALSE,
						  WA_CustomScreen, (ULONG)screen, WA_Backdrop, TRUE, WA_Borderless, TRUE, WA_DragBar, FALSE,
						  WA_Activate, TRUE, WA_SimpleRefresh, TRUE, WA_NoCareRefresh, TRUE, WA_ReportMouse, TRUE,
						  WA_RMBTrap, TRUE, WA_IDCMP, IDCMP_RAWKEY | IDCMP_MOUSEMOVE | IDCMP_MOUSEBUTTONS, TAG_END);
}

void OSYSCGX::unloadGFXMode() {
	_screen.free();
	_tmpscreen.free();

	_overlayscreen8.free();
	_overlayscreen16.free();

	if (_hardwareGameWindow) {
		ClearPointer(_hardwareGameWindow);
		CloseWindow(_hardwareGameWindow);
		_hardwareGameWindow = NULL;
	}

	if (_hardwareGameScreen) {
		CloseScreen(_hardwareGameScreen);
		_hardwareGameScreen = NULL;
	}

	if (_hardwareOverlayWindow) {
		ClearPointer(_hardwareOverlayWindow);
		CloseWindow(_hardwareOverlayWindow);
		_hardwareOverlayWindow = NULL;
	}

	if (_hardwareOverlayScreen) {
		CloseScreen(_hardwareOverlayScreen);
		_hardwareOverlayScreen = NULL;
	}
}

void OSYSCGX::setPalette(const byte *colors, uint start, uint num) {
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

void OSYSCGX::grabPalette(byte *colors, uint start, uint num) const {
#ifndef NDEBUG
	assert(colors);
#endif

	CopyMem(_currentPalette + (3 * start), colors, 3 * num);
}

void OSYSCGX::updatePalette() {
#ifndef NDEBUG
	debug(4, "updatePalette()");

	debug(5, "updatePalette() - _paletteDirtyStart = %d", _paletteDirtyStart);
	debug(5, "updatePalette() - _paletteDirtyEnd = %d", _paletteDirtyEnd);
#endif

	uint j = 1;
	byte *color = (byte *)(_currentPalette + 3 * _paletteDirtyStart);

	for (uint i = _paletteDirtyStart; i < _paletteDirtyEnd; i++) {
		_agaPalette[j] = color[0] << 24;
		_agaPalette[j + 1] = color[1] << 24;
		_agaPalette[j + 2] = color[2] << 24;

		j += 3;
		color += 3;
	}

	uint numberOfEntries = (_paletteDirtyEnd - _paletteDirtyStart);

	_agaPalette[0] = (numberOfEntries << 16) + _paletteDirtyStart;

	// Terminator: NEEDED
	_agaPalette[((numberOfEntries * 3) + 1)] = 0x00000000;

	if (_overlayVisible) {
		LoadRGB32(&_hardwareOverlayScreen->ViewPort, _agaPalette);
	} else {
		LoadRGB32(&_hardwareGameScreen->ViewPort, _agaPalette);
	}

	// Reset.
	_paletteDirtyStart = 256;
	_paletteDirtyEnd = 0;
}

void OSYSCGX::copyRectToScreen(const void *buf, int pitch, int x, int y, int w, int h) {
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

	byte *dst = ((byte *)_screen.getBasePtr(0, 0)) + _videoMode.bytesPerRow * y + x;

	/*if (_videoMode.screenWidth == pitch && pitch == w) {
		CopyMemQuick((byte*)buf, dst, w * h);
		} else {*/
	const byte *src = (const byte *)buf;
	do {
		CopyMem((void *)src, dst, w);
		src += pitch;
		dst += _videoMode.bytesPerRow;
	} while (--h);
	//}

	_screenDirty = true;
}

void OSYSCGX::fillScreen(uint32 col) {
	if (_screen.getPixels()) {
		memset(_screen.getPixels(), (int)col, (_videoMode.bytesPerRow * _videoMode.screenHeight));
		_screenDirty = true;
	}
}

void OSYSCGX::updateScreen() {
#ifndef NDEBUG
	debug(9, "OSystem_AmigaOS3::updateScreen()");
#endif

	UBYTE *src;

	if (_overlayVisible && _overlayDirty) {
		if (_mouseCursor.visible) {
			drawMouse();
		}

		UBYTE *base_address;
		APTR video_bitmap_handle = LockBitMapTags(_hardwareOverlayScreen->ViewPort.RasInfo->BitMap, LBMI_BASEADDRESS,
												  (ULONG)&base_address, TAG_DONE);
		if (video_bitmap_handle) {
			// CopyMemQuick((UBYTE*)_overlayscreen8.getPixels(), base_address, (_videoMode.overlayWidth *
			// _videoMode.overlayHeight));

			UBYTE *dst = base_address;
			UBYTE *src = (UBYTE *)_overlayscreen8.getPixels();

			for (uint r = 0; r < _videoMode.overlayHeight; r++) {
				CopyMem(src, dst, _videoMode.overlayWidth);

				// add a row.
				dst += (_videoMode.overlayBytesPerRow);
				src += (_videoMode.overlayWidth);
			}

			UnLockBitMap(video_bitmap_handle);
			video_bitmap_handle = NULL;
		}

		if (_mouseCursor.visible) {
			undrawMouse();
		}

		_overlayDirty = false;
	} else if (_screenDirty) {
		if (_mouseCursor.visible) {
			drawMouse();
		}

		if (_currentShakePos != _newShakePos) {
			// Set the 'dirty area' to black.
			/*memset(_tmpscreen.getBasePtr(0, (_videoMode.screenHeight - _newShakePos)), 0, (_videoMode.bytesPerRow *
			_newShakePos));

			src = (UBYTE*)_screen.getBasePtr(0, _newShakePos);
			byte *dst = (byte*)_tmpscreen.getBasePtr(0, 0);

			// TODO
			CopyMemQuick(src, dst, (_videoMode.bytesPerRow * (_videoMode.screenHeight - _newShakePos)));

			// Reset.
			_currentShakePos = _newShakePos;*/

			src = (UBYTE *)_tmpscreen.getPixels();
		} else {
			src = (UBYTE *)_screen.getPixels();
		}

		UBYTE *base_address;
		APTR video_bitmap_handle = LockBitMapTags(_hardwareGameScreen->ViewPort.RasInfo->BitMap, LBMI_BASEADDRESS,
												  (ULONG)&base_address, TAG_DONE);
		if (video_bitmap_handle) {
			CopyMemQuick(src, base_address, (_videoMode.bytesPerRow * _videoMode.screenHeight));
			UnLockBitMap(video_bitmap_handle);
			video_bitmap_handle = NULL;
		}

		if (_mouseCursor.visible) {
			undrawMouse();
		}

		_screenDirty = false;
	}

	// Check whether the palette was changed.
	if (_paletteDirtyEnd != 0) {
		updatePalette();
	}
}

void OSYSCGX::setShakePos(int shakeX, int shakeY) {
#ifndef NDEBUG
	assert(_transactionMode == kTransactionNone);
#endif

	_newShakePos = shakeX;
}

#pragma mark -
#pragma mark--- Overlays ---
#pragma mark -

void OSYSCGX::loadOverlayPalette() {
	// Load overlay palette file.
	FILE *paletteFile;

	paletteFile = fopen("overlay.pal", "r");
	if (paletteFile == NULL) {
		printf("Could not load the palette file\n");
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

void OSYSCGX::loadOverlayColorMap() {
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

void OSYSCGX::showOverlay() {
#ifndef NDEBUG
	assert(_transactionMode == kTransactionNone);
#endif

	if (_overlayVisible) {
		return;
	}

	ScreenToFront(_hardwareOverlayScreen);
	ActivateWindow(_hardwareOverlayWindow);

	_overlayVisible = true;
}

void OSYSCGX::hideOverlay() {
#ifndef NDEBUG
	assert(_transactionMode == kTransactionNone);
#endif
	APTR video_bitmap_handle;
	UBYTE *base_address;

	if (!_overlayVisible) {
		return;
	}

	clearOverlay();

	video_bitmap_handle = LockBitMapTags(_hardwareOverlayScreen->ViewPort.RasInfo->BitMap, LBMI_BASEADDRESS,
										 (ULONG)&base_address, TAG_DONE);
	if (video_bitmap_handle) {
		CopyMemQuick((UBYTE *)_overlayscreen8.getPixels(), base_address,
					 (_videoMode.overlayWidth * _videoMode.overlayHeight));
		UnLockBitMap(video_bitmap_handle);
		video_bitmap_handle = NULL;
	}

	ScreenToFront(_hardwareGameScreen);
	ActivateWindow(_hardwareGameWindow);

	_overlayVisible = false;
}

void OSYSCGX::clearOverlay() {
	if (!_overlayVisible) {
		return;
	}

	// Set the background to black.
	byte *src = (byte *)_overlayscreen8.getPixels();
	memset(src, 0, (_videoMode.overlayWidth * _videoMode.overlayHeight));
	_overlayDirty = true;
}

void OSYSCGX::grabOverlay(void *buf, int pitch) {
#ifndef NDEBUG
	assert(_transactionMode == kTransactionNone);
#endif

	// Grab the overlay.
	memcpy(buf, _overlayscreen16.getPixels(),
		   (_videoMode.overlayWidth * _videoMode.overlayHeight) * _overlayscreen16.format.bytesPerPixel);
}

void OSYSCGX::copyRectToOverlay(const void *buf, int pitch, int x, int y, int w, int h) {
#ifndef NDEBUG
	debug(4, "copyRectToOverlay()");

	assert(_transactionMode == kTransactionNone);
#endif

	// Clip the coordinates
	if (x < 0) {
		return;
	}

	if (y < 0) {
		return;
	}

	if (w > _videoMode.overlayWidth - x) {
		w = _videoMode.overlayWidth - x;
	}

	if (h > _videoMode.overlayHeight - y) {
		h = _videoMode.overlayHeight - y;
	}

	if (w <= 0 || h <= 0) {
		return;
	}

	const OverlayColor *src = (const OverlayColor *)buf;
	byte *dst = (byte *)_overlayscreen8.getBasePtr(x, y);  // + y*_videoMode.overlayBytesPerRow;

	OverlayColor color16;
	byte color8;

	for (uint r = 0; r < h; r++) {
		for (uint c = 0; c < w; c++) {
			color16 = *src;

			color8 = _overlayColorMap[color16];

			*dst = color8;

			// Add a column.
			src++;
			dst++;
		}

		// add a row.
		dst += (_videoMode.overlayWidth - w);
		src += (_videoMode.overlayWidth - w);
	}

	_overlayDirty = true;
}

struct Window *OSYSCGX::getHardwareWindow() {
	if (_overlayVisible) {
		return _hardwareOverlayWindow;
	}

	return _hardwareGameWindow;
}

#pragma mark -
#pragma mark--- Mouse ---
#pragma mark -

bool OSYSCGX::showMouse(bool visible) {
	if (_mouseCursor.visible == visible) {
		return visible;
	}

	if (_mouseCursor.visible != visible) {
		if (_overlayVisible) {
			_overlayDirty = true;
		} else {
			_screenDirty = true;
		}
	}

	bool last = _mouseCursor.visible;
	_mouseCursor.visible = visible;

	return last;
}

void OSYSCGX::warpMouse(int x, int y) {
	struct InputEvent ie;
	struct IEPointerPixel pp;

	ie.ie_NextEvent = NULL;
	ie.ie_Class = IECLASS_NEWPOINTERPOS;
	ie.ie_SubClass = IESUBCLASS_PIXEL;
	ie.ie_Code = 0;
	ie.ie_Qualifier = 0;
	ie.ie_EventAddress = &pp;

	pp.iepp_Screen = _hardwareGameScreen;  // Only used by games!
	pp.iepp_Position.X = x;
	pp.iepp_Position.Y = y;

	AddIEvents(&ie);
}

void OSYSCGX::setMouseCursor(const void *buf, uint w, uint h, int hotspot_x, int hotspot_y, uint32 keycolor,
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

	MouseCursor oldMouseCursor = _mouseCursor;

	_mouseCursor.w = w;
	_mouseCursor.h = h;
	_mouseCursor.hotX = hotspot_x;
	_mouseCursor.hotY = hotspot_y;
	_mouseCursor.keyColor = keycolor;

	if (memcmp(&oldMouseCursor, &_mouseCursor, sizeof(MouseCursor)) != 0) {
		if (_overlayVisible) {
			_overlayDirty = true;
		} else {
			_screenDirty = true;
		}
	}
	CopyMem((void *)buf, _mouseCursor.surface.getPixels(), w * h);
}

void OSYSCGX::setMouseCursorPosition(uint16 x, uint16 y) {
	MouseCursor oldMouseCursor = _mouseCursor;

	_mouseCursor.x = x;
	_mouseCursor.y = y;

	if (memcmp(&oldMouseCursor, &_mouseCursor, sizeof(MouseCursor)) != 0) {
		if (_overlayVisible) {
			_overlayDirty = true;
		} else {
			_screenDirty = true;
		}
	}
}

void OSYSCGX::drawMouse() {
#ifndef NDEBUG
	debug(4, "OSystem_AmigaOS3::drawMouse()");
#endif

	uint w = _mouseCursor.w;
	uint h = _mouseCursor.h;

	int x = (_mouseCursor.x - _mouseCursor.hotX);
	int y = (_mouseCursor.y - _mouseCursor.hotY);

	uint width, height;

	if (_overlayVisible) {
		width = _videoMode.overlayWidth;
		height = _videoMode.overlayHeight;
	} else {
		width = _videoMode.screenWidth;
		height = _videoMode.screenHeight;
	}

	byte *mousePixels = (byte *)_mouseCursor.surface.getPixels();

	// Clip the coordinates
	if (x < 0) {
		w += x;
		mousePixels -= x;
		x = 0;
	}

	if (y < 0) {
		h += y;
		mousePixels -= (y * _mouseCursor.surface.pitch);
		y = 0;
	}

	if (w > width - x) {
		w = width - x;
	}

	if (h > height - y) {
		h = height - y;
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
	uint pitch = 0;

	if (_overlayVisible) {
		screenPixels = (byte *)_overlayscreen8.getBasePtr(x, y);  // + y*_videoMode.overlayBytesPerRow;
		pitch = _videoMode.overlayWidth;						  // BytesPerRow;
	} else {
		screenPixels = (byte *)_screen.getBasePtr(x, 0) + y * _videoMode.bytesPerRow;
		pitch = _videoMode.bytesPerRow;
	}

	// Draw it.
	byte color;

	do {
		// Save a copy of this row before it's overwritten.
		CopyMem(screenPixels, maskPixels, w);

		for (uint c = 0; c < w; c++) {
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
		screenPixels += (pitch - w);
	} while (--h);
}

void OSYSCGX::undrawMouse() {
#ifndef NDEBUG
	debug(4, "OSystem_AmigaOS3::undrawMouse()");
#endif

	byte *dst = NULL;
	uint pitch = 0;

	if (_overlayVisible) {
		dst = (byte *)_overlayscreen8.getBasePtr(
		  _mouseCursorMask.x, _mouseCursorMask.y);  // + _mouseCursorMask.y*_videoMode.overlayBytesPerRow;
		pitch = _videoMode.overlayWidth;			// overlayBytesPerRow;
	} else {
		dst = (byte *)_screen.getBasePtr(_mouseCursorMask.x, 0) + _mouseCursorMask.y * _videoMode.bytesPerRow;
		pitch = _videoMode.bytesPerRow;
	}

	byte *src = (byte *)_mouseCursorMask.surface.getPixels();
	for (uint i = 0; i < _mouseCursorMask.h; i++) {
		CopyMem(src, dst, _mouseCursorMask.w);
		dst += pitch;
		src += _mouseCursorMask.w;
	}
}

void OSYSCGX::displayActivityIconOnOSD(const Graphics::Surface *icon) {
	// TODO - UNIMPLEMENTED
}
