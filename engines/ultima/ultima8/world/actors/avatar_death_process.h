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

<<<<<<< HEAD:backends/platform/android/portdefs.h
#ifndef _PORTDEFS_H_
#define _PORTDEFS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>
#include <new>

#define _USE_MATH_DEFINES
#include <math.h>

// This is defined in snprintf.c
#ifdef __cplusplus
extern "C" {
#endif
int rpl_vsnprintf(char *text, size_t maxlen, const char *fmt, va_list ap);
#ifdef __cplusplus
}
#endif
||||||| 6cf52b515dc:engines/bladerunner/ui/kia_shapes.h
#ifndef BLADERUNNER_UI_SHAPES_H
#define BLADERUNNER_UI_SHAPES_H

#include "bladerunner/shape.h"

#include "common/array.h"

namespace BladeRunner {

class BladeRunnerEngine;

class KIAShapes {
	static const uint kShapeCount = 132;

	BladeRunnerEngine *_vm;
	const Shape       *_shapes[kShapeCount];
	bool               _isLoaded;

public:
	KIAShapes(BladeRunnerEngine *vm);
	~KIAShapes();

	void load();
	void unload();
	const Shape *get(int shapeId) const;
};
=======
#ifndef WORLD_ACTORS_AVATARDEATHPROCESS_H
#define WORLD_ACTORS_AVATARDEATHPROCESS_H

#include "ultima/ultima8/kernel/process.h"

namespace Ultima {
namespace Ultima8 {

class MainActor;

class AvatarDeathProcess : public Process {
public:
	AvatarDeathProcess();

	// p_dynamic_cast stuff
	ENABLE_RUNTIME_CLASSTYPE()

	void run() override;

	bool loadData(Common::ReadStream *rs, uint32 version);
	void saveData(Common::WriteStream *ws) override;
};
>>>>>>> scummvm/master:engines/ultima/ultima8/world/actors/avatar_death_process.h

<<<<<<< HEAD:backends/platform/android/portdefs.h
#define vsnprintf rpl_vsnprintf
||||||| 6cf52b515dc:engines/bladerunner/ui/kia_shapes.h
} // End of namespace BladeRunner
=======
} // End of namespace Ultima8
} // End of namespace Ultima
>>>>>>> scummvm/master:engines/ultima/ultima8/world/actors/avatar_death_process.h

#endif // _PORTDEFS_H_
