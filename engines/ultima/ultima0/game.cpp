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

<<<<<<< HEAD:backends/platform/sdl/morphos/morphos.h
#ifndef PLATFORM_SDL_MORPHOS_H
#define PLATFORM_SDL_MORPHOS_H
||||||| 6cf52b515dc:backends/platform/openpandora/op-options.h
/*
 * OpenPandora: Options, custom code and hardware stuff.
 *
 */
=======
#include "ultima/ultima0/game.h"
>>>>>>> scummvm/master:engines/ultima/ultima0/game.cpp

<<<<<<< HEAD:backends/platform/sdl/morphos/morphos.h
#include "backends/platform/sdl/sdl.h"
||||||| 6cf52b515dc:backends/platform/openpandora/op-options.h
#ifndef OP_OPTIONS_H
#define OP_OPTIONS_H
=======
namespace Ultima {
namespace Ultima0 {
>>>>>>> scummvm/master:engines/ultima/ultima0/game.cpp

<<<<<<< HEAD:backends/platform/sdl/morphos/morphos.h
class OSystem_MorphOS : public OSystem_SDL {
public:
	OSystem_MorphOS() {}
	virtual ~OSystem_MorphOS() {}
||||||| 6cf52b515dc:backends/platform/openpandora/op-options.h
namespace OP {
=======
EMPTY_MESSAGE_MAP(Ultima0Game, Shared::Game);
>>>>>>> scummvm/master:engines/ultima/ultima0/game.cpp

<<<<<<< HEAD:backends/platform/sdl/morphos/morphos.h
	virtual void init();
};
||||||| 6cf52b515dc:backends/platform/openpandora/op-options.h
extern int tapmodeLevel;
=======
Ultima0Game::Ultima0Game() : Shared::Game() {
}
>>>>>>> scummvm/master:engines/ultima/ultima0/game.cpp

<<<<<<< HEAD:backends/platform/sdl/morphos/morphos.h
#endif
||||||| 6cf52b515dc:backends/platform/openpandora/op-options.h
extern void ToggleTapMode();

} /* namespace OP */

#endif //OP_OPTIONS_H
=======
} // End of namespace Ultima0
} // End of namespace Ultima
>>>>>>> scummvm/master:engines/ultima/ultima0/game.cpp
