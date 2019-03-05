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
 */
#include "scene.h"
#include "dragons.h"
#include "actor.h"
#include "background.h"
#include "dragonini.h"
#include "dragonimg.h"
#include "inventory.h"
#include "screen.h"
#include "actorresource.h"
#include "scriptopcodes.h"

namespace Dragons {


Scene::Scene(DragonsEngine *vm, Screen *screen, ScriptOpcodes *scriptOpcodes, BigfileArchive *bigfileArchive, ActorManager *actorManager, DragonRMS *dragonRMS, DragonINIResource *dragonINIResource)
		: _vm(vm), _screen(screen), _scriptOpcodes(scriptOpcodes), _stage(0), _bigfileArchive(bigfileArchive), _actorManager(actorManager), _dragonRMS(dragonRMS), _dragonINIResource(dragonINIResource) {
	_backgroundLoader = new BackgroundResourceLoader(_bigfileArchive, _dragonRMS);
}
void Scene::loadScene(uint32 sceneId, uint32 cameraPointId) {
	// TODO
	loadSceneData(sceneId, cameraPointId);

	if (sceneId != 4) {
		Actor *cursor = _actorManager->getActor(0);
		//TODO update cursor.

	}
	//TODO
	if (!(sceneId & 0x8000)) {
		byte *obd = _dragonRMS->getObdDataFieldC(sceneId);
		ScriptOpCall scriptOpCall;
		scriptOpCall._code = obd + 4;
		scriptOpCall._codeEnd = scriptOpCall._code + READ_LE_UINT32(obd);
		_scriptOpcodes->runScript(scriptOpCall);
	}
	DragonINI *ini = _dragonINIResource->getRecord(0xc4);
	ini->field_12 = 0;
}

void Scene::loadSceneData(uint32 sceneId, uint32 cameraPointId) {
	bool isUnkFlag2Set = _vm->isUnkFlagSet(Dragons::ENGINE_UNK1_FLAG_2);

	_vm->setUnkFlags(Dragons::ENGINE_UNK1_FLAG_2 | Dragons::ENGINE_UNK1_FLAG_8);

	for(int i=0;i < _dragonINIResource->totalRecords(); i++) {
		DragonINI *ini = _dragonINIResource->getRecord(i);
		ini->field_10 = -1;
		ini->field_1a_flags_maybe &= ~Dragons::INI_FLAG_10;
	}

	if (!(sceneId & 0x8000)) {
		byte *obd = _dragonRMS->getObdDataField10(sceneId);
		ScriptOpCall scriptOpCall;
		scriptOpCall._code = obd + 4;
		scriptOpCall._codeEnd = scriptOpCall._code + READ_LE_UINT32(obd);
		_currentSceneId = -1;
		_scriptOpcodes->runScript(scriptOpCall);
		_currentSceneId = (uint16)(sceneId & 0x7fff);
	}

	_actorManager->clearActorFlags(2);
	//TODO sub_8003fadc(); might be fade related

	_vm->clearFlags(Dragons::ENGINE_FLAG_20);
	_vm->setUnkFlags(Dragons::ENGINE_UNK1_FLAG_10);

	_vm->call_fade_related_1f();
	// TODO 0x8002f7c4

	_stage = _backgroundLoader->load(sceneId);

	_camera = _stage->getPoint2(cameraPointId);

	debug("Flicker: (%X, %X)", _camera.x, _camera.y);

	if (_camera.x > 160) {
		_camera.x -= 160;
	} else {
		_camera.x = 0;
	}

	if (_camera.y > 100) {
		_camera.y -= 100;
	} else {
		_camera.y = 0;
	}

	if (_camera.x + 320 >= _stage->getWidth()) {
		_camera.x = _stage->getWidth() - 320;
	}

	if (_camera.y + 200 >= _stage->getHeight()) {
		_camera.y = _stage->getHeight() - 200;
	}

	debug("Camera: (%d, %d)", _camera.x, _camera.y);

	// 0x8002ff80
	// TODO fade_related_calls_with_1f();
	_vm->clearUnkFlags(Dragons::ENGINE_UNK1_FLAG_10);
	_vm->setFlags(Dragons::ENGINE_FLAG_20);
	// TODO reset vsync_updater_function
	_vm->setFlags(Dragons::ENGINE_FLAG_200);
	_actorManager->clearActorFlags(2);
	// TODO 0x8002fff0

	for(int i=0;i < _dragonINIResource->totalRecords(); i++) {
		DragonINI *ini = _dragonINIResource->getRecord(i);
		if (ini->sceneId == sceneId) {
			if (ini->field_1a_flags_maybe & 1) {
				Actor *actor = _actorManager->loadActor(ini->actorResourceId, ini->sequenceId, ini->x, ini->y, 0);

				if (actor) {
					ini->actor = actor;
					if (ini->sequenceId & 0x1000) {
						actor->frame_flags |= 0x10;
					} else {
						if (ini->field_1a_flags_maybe & 0x2000) {
							actor->frame_flags |= 0x20;
						} else {
							actor->frame_flags &= 0xffef;
						}
					}

					actor->_sequenceID2 = ini->field_20_actor_field_14;

					if (ini->field_1a_flags_maybe & 2) {
						actor->flags |= Dragons::ACTOR_FLAG_80;
					} else {
						actor->flags &= 0xfeff;
					}

					if (ini->field_1a_flags_maybe & 0x20) {
						actor->flags |= Dragons::ACTOR_FLAG_100;
					} else {
						actor->flags &= 0xfeff;
					}

					if (ini->field_1a_flags_maybe & 4) {
						actor->flags |= Dragons::ACTOR_FLAG_8000;
					} else {
						actor->flags &= 0x7fff;
					}

					if (ini->field_1a_flags_maybe & 0x100) {
						actor->flags |= Dragons::ACTOR_FLAG_4000;
					} else {
						actor->flags &= 0xbfff;
					}
//
//				Graphics::Surface *s = actor->getCurrentFrame();
//				int x = ini->x - actor->frame_vram_x;
//				int y = ini->y - actor->frame_vram_y;
//				if (x >= 0 && y >= 0 && x + s->w < 320 && y + s->h < 200) {
//					debug("Actor %d, %d %d (%d, %d)", actor->_actorID, ini->actorResourceId, ini->field_1a_flags_maybe, ini->x, ini->y);
//					_stage->getFgLayer()->copyRectToSurface(*s, x, y, Common::Rect(s->w, s->h));
//				}
				}
				// _stage->getFgLayer()->drawLine(ini->x, ini->y, ini->x + 8, ini->y + 8, 0x7c00);
				//break;
			} else {
				if (ini->iptIndex_maybe != -1) {
					loadImageOverlay(ini->iptIndex_maybe);
				}
			}
		}
	}
	_currentSceneId = (uint16)(sceneId & 0x7fff); //TODO is this the right spot for this?

	// 0x80030458
	DragonINI *ini = _vm->getINI(1);
	if (ini->actor && _vm->_dragonINIResource->getFlickerRecord() && _vm->_dragonINIResource->getFlickerRecord()->sceneId == _currentSceneId) {
		ini->actor->setFlag(Dragons::ACTOR_FLAG_100);
		ini->actor->priorityLayer = 0;
	}

	DragonINI *flicker = _vm->_dragonINIResource->getFlickerRecord();

	if (flicker && flicker->sceneId != 0) {
		flicker->field_20_actor_field_14 = _vm->data_800633fa;
		if (flicker->actor) {
			flicker->actor->_sequenceID2 = _vm->data_800633fa;
			flicker->actor->setFlag(Dragons::ACTOR_FLAG_4);
		}
	}

	// 0x800305bc
	_vm->_inventory->loadScene(_currentSceneId);

	// 0x8003070c
	// TODO sub_80013b3c(); // palette related.

	if (_vm->_inventory->isVisible()) {
		_vm->_inventory->hide();
	}

	if (!_vm->isFlagSet(Dragons::ENGINE_FLAG_10000)) {
		_vm->setFlags(Dragons::ENGINE_FLAG_10);
	}

	_vm->setFlags(Dragons::ENGINE_FLAG_1);
	_vm->setFlags(Dragons::ENGINE_FLAG_200);
	_vm->setFlags(Dragons::ENGINE_FLAG_4000000);

	if (flicker && flicker->sceneId == _currentSceneId) {

		flicker->actor->updateSequence((uint16)flicker->actor->_sequenceID2);
	}

	_vm->clearUnkFlags(Dragons::ENGINE_UNK1_FLAG_2);
	_vm->clearUnkFlags(Dragons::ENGINE_UNK1_FLAG_8);

	if (isUnkFlag2Set) {
		_vm->setUnkFlags(Dragons::ENGINE_UNK1_FLAG_2);
	}

	if (!(sceneId & 0x8000)) {
		byte *obd = _dragonRMS->getObdData(sceneId);
		ScriptOpCall scriptOpCall;
		scriptOpCall._code = obd + 4;
		scriptOpCall._codeEnd = scriptOpCall._code + READ_LE_UINT32(obd);
		_scriptOpcodes->runScript(scriptOpCall);
	}

}

void Scene::draw() {
	Common::Rect rect(_camera.x, _camera.y, 320, 200);

	for(uint16 priority = 1; priority < 16; priority++) {
		if (priority == 1) {
			_screen->copyRectToSurface(*_stage->getBgLayer(), 0, 0, rect);
		} else if (priority == 2) {
			_screen->copyRectToSurface(*_stage->getMgLayer(), 0, 0, rect);
		} else if (priority == 3) {
			_screen->copyRectToSurface(*_stage->getFgLayer(), 0, 0, rect);
		}

		for (uint16 i = 0; i < DRAGONS_ENGINE_NUM_ACTORS; i++) {
			Actor *actor = _actorManager->getActor(i);
			if (actor->x_pos == -100 && actor->y_pos == 100) {
				actor->priorityLayer = 0;
				continue;
			}

			if (actor->flags & Dragons::ACTOR_FLAG_40 &&
				!(actor->flags & Dragons::ACTOR_FLAG_400) &&
				actor->surface &&
				actor->frame->width != 0 &&
				actor->frame->height != 0
				) {
				Graphics::Surface *s = actor->surface;
				int x = actor->x_pos - actor->frame->xOffset - _camera.x;
				int y = actor->y_pos - actor->frame->yOffset - _camera.y;
				//int x = ini->x;// - actor->frame_vram_x;
				//int y = ini->y;// - actor->frame_vram_y;
				if (actor->priorityLayer == priority) { //} && x + s->w < 320 && y + s->h < 200) {
					debug(4, "Actor %d %s (%d, %d) w:%d h:%d Priority: %d", actor->_actorID, actor->_actorResource->getFilename(), x,
						  y,
						  s->w, s->h, actor->priorityLayer);
					_screen->copyRectToSurface(*s, x, y, Common::Rect(s->w, s->h), (bool)(actor->frame->flags & Dragons::FRAME_FLAG_FLIP_X));
				} else {
					debug(4, "Actor (not displayed) %d %s (%d, %d) Priority: %d", actor->_actorID,
						  actor->_actorResource->getFilename(),
						  x, y, actor->priorityLayer);
				}
			}
		}
	}
}

int16 Scene::getPriorityAtPosition(Common::Point pos) {
	return _stage->getPriorityAtPoint(pos);
}

bool Scene::contains(DragonINI *ini) {
	assert(ini);
	return ini->sceneId == _currentSceneId;
}

byte *Scene::getPalette() {
	assert(_stage);
	return _stage->getPalette();
}

uint16 Scene::getSceneId() {
	return (uint16)_currentSceneId;
}

Common::Point Scene::getPoint(uint32 pointIndex) {
	return _stage->getPoint2(pointIndex);
}

uint16 Scene::getStageWidth() {
	return _stage->getWidth();
}

uint16 Scene::getStageHeight() {
	return _stage->getHeight();
}

void Scene::loadImageOverlay(uint16 iptId) {
	IMG *img =_vm->_dragonIMG->getIMG(iptId);
	if (img->h != 0) {
		if (img->field_e <= 2) {
			_stage->overlayImage(img->layerNum - 1, img->data, img->x, img->y, img->w, img->h);
		}

		if (img->field_e == 2 || img->field_e == 0) {
			// error("img->field_e == 2 || img->field_e == 0");
			//TODO what does this do? Do we need it? 0x800177f8
		}
	}
}

void Scene::removeImageOverlay(uint16 iptId) {
	IMG *img =_vm->_dragonIMG->getIMG(iptId);
	_stage->restoreTiles(img->layerNum - 1, img->x, img->y, img->w, img->h);
}

} // End of namespace Dragons
