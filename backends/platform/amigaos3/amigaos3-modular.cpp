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

#include "backends/platform/amigaos3/amigaos3-modular.h"

#include "backends/audiocd/default/default-audiocd.h"
#include "backends/events/amigaos3/amigaos3-events.h"
#include "backends/fs/amigaos3/amigaos3-fs-factory.h"
#include "backends/fs/amigaos3/amigaos3-fs-node.h"
#include "backends/fs/fs-factory.h"
#include "backends/saves/default/default-saves.h"
#include "backends/timer/amigaos3/amigaos3-timer.h"
#include "backends/timer/default/default-timer.h"

#include "common/debug.h"
#include "common/scummsys.h"
#include "common/textconsole.h"

#include "graphics/colormasks.h"

#include <proto/timer.h>

static struct timeval t0;
extern bool default_timer;

OSystem_AmigaOS3_Modular::OSystem_AmigaOS3_Modular() {
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

OSystem_AmigaOS3_Modular::~OSystem_AmigaOS3_Modular() {
#ifndef NDEBUG
	debug(1, "OSystem_AmigaOS3::~OSystem_AmigaOS3()");
#endif

	/*if (_splashSurface) {
			_splashSurface->free();
			delete _splashSurface;
			_splashSurface = NULL;
	}*/

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

void OSystem_AmigaOS3_Modular::init() {
	_audioThreadPriority = audioThreadPriority;

	// Initialze File System Factory
	_fsFactory = new AmigaOS3FilesystemFactory();

#ifndef NDEBUG
	_debugLogger = new Backends::Log::Log(this);

	AmigaOS3FilesystemNode *node = new AmigaOS3FilesystemNode("debug.log");

	Common::WriteStream *logFile = node->createWriteStream();
	if (logFile) {
		_debugLogger->open(logFile);
	}

	delete node;
#endif


}

void OSystem_AmigaOS3_Modular::initBackend() {
#ifndef NDEBUG
	debug(1, "OSystem_AmigaOS3::initBackend()");

	// Check if backend has not been initialized
	assert(!_inited);
#endif

	// Pass in the current path as the default save path.
	char pathName[255];
	GetCurrentDirName(pathName, 255);
	strncat(pathName, "/", 1);
	_savefileManager = new DefaultSaveFileManager(pathName);

	if (default_timer)
		_timerManager = new DefaultTimerManager();
	else
		_timerManager = new AmigaOS3TimerManager();

	// Setup and start mixer
	_mixerManager = new AmigaOS3MixerManager();
	_mixerManager->init(_audioThreadPriority);

	_audiocdManager = new DefaultAudioCDManager();

	_eventSource = new AmigaOS3EventSource();

	_eventManager = new DefaultEventManager(_eventSource);

	_overlayFormat = Graphics::createPixelFormat<565>();

	memset(&_transactionDetails, 0, sizeof(_transactionDetails));

	memset(&_videoMode, 0, sizeof(_videoMode));

	// Set video mode defaults.
	_videoMode.mode = GFX_NORMAL;
	_videoMode.setup = false;

	//_videoMode.aspectRatioCorrectionRequested = false;
	//_videoMode.aspectRatioCorrectionInUse = false;

	memset(&_oldVideoMode, 0, sizeof(_oldVideoMode));

	_transactionMode = kTransactionNone;

	_screenChangeCount = 0;

	memset(&_mouseCursorMask, 0, sizeof(_mouseCursorMask));

	// allocate palette storage
	_currentPalette = (byte *)calloc(PALETTE_SIZE, sizeof(byte));
	_gamePalette = (byte *)calloc(PALETTE_SIZE, sizeof(byte));
	_overlayPalette = NULL;
	_agaPalette = (ULONG *)calloc(770, sizeof(uint32));

	_paletteDirtyStart = 256;
	_paletteDirtyEnd = 0;

	_screenDirty = false;
	_overlayDirty = false;

	_currentShakePos = _newShakePos = 0;

	_inited = true;
}

Common::SeekableReadStream *OSystem_AmigaOS3_Modular::createConfigReadStream() {
	Common::FSNode file(getDefaultConfigFileName());
	return file.createReadStream();
}

Common::WriteStream *OSystem_AmigaOS3_Modular::createConfigWriteStream() {
	Common::FSNode file(getDefaultConfigFileName());
	return file.createWriteStream();
}

void OSystem_AmigaOS3_Modular::quit() {
#ifndef NDEBUG
	debug(1, "OSystem_AmigaOS3::quit()");
#endif

	delete this;
	exit(0);
}

void OSystem_AmigaOS3_Modular::logMessage(LogMessageType::Type type, const char *message) {
	if (type == LogMessageType::kDebug) {
		if (_debugLogger) {
			_debugLogger->print(message);
		}
	} else {
		if (type == LogMessageType::kWarning) {
			if (!_warningLogger) {
				_warningLogger = new Backends::Log::Log(this);

				AmigaOS3FilesystemNode *node = new AmigaOS3FilesystemNode("warning.log");

				Common::WriteStream *logFile = node->createWriteStream();
				delete node;

				if (logFile) {
					_warningLogger->open(logFile);
				}
			}

			if (_warningLogger) {
				_warningLogger->print(message);
			}
		} else {
			if (!_errorLogger) {
				_errorLogger = new Backends::Log::Log(this);

				AmigaOS3FilesystemNode *node = new AmigaOS3FilesystemNode("error.log");

				Common::WriteStream *logFile = node->createWriteStream();
				delete node;

				if (logFile) {
					_errorLogger->open(logFile);
					_errorLogger->print(message);
				}
			}
		}
	}
}

uint32 OSystem_AmigaOS3_Modular::getMillis(bool skipRecord) {
	// Kickstart 2.0 version
	struct timeval t1;
	long secs, usecs;
	unsigned long tc;

	if (t0.tv_secs == 0 && t0.tv_micro == 0) {
		GetSysTime(&t0);
		return 0;
	}

	GetSysTime(&t1);

	secs = t1.tv_secs - t0.tv_secs;
	usecs = t1.tv_micro - t0.tv_micro;

	if (usecs < 0) {
		usecs += 1000000;
		secs--;
	}

	tc = secs * 1000 + usecs / 1000;

	return tc;
}

extern struct timerequest *TimerIOReq;

void OSystem_AmigaOS3_Modular::delayMillis(uint msecs) {
	// Temporary workaround, using the same IO request from multiple threads
	//  could be dangerous.
	if (default_timer) {
		if (msecs) {
			if (msecs < 1000) {
				TimerIOReq->tr_time.tv_secs = 0;
				TimerIOReq->tr_time.tv_micro = msecs * 1000;
			} else {
				TimerIOReq->tr_time.tv_secs = msecs / 1000;
				TimerIOReq->tr_time.tv_micro = (msecs % 1000) * 1000;
			}
			TimerIOReq->tr_node.io_Command = TR_ADDREQUEST;
			DoIO(&TimerIOReq->tr_node);
			WaitIO(&TimerIOReq->tr_node);
		}
	} else {
		// 1 Tick is 50Hz, 20ms
		Delay(msecs / 20);
	}
}

void OSystem_AmigaOS3_Modular::getTimeAndDate(TimeDate &td) const {
	time_t curTime = time(0);
	struct tm t = *localtime(&curTime);

	td.tm_sec = t.tm_sec;
	td.tm_min = t.tm_min;
	td.tm_hour = t.tm_hour;
	td.tm_mday = t.tm_mday;
	td.tm_mon = t.tm_mon;
	td.tm_year = t.tm_year;
}

Common::String OSystem_AmigaOS3_Modular::getDefaultConfigFileName() {
	return "scummvm.ini";
}

OSystem::MutexRef OSystem_AmigaOS3_Modular::createMutex() {
	SignalSemaphore *sem = (SignalSemaphore *)AllocVec(sizeof(SignalSemaphore), MEMF_PUBLIC);

	if (sem) {
		InitSemaphore(sem);
	}

	return (MutexRef)sem;
}
