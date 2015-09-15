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

#ifndef BLADERUNNER_SCENE_H
#define BLADERUNNER_SCENE_H

#include "bladerunner/bladerunner.h"

#include "bladerunner/set.h"
//#include "bladerunner/view.h"
#include "bladerunner/vqa_player.h"
#include "bladerunner/regions.h"

namespace BladeRunner {

class BladeRunnerEngine;

class Scene {
	BladeRunnerEngine *_vm;

public:
	Set        *_set;
	int         _setId;
	int         _sceneId;
	VQAPlayer   _vqaPlayer;
	int         _defaultLoop;
	int         _nextSetId;
	int         _nextSceneId;
	int         _frame;
	Vector3     _actorStartPosition;
	int         _actorStartFacing;
	bool        _playerWalkedIn;
	Regions*    _regions;
	Regions*    _exits;

public:
	Scene(BladeRunnerEngine *vm)
		: _vm(vm),
		  _set(new Set(vm)),
		  _setId(-1),
		  _sceneId(-1),
		  _vqaPlayer(vm, vm->_view),
		  _defaultLoop(0),
		  _nextSetId(-1),
		  _nextSceneId(-1),
		  _playerWalkedIn(false),
		  _regions(new Regions()),
		  _exits(new Regions())
	{}

	~Scene() {
		delete _set;
		delete _regions;
		delete _exits;
	}

	bool open(int setId, int sceneId, bool isLoadingGame);
	int  advanceFrame(Graphics::Surface &surface, uint16 *&zBuffer);
	void setActorStart(Vector3 position, int facing);
	int getSetId();
	int findObject(char *objectName);
	bool objectSetHotMouse(int objectId);
	bool objectGetBoundingBox(int objectId, BoundingBox *boundingBox);
};

} // End of namespace BladeRunner

#endif
