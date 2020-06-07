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

<<<<<<< HEAD:backends/plugins/3ds/3ds-provider.h
#if defined(DYNAMIC_MODULES) && defined(__3DS__)
||||||| 6cf52b515dc:backends/platform/3ds/gui.h
#ifndef GUI_3DS_H
#define GUI_3DS_H
=======
#ifndef ULTIMA_ULTIMA0_GAME_H
#define ULTIMA_ULTIMA0_GAME_H
>>>>>>> scummvm/master:engines/ultima/ultima0/game.h

<<<<<<< HEAD:backends/plugins/3ds/3ds-provider.h
#ifndef BACKENDS_PLUGINS_3DS_PROVIDER_H
#define BACKENDS_PLUGINS_3DS_PROVIDER_H

#include "backends/plugins/elf/elf-provider.h"
||||||| 6cf52b515dc:backends/platform/3ds/gui.h
#include "gui/message.h"
=======
#include "ultima/shared/early/game.h"
>>>>>>> scummvm/master:engines/ultima/ultima0/game.h

namespace Ultima {
namespace Ultima0 {

class Ultima0Game : public Shared::Game {
	DECLARE_MESSAGE_MAP;
public:
<<<<<<< HEAD:backends/plugins/3ds/3ds-provider.h
	Plugin *createPlugin(const Common::FSNode &node) const;
||||||| 6cf52b515dc:backends/platform/3ds/gui.h
	StatusMessageDialog(const Common::String &message, uint32 duration);

	void handleTickle();

protected:
	virtual void close();

	uint32 _timer;
	static StatusMessageDialog* _opened;
=======
	CLASSDEF;
	Ultima0Game();
	~Ultima0Game() override {}

>>>>>>> scummvm/master:engines/ultima/ultima0/game.h
};

<<<<<<< HEAD:backends/plugins/3ds/3ds-provider.h
#endif // BACKENDS_PLUGINS_3DS_PROVIDER_H

#endif // defined(DYNAMIC_MODULES) && defined(__3DS__)
||||||| 6cf52b515dc:backends/platform/3ds/gui.h
#endif // GUI_3DS_H
=======
} // End of namespace Ultima0
} // End of namespace Ultima

#endif
>>>>>>> scummvm/master:engines/ultima/ultima0/game.h
