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

#ifndef PLATFORM_AMIGAOS3_H
#define PLATFORM_AMIGAOS3_H

#include <time.h>

#include "audio/mixer_intern.h"
#include "backends/events/amigaos3/amigaos3-events.h"
#include "backends/log/log.h"
#include "backends/mixer/amigaos3/amigaos3-mixer.h"

#include "common/scummsys.h"
#include "common/system.h"
#include "graphics/palette.h"
#include "graphics/surface.h"

// Amiga includes.
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>

#define PALETTE_SIZE 768

enum { GFX_NORMAL = 0 };

class OSystem_AmigaOS3 : public OSystem, PaletteManager {
public:
	OSystem_AmigaOS3();

	~OSystem_AmigaOS3();

	/**
	 * Pre-initialize backend. It should be called after
	 * instantiating the backend. Early needed managers are
	 * created here.
	 */
	void init(int audioThreadPriority);

	// Override functions from ModularBackend and OSystem
	virtual void initBackend();

	virtual void quit();

	// Logging
	virtual void logMessage(LogMessageType::Type type, const char *message);

	virtual Common::SeekableReadStream *createConfigReadStream();
	virtual Common::WriteStream *createConfigWriteStream();

	virtual uint32 getMillis(bool skipRecord = false);

	virtual void delayMillis(uint msecs);

	virtual void getTimeAndDate(TimeDate &td) const;

	virtual bool hasFeature(OSystem::Feature f);
	virtual void setFeatureState(OSystem::Feature f, bool enable);
	virtual bool getFeatureState(OSystem::Feature f);

	virtual const OSystem::GraphicsMode *getSupportedGraphicsModes() const;
	virtual int getDefaultGraphicsMode() const;
	virtual bool setGraphicsMode(int mode);
	virtual void resetGraphicsScale();
	virtual int getGraphicsMode() const;

	virtual void initSize(uint width, uint height, const Graphics::PixelFormat *format = NULL);

	virtual int getScreenChangeID() const { return _screenChangeCount; }

	virtual void beginGFXTransaction();
	virtual OSystem::TransactionError endGFXTransaction();

	virtual inline int16 getHeight() { return _videoMode.screenHeight; }
	virtual inline int16 getWidth() { return _videoMode.screenWidth; }

	virtual inline PaletteManager *getPaletteManager() { return this; }

	virtual void setPalette(const byte *colors, uint start, uint num);
	virtual void grabPalette(byte *colors, uint start, uint num) const;

	virtual void copyRectToScreen(const void *buf, int pitch, int x, int y, int w, int h);
	virtual inline Graphics::Surface *lockScreen() { return &_screen; }
	virtual void unlockScreen() { _screenDirty = true; }
	virtual void fillScreen(uint32 col);
	virtual void updateScreen();
	virtual void setShakePos(int shakeXOffset, int shakeYOffset);

	virtual void setFocusRectangle(const Common::Rect &rect) {}
	virtual void clearFocusRectangle() {}

	virtual void showOverlay();
	virtual void hideOverlay();
	virtual inline Graphics::PixelFormat getOverlayFormat() const { return _overlayFormat; }
	virtual void clearOverlay();
	virtual void grabOverlay(void *buf, int pitch);
	virtual void copyRectToOverlay(const void *buf, int pitch, int x, int y, int w, int h);
	virtual inline int16 getOverlayHeight() { return _videoMode.overlayHeight; }
	virtual inline int16 getOverlayWidth() { return _videoMode.overlayWidth; }

	virtual bool showMouse(bool visible);
	virtual void warpMouse(int x, int y);

	virtual void setMouseCursor(const void *buf, uint w, uint h, int hotspotX, int hotspotY, uint32 keycolor,
								bool dontScale = false, const Graphics::PixelFormat *format = NULL);

	virtual void setCursorPalette(const byte *colors, uint start, uint num) {}
	void disableCursorPalette(bool disable) {}
	void setMouseCursorPosition(uint16 x, uint16 y);

	// inline struct Window* getHardwareWindow() { return _hardwareWindow; }
	struct Window *getHardwareWindow();

	virtual inline Audio::Mixer *getMixer() { return _mixerManager->getMixer(); }

	virtual void displayMessageOnOSD(const char *msg) {}

	virtual MutexRef createMutex();
	virtual inline void lockMutex(OSystem::MutexRef mutex) { ObtainSemaphore((SignalSemaphore *)mutex); }
	virtual inline void unlockMutex(OSystem::MutexRef mutex) { ReleaseSemaphore((SignalSemaphore *)mutex); }
	virtual inline void deleteMutex(OSystem::MutexRef mutex) { FreeVec(mutex); }

protected:
	typedef uint16 OverlayColor;

	bool _inited;

	AmigaOS3MixerManager *_mixerManager;

	/**
	 * The event source we use for obtaining SDL events.
	 */
	AmigaOS3EventSource *_eventSource;

	Backends::Log::Log *_debugLogger;
	Backends::Log::Log *_warningLogger;
	Backends::Log::Log *_errorLogger;

	int _audioThreadPriority;

	/** Hardware window */
	struct Window *_hardwareWindow;

	/** Hardware screen */
	static const UBYTE AGA_VIDEO_DEPTH = 8;
	static const UBYTE NUM_SCREENBUFFERS = 2;

	struct ScreenBuffer *_hardwareScreenBuffer[NUM_SCREENBUFFERS];
	struct RastPort _screenRastPorts[NUM_SCREENBUFFERS];
	UBYTE _currentScreenBuffer = 0;

	struct Screen *_hardwareScreen;

	/** Unseen game screen */
	Graphics::Surface _screen;
	Graphics::Surface _tmpscreen;

	// Loading screen.
	// Graphics::Surface* _splashSurface;

	Graphics::Surface _overlayscreen8;
	Graphics::Surface _overlayscreen16;
	bool _overlayVisible;
	Graphics::PixelFormat _overlayFormat;
	byte *_overlayColorMap;

	enum { kTransactionNone = 0, kTransactionActive = 1, kTransactionRollback = 2 };

	struct TransactionDetails {
		bool sizeChanged;
	};
	TransactionDetails _transactionDetails;

	struct VideoState {
		uint16 screenWidth, screenHeight;
		uint16 overlayScreenHeight;

		uint16 overlayWidth;
		uint16 overlayHeight;

		uint16 bytesPerRow;
		uint16 overlayBytesPerRow;

		int mode;

		bool setup;
	};
	VideoState _videoMode, _oldVideoMode;

	int _transactionMode;

	int _screenChangeCount;

	int _overlayWidth;
	int _overlayHeight;

	bool _screenDirty;
	bool _overlayDirty;
	bool _mouseDirty;

	// Mouse data.
	struct MouseCursor {
		MouseCursor() : visible(false), keyColor(0), w(0), h(0), x(0), y(0), hotX(0), hotY(0) {}
		uint32 keyColor;
		uint16 w, h;
		// The mouse position.
		uint16 x, y;
		int16 hotX, hotY;
		bool visible;

		Graphics::Surface surface;
	};

	MouseCursor _mouseCursor;

	struct MouseCursorMask {
		// The mouse position.
		uint x, y;

		// The size of the cursor image.
		uint w, h;

		Graphics::Surface surface;
	};

	MouseCursorMask _mouseCursorMask;

	// Palette data
	byte *_currentPalette;
	byte *_gamePalette;
	byte *_overlayPalette;

	ULONG *_agaPalette;
	uint16 _paletteDirtyStart, _paletteDirtyEnd;

	// Shake mode
	int _currentShakePos;
	int _newShakePos;

	/**
	 * Get the file path where the user configuration
	 * of ScummVM will be saved.
	 */
	Common::String getDefaultConfigFileName();

	bool loadGFXMode();
	ULONG loadModeId();
	void saveModeId(ULONG modeId);

	struct Screen *createHardwareScreen(uint16 width, uint16 height);
	struct Window *createHardwareWindow(uint16 width, uint16 height, struct Screen *screen);
	void unloadGFXMode();
	void updatePalette();

	void loadOverlayPalette();
	void loadOverlayColorMap();

	void drawMouse();
	void undrawMouse();

	// UBYTE *scaleScreen();

	// TODO - UNIMPLEMENTED
	virtual void displayActivityIconOnOSD(const Graphics::Surface *icon);
};

#endif
