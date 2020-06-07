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

<<<<<<< HEAD:backends/platform/sdl/morphos/morphos.cpp
#include "common/scummsys.h"
||||||| 6cf52b515dc:backends/graphics/androidsdl/androidsdl-graphics.h
#ifndef BACKENDS_GRAPHICS_SDL_ANDROIDSDL_H
#define BACKENDS_GRAPHICS_SDL_ANDROIDSDL_H
=======
#ifndef ULTIMA_ULTIMA6_META_ENGINE
#define ULTIMA_ULTIMA6_META_ENGINE
>>>>>>> scummvm/master:engines/ultima/nuvie/meta_engine.h

<<<<<<< HEAD:backends/platform/sdl/morphos/morphos.cpp
#ifdef __MORPHOS__
||||||| 6cf52b515dc:backends/graphics/androidsdl/androidsdl-graphics.h
#include "backends/graphics/surfacesdl/surfacesdl-graphics.h"
=======
#include "engines/savestate.h"
>>>>>>> scummvm/master:engines/ultima/nuvie/meta_engine.h

<<<<<<< HEAD:backends/platform/sdl/morphos/morphos.cpp
#include "backends/platform/sdl/morphos/morphos.h"
#include "backends/fs/morphos/morphos-fs-factory.h"

void OSystem_MorphOS::init() {
	// Initialze File System Factory
	_fsFactory = new MorphOSFilesystemFactory();

	// Invoke parent implementation of this method
	OSystem_SDL::init();
}
||||||| 6cf52b515dc:backends/graphics/androidsdl/androidsdl-graphics.h
class AndroidSdlGraphicsManager : public SurfaceSdlGraphicsManager {
public:
	AndroidSdlGraphicsManager(SdlEventSource *sdlEventSource, SdlWindow *window);

};
=======
namespace Ultima {
namespace Nuvie {

enum {
	ORIGINAL_SAVE_SLOT = 1
};

class MetaEngine {
public:
	static void listSaves(SaveStateList &saveList);
};
>>>>>>> scummvm/master:engines/ultima/nuvie/meta_engine.h

} // End of namespace Nuvie
} // End of namespace Ultima

#endif
