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

<<<<<<< HEAD:backends/fs/posix-drives/posix-drives-fs-factory.h
#ifndef POSIX_DRIVES_FILESYSTEM_FACTORY_H
#define POSIX_DRIVES_FILESYSTEM_FACTORY_H

#include "backends/fs/fs-factory.h"

/**
 * A FilesystemFactory implementation for filesystems with a special
 * top-level directory with hard-coded entries but that otherwise
 * implement the POSIX APIs.
 *
 * For used with paths like these:
 * - 'sdcard:/games/scummvm.ini'
 * - 'hdd1:/usr/bin'
 */
class DrivesPOSIXFilesystemFactory : public FilesystemFactory {
public:
	/**
	 * Add a drive to the top-level directory
	 */
	void addDrive(const Common::String &name);
||||||| 6cf52b515dc:backends/platform/openpandora/op-options.cpp
/*
 * OpenPandora: Options, custom code and hardware stuff.
 *
 */
=======
#include "ultima/shared/maps/creature.h"
#include "ultima/shared/early/game.h"
>>>>>>> scummvm/master:engines/ultima/shared/maps/creature.cpp

<<<<<<< HEAD:backends/fs/posix-drives/posix-drives-fs-factory.h
protected:
	// FilesystemFactory API
	AbstractFSNode *makeRootFileNode() const override;
	AbstractFSNode *makeCurrentDirectoryFileNode() const override;
	AbstractFSNode *makeFileNodePath(const Common::String &path) const override;
||||||| 6cf52b515dc:backends/platform/openpandora/op-options.cpp
#include "backends/platform/openpandora/op-options.h"

namespace OP {
=======
namespace Ultima {
namespace Shared {
namespace Maps {

void Creature::synchronize(Common::Serializer &s) {
	s.syncAsSint32LE(_hitPoints);
}
>>>>>>> scummvm/master:engines/ultima/shared/maps/creature.cpp

<<<<<<< HEAD:backends/fs/posix-drives/posix-drives-fs-factory.h
private:
	Common::Array<Common::String> _drives;
};
||||||| 6cf52b515dc:backends/platform/openpandora/op-options.cpp
enum {
	/* Touchscreen TapMode */
	TAPMODE_LEFT        = 0,
	TAPMODE_RIGHT       = 1,
	TAPMODE_HOVER       = 2,
	TAPMODE_HOVER_DPAD  = 3
};
=======
void Creature::update(bool isPreUpdate) {
	if (isPreUpdate) {
		// Check whether creature can attack
		movement();
		_isAttacking = attackDistance() != 0;
	} else if (_isAttacking && !_gameRef->_party->isDead()) {
		attackParty();
	}
}
>>>>>>> scummvm/master:engines/ultima/shared/maps/creature.cpp

<<<<<<< HEAD:backends/fs/posix-drives/posix-drives-fs-factory.h
#endif
||||||| 6cf52b515dc:backends/platform/openpandora/op-options.cpp
int tapmodeLevel = TAPMODE_LEFT;

void ToggleTapMode() {
	if (tapmodeLevel == TAPMODE_LEFT) {
		tapmodeLevel = TAPMODE_RIGHT;
	} else if (tapmodeLevel == TAPMODE_RIGHT) {
		tapmodeLevel = TAPMODE_HOVER;
	} else if (tapmodeLevel == TAPMODE_HOVER) {
		tapmodeLevel = TAPMODE_HOVER_DPAD;
	} else if (tapmodeLevel == TAPMODE_HOVER_DPAD) {
		tapmodeLevel = TAPMODE_LEFT;
	} else {
		tapmodeLevel = TAPMODE_LEFT;
	}
}

} /* namespace OP */
=======
bool Creature::subtractHitPoints(uint amount) {
	if ((int)amount >= _hitPoints) {
		_hitPoints = 0;
		return true;
	} else {
		_hitPoints -= amount;
		return false;
	}
}

} // End of namespace Maps
} // End of namespace Shared
} // End of namespace Ultima
>>>>>>> scummvm/master:engines/ultima/shared/maps/creature.cpp
