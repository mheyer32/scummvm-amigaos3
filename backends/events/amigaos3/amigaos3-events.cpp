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

#include "common/scummsys.h"
#include "common/system.h"

#include "backends/platform/amigaos3/amigaos3-modular.h"

#include "backends/events/amigaos3/amigaos3-events.h"
#include "backends/timer/default/default-timer.h"

#include "common/textconsole.h"
#include "engines/engine.h"

// Amiga includes.
#include <proto/intuition.h>
#include <proto/exec.h>
#include <proto/keymap.h>

//#include "rawkeycodes.h"
#define RAWKEY_ESCAPE 0x45

#define RAWKEY_F1 0x50
#define RAWKEY_F2 0x51
#define RAWKEY_F3 0x52
#define RAWKEY_F4 0x53
#define RAWKEY_F5 0x54
#define RAWKEY_F6 0x55
#define RAWKEY_F7 0x56
#define RAWKEY_F8 0x57
#define RAWKEY_F9 0x58
#define RAWKEY_F10 0x59
#define RAWKEY_F11 0x4B
#define RAWKEY_F12 0x6F

#define RAWKEY_X 0x32
#define RAWKEY_Y 0x15
#define RAWKEY_Z 0x31

#define RAWKEY_KP_0 0x0F
#define RAWKEY_KP_1 0x1D
#define RAWKEY_KP_2 0x1E
#define RAWKEY_KP_3 0x1F
#define RAWKEY_KP_4 0x2D
#define RAWKEY_KP_5 0x2E
#define RAWKEY_KP_6 0x2F
#define RAWKEY_KP_7 0x3D
#define RAWKEY_KP_8 0x3E
#define RAWKEY_KP_9 0x3F
#define RAWKEY_KP_POINT 0x3C
#define RAWKEY_KP_ENTER 0x43
#define RAWKEY_KP_MINUS 0x4A
#define RAWKEY_KP_NUMLOCK 0x5A
#define RAWKEY_KP_SCROLLOCK 0x5B
#define RAWKEY_KP_MULTIPLY 0x5D
#define RAWKEY_KP_PLUS 0x5E


bool default_timer = false;

AmigaOS3EventSource::AmigaOS3EventSource() {
#ifndef NDEBUG
	debug(9, "AmigaOS3EventSource::AmigaOS3EventSource()");
#endif
	if (default_timer)
		_timerManager = ((DefaultTimerManager *)g_system->getTimerManager());
}

AmigaOS3EventSource::~AmigaOS3EventSource() {
#ifndef NDEBUG
	debug(9, "AmigaOS3EventSource::~AmigaOS3EventSource()");
	if (default_timer)
		_timerManager = NULL;
#endif
}

bool AmigaOS3EventSource::pollEvent(Common::Event &event) {
#ifndef NDEBUG
	debug(9, "AmigaOS3EventSource::pollEvent()");
#endif

	bool result = false;
	if (default_timer)
		_timerManager->handler();

	OSystemAGA *const system = static_cast<OSystemAGA *>(g_system);
	struct Window *hardwareWindow = system->getHardwareWindow();

	if (hardwareWindow) {
		struct IntuiMessage *imsg;

		while (!result && (imsg = (struct IntuiMessage *)GetMsg(hardwareWindow->UserPort))) {
			ReplyMsg((struct Message *)imsg);

			switch (imsg->Class) {
			case IDCMP_MOUSEMOVE:
				event.type = Common::EVENT_MOUSEMOVE;
				event.mouse.x = imsg->MouseX;
				event.mouse.y = imsg->MouseY;
				system->setMouseCursorPosition(imsg->MouseX, imsg->MouseY);
				result = true;
				break;

			case IDCMP_MOUSEBUTTONS:
				if (imsg->Code == SELECTDOWN) {
					event.type = Common::EVENT_LBUTTONDOWN;
				}

				if (imsg->Code == SELECTUP) {
					event.type = Common::EVENT_LBUTTONUP;
				}

				if (imsg->Code == MENUDOWN) {
					event.type = Common::EVENT_RBUTTONDOWN;
				}

				if (imsg->Code == MENUUP) {
					event.type = Common::EVENT_RBUTTONUP;
				}

				event.mouse.x = imsg->MouseX;
				event.mouse.y = imsg->MouseY;
				result = true;
				break;

			case IDCMP_RAWKEY:
				int code = imsg->Code;

				int flags = 0;

				if (imsg->Qualifier & (IEQUALIFIER_LALT | IEQUALIFIER_RALT)) {
					flags |= Common::KBD_ALT;
				}

				if (imsg->Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT)) {
					flags |= Common::KBD_SHIFT;
				}

				if (imsg->Qualifier & IEQUALIFIER_CONTROL) {
					flags |= Common::KBD_CTRL;
				}

				event.kbd.flags = flags;

				event.type = (code & IECODE_UP_PREFIX) ? Common::EVENT_KEYUP : Common::EVENT_KEYDOWN;

				code &= ~IECODE_UP_PREFIX;

				switch (code) {
				case RAWKEY_ESCAPE:
					event.kbd.keycode = Common::KEYCODE_ESCAPE;
					event.kbd.ascii = Common::ASCII_ESCAPE;
					result = true;
					break;

				case RAWKEY_F1:
					event.kbd.keycode = Common::KEYCODE_F1;
					event.kbd.ascii = Common::ASCII_F1;
					result = true;
					break;

				case RAWKEY_F2:
					event.kbd.keycode = Common::KEYCODE_F2;
					event.kbd.ascii = Common::ASCII_F2;
					result = true;
					break;

				case RAWKEY_F3:
					event.kbd.keycode = Common::KEYCODE_F3;
					event.kbd.ascii = Common::ASCII_F3;
					result = true;
					break;

				case RAWKEY_F4:
					event.kbd.keycode = Common::KEYCODE_F4;
					event.kbd.ascii = Common::ASCII_F4;
					result = true;
					break;

				case RAWKEY_F5:
					event.kbd.keycode = Common::KEYCODE_F5;
					event.kbd.ascii = Common::ASCII_F5;
					result = true;
					break;

				case RAWKEY_F6:
					event.kbd.keycode = Common::KEYCODE_F6;
					event.kbd.ascii = Common::ASCII_F6;
					result = true;
					break;

				case RAWKEY_F7:
					event.kbd.keycode = Common::KEYCODE_F7;
					event.kbd.ascii = Common::ASCII_F7;
					result = true;
					break;

				case RAWKEY_F8:
					event.kbd.keycode = Common::KEYCODE_F8;
					event.kbd.ascii = Common::ASCII_F8;
					result = true;
					break;

				case RAWKEY_F9:
					event.kbd.keycode = Common::KEYCODE_F9;
					event.kbd.ascii = Common::ASCII_F9;
					result = true;
					break;

				case RAWKEY_F10:
					event.kbd.keycode = Common::KEYCODE_F10;
					event.kbd.ascii = Common::ASCII_F10;
					result = true;
					break;

				case RAWKEY_F11:
					event.kbd.keycode = Common::KEYCODE_F11;
					event.kbd.ascii = Common::ASCII_F11;
					result = true;
					break;

				case RAWKEY_F12:
					event.kbd.keycode = Common::KEYCODE_F12;
					event.kbd.ascii = Common::ASCII_F12;
					result = true;
					break;

				case RAWKEY_KP_0:
					event.kbd.keycode = Common::KEYCODE_KP0;
					goto TranslateToAscii;

				case RAWKEY_KP_1:
					event.kbd.keycode = Common::KEYCODE_KP1;
					goto TranslateToAscii;

				case RAWKEY_KP_2:
					event.kbd.keycode = Common::KEYCODE_KP2;
					goto TranslateToAscii;

				case RAWKEY_KP_3:
					event.kbd.keycode = Common::KEYCODE_KP3;
					goto TranslateToAscii;

				case RAWKEY_KP_4:
					event.kbd.keycode = Common::KEYCODE_KP4;
					goto TranslateToAscii;

				case RAWKEY_KP_5:
					event.kbd.keycode = Common::KEYCODE_KP5;
					goto TranslateToAscii;

				case RAWKEY_KP_6:
					event.kbd.keycode = Common::KEYCODE_KP6;
					goto TranslateToAscii;

				case RAWKEY_KP_7:
					event.kbd.keycode = Common::KEYCODE_KP7;
					goto TranslateToAscii;

				case RAWKEY_KP_8:
					event.kbd.keycode = Common::KEYCODE_KP8;
					goto TranslateToAscii;

				case RAWKEY_KP_9:
					event.kbd.keycode = Common::KEYCODE_KP9;
					goto TranslateToAscii;

				case RAWKEY_KP_POINT:
					event.kbd.keycode = Common::KEYCODE_KP0;
					goto TranslateToAscii;

				case RAWKEY_KP_ENTER:
					event.kbd.keycode = Common::KEYCODE_KP_ENTER;
					goto TranslateToAscii;

				case RAWKEY_KP_MINUS:
					event.kbd.keycode = Common::KEYCODE_KP0;
					goto TranslateToAscii;

				case RAWKEY_KP_NUMLOCK:
					event.kbd.keycode = Common::KEYCODE_NUMLOCK;
					goto TranslateToAscii;

				case RAWKEY_KP_SCROLLOCK:
					event.kbd.keycode = Common::KEYCODE_SCROLLOCK;
					goto TranslateToAscii;

				case RAWKEY_KP_MULTIPLY:
					event.kbd.keycode = Common::KEYCODE_KP_MULTIPLY;
					goto TranslateToAscii;

				case RAWKEY_KP_PLUS:
					event.kbd.keycode = Common::KEYCODE_KP_PLUS;
					goto TranslateToAscii;

				default:
					if (flags == Common::KBD_CTRL && code == RAWKEY_Z) {
						event.type = Common::EVENT_QUIT;
					}

					if (flags == Common::KBD_ALT && code == RAWKEY_X) {
						event.type = Common::EVENT_QUIT;
					}

				TranslateToAscii:
					InputEvent FakedIEvent;
					memset(&FakedIEvent, 0, sizeof(InputEvent));
					FakedIEvent.ie_Class = IECLASS_RAWKEY;
					FakedIEvent.ie_Code = code;
					FakedIEvent.ie_Qualifier = imsg->Qualifier;

					char charbuf;

					if (MapRawKey(&FakedIEvent, &charbuf, 1, NULL) == 1) {
						event.kbd.ascii = charbuf;
						if (event.kbd.keycode == Common::KEYCODE_INVALID) {
							event.kbd.keycode = (Common::KeyCode)charbuf;
						}
						result = true;
					}
					break;

				}
			}
		}
	}

	return result;
}
