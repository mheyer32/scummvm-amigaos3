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

<<<<<<< HEAD:backends/fs/posix-drives/posix-drives-fs-factory.cpp
#if defined(POSIX)
||||||| 6cf52b515dc:engines/bladerunner/ui/kia_shapes.cpp
#include "bladerunner/ui/kia_shapes.h"
=======
#include "glk/level9/level9.h"
#include "glk/level9/level9_main.h"
#include "glk/level9/os_glk.h"
>>>>>>> scummvm/master:engines/glk/level9/level9.cpp

<<<<<<< HEAD:backends/fs/posix-drives/posix-drives-fs-factory.cpp
#define FORBIDDEN_SYMBOL_ALLOW_ALL
||||||| 6cf52b515dc:engines/bladerunner/ui/kia_shapes.cpp
#include "bladerunner/bladerunner.h"
=======
namespace Glk {
namespace Level9 {
>>>>>>> scummvm/master:engines/glk/level9/level9.cpp

<<<<<<< HEAD:backends/fs/posix-drives/posix-drives-fs-factory.cpp
#include "backends/fs/posix-drives/posix-drives-fs-factory.h"
#include "backends/fs/posix-drives/posix-drives-fs.h"
||||||| 6cf52b515dc:engines/bladerunner/ui/kia_shapes.cpp
namespace BladeRunner {
=======
Level9 *g_vm = nullptr;
>>>>>>> scummvm/master:engines/glk/level9/level9.cpp

<<<<<<< HEAD:backends/fs/posix-drives/posix-drives-fs-factory.cpp
#include <unistd.h>
||||||| 6cf52b515dc:engines/bladerunner/ui/kia_shapes.cpp
KIAShapes::KIAShapes(BladeRunnerEngine *vm) {
	_vm = vm;
	_isLoaded = false;
	for (uint i = 0; i < kShapeCount; ++i) {
		_shapes[i] = nullptr;
	}
}

KIAShapes::~KIAShapes() {
	unload();
}

void KIAShapes::load() {
	if (_isLoaded) {
		return;
	}

	for (uint i = 0; i < kShapeCount; ++i) {
		Shape *shape = new Shape(_vm);
		shape->open("kiaopt.shp", i);
		_shapes[i] = shape;
	}

	_isLoaded = true;
}
=======
Level9::Level9(OSystem *syst, const GlkGameDescription &gameDesc) : GlkAPI(syst, gameDesc),
		_detection(startdata, FileSize) {
	g_vm = this;
}

void Level9::runGame() {
	initialize();

	_gameFile.close();
	gln_main(getFilename().c_str());

	deinitialize();
}
>>>>>>> scummvm/master:engines/glk/level9/level9.cpp

<<<<<<< HEAD:backends/fs/posix-drives/posix-drives-fs-factory.cpp
void DrivesPOSIXFilesystemFactory::addDrive(const Common::String &name) {
	_drives.push_back(Common::normalizePath(name, '/'));
||||||| 6cf52b515dc:engines/bladerunner/ui/kia_shapes.cpp
void KIAShapes::unload() {
	if (!_isLoaded) {
		return;
=======
bool Level9::initialize() {
	gln_initialize();
	return gln_startup_code(0, nullptr);
>>>>>>> scummvm/master:engines/glk/level9/level9.cpp
}

<<<<<<< HEAD:backends/fs/posix-drives/posix-drives-fs-factory.cpp
AbstractFSNode *DrivesPOSIXFilesystemFactory::makeRootFileNode() const {
	return new DrivePOSIXFilesystemNode(_drives);
||||||| 6cf52b515dc:engines/bladerunner/ui/kia_shapes.cpp
	for (uint i = 0; i < kShapeCount; ++i) {
		delete _shapes[i];
		_shapes[i] = nullptr;
=======
void Level9::deinitialize() {
>>>>>>> scummvm/master:engines/glk/level9/level9.cpp
}

<<<<<<< HEAD:backends/fs/posix-drives/posix-drives-fs-factory.cpp
AbstractFSNode *DrivesPOSIXFilesystemFactory::makeCurrentDirectoryFileNode() const {
	char buf[MAXPATHLEN];
	return getcwd(buf, MAXPATHLEN) ? new DrivePOSIXFilesystemNode(buf, _drives) : nullptr;
||||||| 6cf52b515dc:engines/bladerunner/ui/kia_shapes.cpp
	_isLoaded = false;
=======
Common::Error Level9::readSaveData(Common::SeekableReadStream *rs) {
	// TODO
	return Common::kNoError;
>>>>>>> scummvm/master:engines/glk/level9/level9.cpp
}

<<<<<<< HEAD:backends/fs/posix-drives/posix-drives-fs-factory.cpp
AbstractFSNode *DrivesPOSIXFilesystemFactory::makeFileNodePath(const Common::String &path) const {
	assert(!path.empty());
	return new DrivePOSIXFilesystemNode(path, _drives);
||||||| 6cf52b515dc:engines/bladerunner/ui/kia_shapes.cpp
const Shape *KIAShapes::get(int shapeId) const {
	return _shapes[shapeId];
=======
Common::Error Level9::writeGameData(Common::WriteStream *ws) {
	// TODO
	return Common::kNoError;
>>>>>>> scummvm/master:engines/glk/level9/level9.cpp
}

<<<<<<< HEAD:backends/fs/posix-drives/posix-drives-fs-factory.cpp
#endif
||||||| 6cf52b515dc:engines/bladerunner/ui/kia_shapes.cpp
} // End of namespace BladeRunner
=======
} // End of namespace Level9
} // End of namespace Glk
>>>>>>> scummvm/master:engines/glk/level9/level9.cpp
