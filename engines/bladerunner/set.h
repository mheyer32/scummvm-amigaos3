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

#ifndef BLADERUNNER_SET_H
#define BLADERUNNER_SET_H

#include "bladerunner/boundingbox.h"
#include "bladerunner/set_effects.h"
#include "bladerunner/lights.h"
#include "bladerunner/scene_objects.h"


#include "common/scummsys.h"
#include "common/str.h"

namespace BladeRunner {

class BladeRunnerEngine;
class VQADecoder;
class Scene;

struct Object {
	char        _name[20];
	BoundingBox _bbox;
	uint8       _isObstacle;
	uint8       _isClickable;
	uint8       _isHotMouse;
	uint8       _isCombatTarget;
	uint8       _unknown1;
};

struct Walkbox {
	char    _name[20];
	float   _altitude;
	uint32  _vertexCount;
	Vector3 _vertices[8];
};

class Set {
	friend class Scene;

	BladeRunnerEngine *_vm;

	uint32      _objectCount;
	uint32      _walkboxCount;
	Object     *_objects;
	Walkbox    *_walkboxes;
	int         _walkboxStepSound[85];
	int         _footstepSoundOverride;
	SetEffects *_effects;
public:
	

public:
	Set(BladeRunnerEngine *vm);
	~Set();

	bool open(const Common::String &name);
	void addObjectsToScene(SceneObjects *sceneObjects);

	int findWalkbox(float x, float z);
	int findObject(char* objectName);

	bool objectSetHotMouse(int objectId);
	bool objectGetBoundingBox(int objectId, BoundingBox *boundingBox);

private:
	bool isXzInWalkbox(float x, float z, Walkbox* walkbox);

};

} // End of namespace BladeRunner

#endif
