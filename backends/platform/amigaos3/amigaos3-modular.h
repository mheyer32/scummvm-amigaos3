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
#include "backends/modular-backend.h"
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

class OSystem_AmigaOS3_Modular : public ModularBackend, PaletteManager {
public:
	OSystem_AmigaOS3_Modular();
	virtual ~OSystem_AmigaOS3_Modular();

	int audioThreadPriority;

	/**
	 * Pre-initialize backend. It should be called after
	 * instantiating the backend. Early needed managers are
	 * created here.
	**/
	virtual void init();

	// Override functions from ModularBackend and OSystem
	virtual void initBackend() override;
	
	virtual void quit() override;
	
	// Logging
	virtual void logMessage(LogMessageType::Type type, const char *message) override;

	virtual Common::SeekableReadStream *createConfigReadStream();
	virtual Common::WriteStream *createConfigWriteStream();

	virtual uint32 getMillis(bool skipRecord = false) override;
	
	virtual void delayMillis(uint msecs) override;
	virtual void getTimeAndDate(TimeDate &td) const override;

	virtual inline Audio::Mixer *getMixer() { return _mixerManager->getMixer(); }
	virtual inline PaletteManager *getPaletteManager() { return this; }

	virtual MutexRef createMutex() override;
	virtual inline void lockMutex(OSystem::MutexRef mutex) { ObtainSemaphore((SignalSemaphore *)mutex); }
	virtual inline void unlockMutex(OSystem::MutexRef mutex) { ReleaseSemaphore((SignalSemaphore *)mutex); }
	virtual inline void deleteMutex(OSystem::MutexRef mutex) { FreeVec(mutex); }
	

	//virtual Common::String getSystemLanguage() const override;

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

	// UBYTE *scaleScreen();

	virtual Common::EventSource *getDefaultEventSource() override { return _eventSource; }
};

class OSystemAGA : public OSystem_AmigaOS3_Modular {
public:
	OSystemAGA();
	virtual ~OSystemAGA();

	virtual bool hasFeature(OSystem::Feature f) override;
	virtual void setFeatureState(OSystem::Feature f, bool enable) override;
	virtual bool getFeatureState(OSystem::Feature f) override;

	virtual const OSystem::GraphicsMode *getSupportedGraphicsModes() const override;
	virtual const OSystem::GraphicsMode *getSupportedStretchModes() const override;
	virtual int getDefaultGraphicsMode() const override;
	virtual bool setGraphicsMode(int mode) override;
	virtual void resetGraphicsScale() override;
	virtual int getGraphicsMode() const override;

	virtual void initSize(uint width, uint height, const Graphics::PixelFormat *format = NULL) override;

	virtual int getScreenChangeID() const { return _screenChangeCount; }

	virtual void beginGFXTransaction() override;
	virtual OSystem::TransactionError endGFXTransaction() override;

	virtual inline int16 getHeight() { return _videoMode.screenHeight; }
	virtual inline int16 getWidth() { return _videoMode.screenWidth; }

	virtual void setPalette(const byte *colors, uint start, uint num) override;
	virtual void grabPalette(byte *colors, uint start, uint num) const override;

	virtual void copyRectToScreen(const void *buf, int pitch, int x, int y, int w, int h) override;
	virtual inline Graphics::Surface *lockScreen() { return &_screen; }
	virtual void unlockScreen() { _screenDirty = true; }
	virtual void fillScreen(uint32 col) override;
	virtual void updateScreen() override;
	virtual void setShakePos(int shakeXOffset, int shakeYOffset) override;

	virtual void setFocusRectangle(const Common::Rect &rect) {}
	virtual void clearFocusRectangle() {}

	virtual void showOverlay() override;
	virtual void hideOverlay() override;
	virtual inline Graphics::PixelFormat getOverlayFormat() const { return _overlayFormat; }
	virtual void clearOverlay() override;
	virtual void grabOverlay(void *buf, int pitch) override;
	virtual void copyRectToOverlay(const void *buf, int pitch, int x, int y, int w, int h) override;
	virtual inline int16 getOverlayHeight() { return _videoMode.overlayHeight; }
	virtual inline int16 getOverlayWidth() { return _videoMode.overlayWidth; }

	virtual bool showMouse(bool visible) override;
	virtual void warpMouse(int x, int y) override;

	virtual void setMouseCursor(const void *buf, uint w, uint h, int hotspotX, int hotspotY, uint32 keycolor,
								bool dontScale = false, const Graphics::PixelFormat *format = NULL) override;

	virtual void setCursorPalette(const byte *colors, uint start, uint num) {}
	virtual void disableCursorPalette(bool disable) {}
	virtual void setMouseCursorPosition(uint16 x, uint16 y);

	virtual struct Window *getHardwareWindow();

	// TODO - UNIMPLEMENTED
	virtual void displayActivityIconOnOSD(const Graphics::Surface *icon) override;
	virtual void displayMessageOnOSD(const char *msg) {}

	void loadOverlayPalette();
	void loadOverlayColorMap();

	void drawMouse();
	void undrawMouse();

	bool loadGFXMode();
	ULONG loadModeId();
	void saveModeId(ULONG modeId);

	struct Screen *createHardwareScreen(uint16 width, uint16 height);
	struct Window *createHardwareWindow(uint16 width, uint16 height, struct Screen *screen);
	void unloadGFXMode();
	void updatePalette();
};

#endif
