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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "common/scummsys.h"
#include "mads/mads.h"
#include "mads/conversations.h"
#include "mads/scene.h"
#include "mads/phantom/phantom_scenes.h"
#include "mads/phantom/phantom_scenes1.h"

namespace MADS {

namespace Phantom {

void Scene1xx::setAAName() {
	_game._aaName = Resources::formatAAName(_globals[kTempInterface]);
	_vm->_palette->setEntry(254, 43, 47, 51);
}

void Scene1xx::sceneEntrySound() {
	if (!_vm->_musicFlag)
		return;

	if (_globals[kDoneBrieConv203] == 3) {
		_vm->_sound->command(39);
	} else {
		switch (_scene->_nextSceneId) {
		case 101:
			if (!_game._visitedScenes._sceneRevisited)
				_vm->_sound->command(38);
			else
				_vm->_sound->command(16);
			break;

		case 102:
			if (_scene->_priorSceneId == 104)
				_vm->_sound->command(27);
			else
				_vm->_sound->command(16);
			break;

		case 103:
			if ((_globals[kJacquesStatus] == 2)) {
				_vm->_sound->command(32);
				_globals[kJacquesStatus] = 3;
			} else
				_vm->_sound->command(16);
			break;

		case 104:
			if ((_vm->_gameConv->_restoreRunning == 7) || (_scene->_priorSceneId == 301)) {
				_vm->_sound->command(33);
			} else if (!_globals[kRoom103104Transition] && !_globals[kObservedPhan104]) {
				_vm->_sound->command(37);
				_globals[kObservedPhan104] = true;
			} else
				_vm->_sound->command(16);
			break;

		case 111:
			if (_scene->_priorSceneId == 150)
				_vm->_sound->command(38);
			else if (_globals[kLeaveAngelMusicOn])
				_vm->_sound->command(34);
			else
				_vm->_sound->command(16);
			break;
		case 113:
			if (_globals[kLeaveAngelMusicOn])
				_vm->_sound->command(34);
			else if (_globals[kCurrentYear] == 1993)
				_vm->_sound->command(36);
			else
				_vm->_sound->command(35);
			break;

		default:
			if ((_scene->_priorSceneId != 204) && (_scene->_nextSceneId != 150))
				_vm->_sound->command(16);
			break;
		}
	}
}

void Scene1xx::setPlayerSpritesPrefix() {
	_vm->_sound->command(5);

	Common::String oldName = _game._player._spritesPrefix;

	if (!_game._player._forcePrefix)
		_game._player._spritesPrefix = "RAL";

	if (oldName != _game._player._spritesPrefix)
		_game._player._spritesChanged = true;

	_game._player._scalingVelocity = true;}

/*------------------------------------------------------------------------*/

Scene101::Scene101(MADSEngine *vm) : Scene1xx(vm) {
	_chanStatus = -1;
	_wipeStatus = -1;
	_callingStatus = -1;
	_chandelierStatus = -1;
	_callingFrame = -1;
	_chandelierFrame = -1;
	_convCounter = 0;
	_talkCounter = -1;
	_brieAnimId = 0;
	_startWalkingFl = false;
	_startWalking0Fl = false;
	_anim0Running = false;
	_anim1Running = false;
	_startSittingFl = false;
}

void Scene101::synchronize(Common::Serializer &s) {
	Scene1xx::synchronize(s);
	s.syncAsSint16LE(_chanStatus);
	s.syncAsSint16LE(_wipeStatus);
	s.syncAsSint16LE(_callingStatus);
	s.syncAsSint16LE(_chandelierStatus);
	s.syncAsSint16LE(_callingFrame);
	s.syncAsSint16LE(_chandelierFrame);
	s.syncAsSint16LE(_convCounter);
	s.syncAsSint16LE(_talkCounter);
	s.syncAsSint16LE(_brieAnimId);
	s.syncAsByte(_startWalkingFl);
	s.syncAsByte(_startWalking0Fl);
	s.syncAsByte(_anim0Running);
	s.syncAsByte(_anim1Running);
	s.syncAsByte(_startSittingFl);
}

void Scene101::setup() {
	setPlayerSpritesPrefix();
	setAAName();
	_scene->addActiveVocab(NOUN_MONSIEUR_BRIE);
}

void Scene101::enter() {
	_vm->_disableFastwalk = true;

	if (_scene->_priorSceneId != RETURNING_FROM_DIALOG) {
		_chanStatus = _wipeStatus = -1;
		_startWalkingFl = _startWalking0Fl = false;
		_anim0Running = _anim1Running = false;
		_startSittingFl = false;
	}

	// Load Dialogs
	_vm->_gameConv->get(0);
	_vm->_gameConv->get(1);

	if (_globals[kCurrentYear] == 1993) {
		_globals._spriteIndexes[0] = _scene->_sprites.addSprites(formAnimName('z', -1));
		_globals._sequenceIndexes[0] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[0], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[0], 14);
	} else {
		_scene->_hotspots.activate(NOUN_CHANDELIER, false);
	}

	if (_globals[kBrieTalkStatus] == 0) {
		_game._player.firstWalk(Common::Point(-20, 75), FACING_EAST, Common::Point(18, 79), FACING_EAST, true);
		_callingStatus = 0;
		_chandelierStatus = 3;
		_game._player.setWalkTrigger(50);

		_globals._animationIndexes[1] = _scene->loadAnimation(formAnimName('b', 9), 1);
		_globals._animationIndexes[0] = _scene->loadAnimation(formAnimName('b', 8), 1);

		_anim0Running  = true;
		_anim1Running  = true;

		_brieAnimId = _scene->_dynamicHotspots.add(NOUN_MONSIEUR_BRIE, VERB_WALKTO, SYNTAX_SINGULAR_MASC, -1, Common::Rect(0, 0, 0, 0));
		_scene->_dynamicHotspots[_brieAnimId]._articleNumber = PREP_ON;
		_scene->_dynamicHotspots.setPosition(_brieAnimId, Common::Point(490, 119), FACING_NONE);
		_scene->setDynamicAnim(_brieAnimId, 0, 0);
		_scene->setDynamicAnim(_brieAnimId, 0, 1);
		_scene->setDynamicAnim(_brieAnimId, 0, 2);
		_scene->setDynamicAnim(_brieAnimId, 0, 3);
		_scene->setDynamicAnim(_brieAnimId, 0, 4);

		int tmpIdx = _scene->_dynamicHotspots.add(NOUN_MONSIEUR_BRIE, VERB_WALKTO, SYNTAX_SINGULAR_MASC, -1, Common::Rect(0, 0, 0, 0));
		_scene->_dynamicHotspots[tmpIdx]._articleNumber = PREP_ON;
		_scene->_dynamicHotspots.setPosition(tmpIdx, Common::Point(25, 80), FACING_NONE);
		_scene->setDynamicAnim(tmpIdx, 1, 1);
		_scene->setDynamicAnim(tmpIdx, 1, 2);

		_talkCounter = 0;
	} else if (_globals[kBrieTalkStatus] == 1) {
		_globals._animationIndexes[1] = _scene->loadAnimation(formAnimName('b', 9), 1);
		_brieAnimId = _scene->_dynamicHotspots.add(NOUN_MONSIEUR_BRIE, VERB_WALKTO, SYNTAX_SINGULAR_MASC, -1, Common::Rect(0, 0, 0, 0));
		_scene->_dynamicHotspots[_brieAnimId]._articleNumber = PREP_ON;
		_scene->setDynamicAnim(_brieAnimId, 1, 1);
		_scene->setDynamicAnim(_brieAnimId, 1, 2);
		_anim1Running = true;
		_talkCounter = 0;
		_chandelierStatus = 3;

		if (_vm->_gameConv->_restoreRunning == 1) {
			_vm->_gameConv->run(1);
			_vm->_gameConv->exportPointer(&_globals[kPlayerScore]);
			_chandelierStatus = 4;
			_scene->setAnimFrame(_globals._animationIndexes[1], 25);
		}
	} else if (_scene->_priorSceneId == 202) {
		if (_globals[kJacquesStatus] == 1)
			_globals[kJacquesStatus] = 2;
		_game._player.firstWalk(Common::Point(-20, 75), FACING_EAST, Common::Point(18, 79), FACING_EAST, true);
	} else if ((_scene->_priorSceneId == 102) || (_scene->_priorSceneId != RETURNING_FROM_LOADING)) {
		_game._player.firstWalk(Common::Point(655, 130), FACING_WEST, Common::Point(625, 127), FACING_WEST, true);
		_scene->setCamera(Common::Point(320, 0));
	}

	sceneEntrySound();
}

void Scene101::step() {
	if (_anim0Running)
		handleAnimation0();

	if ((_globals[kWalkerConverse] == 2) || (_globals[kWalkerConverse] == 3)) {
		++_convCounter;
		if (_convCounter > 200)
			_globals[kWalkerConverse] = _vm->getRandomNumber(1, 4);
	}

	if (_anim1Running) {
		handleAnimation1();

		if (_scene->getAnimFrame(_globals._animationIndexes[1]) == 80) {
			_game._player._stepEnabled = true;
			_game._player.setWalkTrigger(55);
		}
	}

	if (!_startSittingFl && (_globals[kBrieTalkStatus] != 2)) {
		warning("TODO: Add a check on view port x > 200");
		_startSittingFl = true;
		_game._player.walk(Common::Point(490, 119), FACING_NORTHEAST);
		_game._player._stepEnabled = false;
		_game._player.setWalkTrigger(55);
		_chandelierStatus = 4;
	}

	if (_game._trigger == 55) {
		_game._player._stepEnabled = true;
		_vm->_gameConv->run(1);
		_vm->_gameConv->exportPointer(&_globals[kPlayerScore]);
		_chandelierFrame = -1;
		_talkCounter = 0;
	}

	if (_game._trigger == 50) {
		_vm->_gameConv->run(0);
		_callingStatus = 1;
	}
}

void Scene101::preActions() {
	if (_action.isAction(VERB_EXIT_TO, NOUN_ORCHESTRA_PIT)) {
		if ((_globals[kBrieTalkStatus] == 2) || _startWalkingFl) {
			_game._player._walkOffScreenSceneId = 102;
			_globals[kBrieTalkStatus] = 2;
		} else {
			_vm->_gameConv->run(0);
			_game._player._needToWalk = false;
		}
	} else if (_action.isAction(VERB_EXIT_TO, NOUN_GRAND_FOYER)) {
		if ((_globals[kBrieTalkStatus] == 2) || _startWalkingFl)
			_game._player._walkOffScreenSceneId = 202;
		else {
			_vm->_gameConv->run(0);
			_game._player._needToWalk = false;
		}
	} else if (_action.isAction(VERB_TAKE, NOUN_MONSIEUR_BRIE))
		_vm->_dialogs->show(10121);
	else if (_action.isAction(VERB_TALK_TO, NOUN_MONSIEUR_BRIE) && (_globals[kBrieTalkStatus] == 2))
		_game._player._needToWalk = false;
}

void Scene101::actions() {
	if (_vm->_gameConv->_running == 0)
		handleConversation0();
	else if (_vm->_gameConv->_running == 1)
		handleConversation1();
	else if (_action._lookFlag) {
		if (_globals[kCurrentYear] == 1993)
			_vm->_dialogs->show(10110);
		else
			_vm->_dialogs->show(10111);
	} else if (_action.isAction(VERB_LOOK) || _action.isAction(VERB_LOOK_AT)) {
		if (_action.isObject(NOUN_AISLE)) {
			_vm->_dialogs->show(10112);
		} else if (_action.isObject(NOUN_CHANDELIER)) {
			_vm->_dialogs->show(10113);
		} else if (_action.isObject(NOUN_BACK_WALL)) {
			_vm->_dialogs->show(10114);
		} else if (_action.isObject(NOUN_SIDE_WALL)) {
			_vm->_dialogs->show(10115);
		} else if (_action.isObject(NOUN_SEATS)) {
			if ((_globals[kBrieTalkStatus] > 1) || _startWalkingFl)
				_vm->_dialogs->show(10119);
			else
				_vm->_dialogs->show(10116);
		} else if (_action.isObject(NOUN_GRAND_FOYER)) {
			_vm->_dialogs->show(10117);
		} else if (_action.isObject(NOUN_ORCHESTRA_PIT)) {
			_vm->_dialogs->show(10118);
		} else if (_action.isObject(NOUN_MONSIEUR_BRIE)) {
			_vm->_dialogs->show(10120);
		}
	} else if (_action.isAction(VERB_TALK_TO, NOUN_MONSIEUR_BRIE)) {
		if (_globals[kBrieTalkStatus] == 2)
			_vm->_dialogs->show(10122);
	} else if (!_action.isAction(VERB_TAKE, NOUN_MONSIEUR_BRIE))
		return;

	_action._inProgress = false;
}

void Scene101::handleConversation0() {
	_vm->_gameConv->setHeroTrigger(90);

	if (_game._trigger == 90) {
		_globals[kBrieTalkStatus] = 1;
		_startWalking0Fl = true;
	}
}

void Scene101::handleConversation1() {
	if ((_action._activeAction._verbId >= 0) && (_action._activeAction._verbId <= 27)) {
		int interlocutorFl = false;

		if (_game._trigger == 60) {
			switch (_action._activeAction._verbId) {
			case 0:
				_chandelierStatus = 6;
				_wipeStatus = 2;
				break;

			case 1:
				_chandelierStatus = 2;
				_chanStatus = 9;
				break;

			case 4:
				_chandelierStatus = 0;
				_chanStatus = -1;
				_wipeStatus = -1;
				break;

			case 8:
			case 10:
			case 18:
			case 22:
			case 24:
				_startWalkingFl = true;
				_chanStatus = -1;
				_wipeStatus = -1;
				_globals[kWalkerConverse] = 0;
				_vm->_gameConv->setInterlocutorTrigger(105);
				interlocutorFl = true;
				break;

			case 12:
				_chandelierStatus = 5;
				_chanStatus = -1;
				_wipeStatus = -1;
				break;

			default:
				break;
			}
		}

		if (!interlocutorFl)
			_vm->_gameConv->setInterlocutorTrigger(60);

		_vm->_gameConv->setHeroTrigger(70);

		_talkCounter = 0;

		if (_game._trigger == 60) {
			if (!_startWalkingFl)
				_globals[kWalkerConverse] = _vm->getRandomNumber(1, 4);

			_chandelierStatus = 2;
		} else if ((_game._trigger == 70) && !_startWalkingFl) {
			_chandelierStatus = 4;
			_chanStatus = -1;
			_wipeStatus = -1;
			if (!_startWalkingFl)
				_globals[kWalkerConverse] = _vm->getRandomNumber(2, 3);

			_convCounter = 0;
		}
	}
}

void Scene101::handleAnimation0() {
	int random;
	int reset_frame;

	if (_scene->getAnimFrame(_globals._animationIndexes[0]) != _callingFrame) {
		_callingFrame = _scene->getAnimFrame(_globals._animationIndexes[0]);
		reset_frame = -1;
		switch (_callingFrame) {
		case 1:
		case 9:
		case 12:
			if (_callingStatus == 1) {
				if (_callingFrame == 9) {
					if (_startWalking0Fl) {
						reset_frame = 13;
						_callingStatus = 3;
					} else
						_callingStatus = 2;
				} else
					reset_frame = 1;
			}

			if (_callingStatus == 0) {
				if (_startWalking0Fl) {
					reset_frame = 60;
					_callingStatus = 3;
				} else
					reset_frame = 0;
			}

			if (_callingStatus == 2) {
				if (_startWalking0Fl) {
					reset_frame = 13;
					_callingStatus = 3;
				} else {
					random = _vm->getRandomNumber(1, 2);
					++_talkCounter;

					if (_talkCounter < 18) {
						if (random == 1)
							reset_frame = 7;
						else
							reset_frame = 10;
					} else {
						reset_frame = 54;
						_callingStatus = 0;
					}
				}
			}
			break;

		case 53:
			_anim0Running    = false;
			_scene->freeAnimation(0);
			break;

		case 59:
			if (_startWalking0Fl) {
				reset_frame = 60;
				_callingStatus = 3;
			} else {
				reset_frame = 0;
				_callingStatus = 0;
			}
			break;

		case 66:
			reset_frame = 24;
			break;
		}

		if (reset_frame >= 0) {
			_scene->setAnimFrame(_globals._animationIndexes[0], reset_frame);
			_callingFrame = reset_frame;
		}
	}
}

void Scene101::handleAnimation1() {
	int random;
	int reset_frame;

	if (_scene->getAnimFrame(_globals._animationIndexes[1]) != _chandelierFrame) {
		_chandelierFrame = _scene->getAnimFrame(_globals._animationIndexes[1]);
		reset_frame = -1;
		switch (_chandelierFrame) {
		case 1:
			if (_chandelierStatus == 3)
				reset_frame = 0;
			break;

		case 11:
		case 14:
		case 17:
		case 19:
		case 26:
		case 44:
		case 333:
			if (_talkCounter == _chanStatus) {
				_chandelierStatus = 0;
				++_talkCounter;
				_chanStatus = -1;
			}

			if (_talkCounter == _wipeStatus) {
				_chandelierStatus = 6;
				++_talkCounter;
				_wipeStatus = -1;
			}

			if (_startWalkingFl) {
				if (_vm->_gameConv->_running == 1) {
					if (_talkCounter > 13)
						_chandelierStatus = 1;
				} else
					_chandelierStatus = 1;
			}

			switch (_chandelierStatus) {
			case 0:
				reset_frame = 27;
				_chandelierStatus = 2;
				break;

			case 1:
				_globals[kBrieTalkStatus] = 2;
				reset_frame = 45;
				if (_vm->_gameConv->_running == 1)
					_vm->_gameConv->stop();
				_scene->_dynamicHotspots.remove(_brieAnimId);
				_game._player._stepEnabled = false;
				break;

			case 2:
				random = _vm->getRandomNumber(1, 3);
				++_talkCounter;
				if (_talkCounter < 15) {
					if (random == 1)
						reset_frame = 12;
					else if (random == 2)
						reset_frame = 14;
					else if (random == 3)
						reset_frame = 17;
				} else {
					_chandelierStatus = 4;
					reset_frame = 25;
				}
				break;

			case 4:
				reset_frame = 25;
				break;

			case 5:
				reset_frame = 21;
				_chandelierStatus = 2;
				break;

			case 6:
				reset_frame = 316;
				_chandelierStatus = 2;
				break;

			default:
				break;
			}
			break;

		case 315:
			_scene->freeAnimation(1);
			break;
		}

		if (reset_frame >= 0) {
			_scene->setAnimFrame(_globals._animationIndexes[1], reset_frame);
			_chandelierFrame = reset_frame;
		}
	}
}

/*------------------------------------------------------------------------*/

Scene102::Scene102(MADSEngine *vm) : Scene1xx(vm) {
	_anim0Running = false;
}

void Scene102::synchronize(Common::Serializer &s) {
	Scene1xx::synchronize(s);

	s.syncAsByte(_anim0Running);
}

void Scene102::setup() {
	setPlayerSpritesPrefix();
	setAAName();
}

void Scene102::enter() {
	_anim0Running = false;

	_globals._spriteIndexes[2] = _scene->_sprites.addSprites(formAnimName('x', 0));
	_globals._spriteIndexes[3] = _scene->_sprites.addSprites("*RAL86");

	if (_globals[kCurrentYear] == 1993) {
		_globals._spriteIndexes[0] = _scene->_sprites.addSprites(formAnimName('z', -1));
		_scene->drawToBackground(_globals._sequenceIndexes[0], 1, Common::Point(-32000, -32000), 0, 100);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[0], 14);
	} else {
		_scene->_hotspots.activate(NOUN_CHANDELIER, false);
	}

	if (_scene->_priorSceneId == 101) {
		_game._player._playerPos = Common::Point(97, 79);
		_game._player._facing = FACING_SOUTHEAST;
		_globals._sequenceIndexes[2] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[2], false, 4);
		_game._player.walk(Common::Point(83, 87), FACING_SOUTHEAST);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[2], 14);
	} else if (_scene->_priorSceneId == 104) {
		// Player fell from pit -> death
		Common::Point deathPos = Common::Point(0, 0);
		int deathScale = 0;
		int deathDepth = 0;
		_game._player._stepEnabled = false;
		_game._player._visible = false;

		switch (_globals[36]) {
		case 0:
			deathPos = Common::Point(221, 57);
			deathScale = 50;
			deathDepth = 14;
			break;

		case 1:
			deathPos = Common::Point(219, 85);
			deathScale = 60;
			deathDepth = 6;
			break;

		case 2:
			deathPos = Common::Point(257, 138);
			deathScale = 76;
			deathDepth = 1;
			break;

		default:
			break;
		}
		_scene->_userInterface.emptyConversationList();
		_scene->_userInterface.setup(kInputConversation);
		_globals._sequenceIndexes[3] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[3], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[3], deathDepth);
		_scene->_sequences.setPosition(_globals._sequenceIndexes[3], deathPos);
		_scene->_sequences.setScale(_globals._sequenceIndexes[3], deathScale);
		_scene->_sequences.setTimingTrigger(120, 65);
		_globals._sequenceIndexes[2] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[2], false, 4);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[2], 14);
	} else if (_scene->_priorSceneId == 103 || _scene->_priorSceneId != RETURNING_FROM_LOADING) {
		_game._player._playerPos = Common::Point(282, 145);
		_game._player._facing = FACING_WEST;
		_anim0Running = true;
		_globals._animationIndexes[0] = _scene->loadAnimation(formAnimName('d', 1), 60);
	} else if (_scene->_priorSceneId == -1) {
		_globals._sequenceIndexes[2] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[2], false, 4);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[2], 14);
	}

	sceneEntrySound();
}

void Scene102::step() {
	if (_game._trigger == 60) {
		// Door closes
		_globals._sequenceIndexes[2] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[2], false, 4);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[2], 14);
		_anim0Running = false;
	} else if (_game._trigger == 65) {
		// Death
		if (_globals[kDeathLocation] == 0)
			_vm->_dialogs->show(10232);
		else
			_vm->_dialogs->show(10229);

		_vm->_sound->command(16);
		_scene->_nextSceneId = 104;
	}
}

void Scene102::preActions() {
	if (_action.isAction(VERB_OPEN, NOUN_ORCHESTRA_DOOR) || _action.isAction(VERB_PUSH, NOUN_ORCHESTRA_DOOR))
		_game._player.walk(Common::Point(282, 145), FACING_EAST);
}

void Scene102::actions() {
	if (_action.isAction(VERB_WALK_DOWN, NOUN_AISLE)) {
		_scene->_nextSceneId = 101;
	} else if (_action.isAction(VERB_WALK_THROUGH, NOUN_ORCHESTRA_DOOR) ||
		_action.isAction(VERB_PUSH, NOUN_ORCHESTRA_DOOR) ||
		_action.isAction(VERB_OPEN, NOUN_ORCHESTRA_DOOR)) {
		if (_anim0Running) {
			_scene->_sequences.setTimingTrigger(15, 70);
			_game._player._stepEnabled = false;
		} else {
			switch (_game._trigger) {
			case 70:	// try again
			case 0:
				_scene->deleteSequence(_globals._sequenceIndexes[2]);
				_globals._animationIndexes[0] = _scene->loadAnimation(formAnimName('d', 0), 1);
				_game._player._stepEnabled = false;
				_game._player._visible = false;
				break;
			case 1:
				_scene->_nextSceneId = 103;
				break;
			default:
				break;
			}
		}
	} else if (_action._lookFlag)
		_vm->_dialogs->show(10210);
	else if (_action.isAction(VERB_LOOK) || _action.isAction(VERB_LOOK_AT)) {
		if (_action.isObject(NOUN_PIT))
			_vm->_dialogs->show(10211);
		else if (_action.isObject(NOUN_SEATS))
			if (_globals[kCurrentYear] == 1881)
				_vm->_dialogs->show(10212);
			else
				_vm->_dialogs->show(10230);
		else if (_action.isObject(NOUN_ORCHESTRA_DOOR))
			_vm->_dialogs->show(10213);
		else if (_action.isObject(NOUN_CONDUCTORS_STAND))
			_vm->_dialogs->show(10214);
		else if (_action.isObject(NOUN_MUSIC_STAND) || _action.isObject(NOUN_MUSIC_STANDS))
			_vm->_dialogs->show(10215);
		else if (_action.isObject(NOUN_PROMPTERS_BOX))
			_vm->_dialogs->show(10217);
		else if (_action.isObject(NOUN_STAGE))
			_vm->_dialogs->show(10218);
		else if (_action.isObject(NOUN_APRON))
			_vm->_dialogs->show(10219);
		else if (_action.isObject(NOUN_SIDE_WALL))
			_vm->_dialogs->show(10220);
		else if (_action.isObject(NOUN_FOLDING_CHAIRS))
			_vm->_dialogs->show(10221);
		else if (_action.isObject(NOUN_AISLE))
			_vm->_dialogs->show(10222);
		else if (_action.isObject(NOUN_PROSCENIUM_ARCH))
			_vm->_dialogs->show(10223);
		else if (_action.isObject(NOUN_ACT_CURTAIN))
			_vm->_dialogs->show(10224);
		else if (_action.isObject(NOUN_IN_ONE))
			_vm->_dialogs->show(10225);
		else if (_action.isObject(NOUN_IN_TWO))
			_vm->_dialogs->show(10226);
		else if (_action.isObject(NOUN_LEG))
			_vm->_dialogs->show(10227);
		else if (_action.isObject(NOUN_CHANDELIER))
			_vm->_dialogs->show(10231);
	} else if (_action.isAction(VERB_CLOSE, NOUN_ORCHESTRA_DOOR))
		_vm->_dialogs->show(10228);

	_game._player._stepEnabled = false;
}

/*------------------------------------------------------------------------*/
Scene103::Scene103(MADSEngine *vm) : Scene1xx(vm) {
	_jacquesAction = -1;
	_lastRandom = -1;
	_standPosition = -1;
	_hotspotPrompt1 = -1;
	_hotspotPrompt2 = -1;
	_hotspotPrompt3 = -1;
	_hotspotPrompt4 = -1;
	_hotspotPrompt5 = -1;
	_hotspotRightFloor1 = -1;
	_hotspotRightFloor2 = -1;
	_hotspotLeftFloor1 = -1;
	_hotspotLeftFloor2 = -1;
	_hotspotGentleman = -1;
	_convCount = -1;
	_lastStairFrame = -1;
	_lastJacquesFrame = -1;
	_talkCount = -1;

	_anim0ActvFl = false;
	_anim1ActvFl = false;
	_anim2ActvFl = false;
	_anim3ActvFl = false;
	_anim4ActvFl = false;
	_anim5ActvFl = false;
	_anim6ActvFl = false;
	_climbThroughTrapFl = false;
	_guardFrameFl = false;
	_sitFl = false;
}

void Scene103::synchronize(Common::Serializer &s) {
	Scene1xx::synchronize(s);

	s.syncAsSint16LE(_jacquesAction);
	s.syncAsSint16LE(_lastRandom);
	s.syncAsSint16LE(_standPosition);
	s.syncAsSint16LE(_hotspotPrompt1);
	s.syncAsSint16LE(_hotspotPrompt2);
	s.syncAsSint16LE(_hotspotPrompt3);
	s.syncAsSint16LE(_hotspotPrompt4);
	s.syncAsSint16LE(_hotspotPrompt5);
	s.syncAsSint16LE(_hotspotRightFloor1);
	s.syncAsSint16LE(_hotspotRightFloor2);
	s.syncAsSint16LE(_hotspotLeftFloor1);
	s.syncAsSint16LE(_hotspotLeftFloor2);
	s.syncAsSint16LE(_convCount);
	s.syncAsSint16LE(_lastStairFrame);

	s.syncAsByte(_anim0ActvFl);
	s.syncAsByte(_anim1ActvFl);
	s.syncAsByte(_anim2ActvFl);
	s.syncAsByte(_anim3ActvFl);
	s.syncAsByte(_anim4ActvFl);
	s.syncAsByte(_anim5ActvFl);
	s.syncAsByte(_anim6ActvFl);
	s.syncAsByte(_climbThroughTrapFl);
	s.syncAsByte(_guardFrameFl);
	s.syncAsByte(_sitFl);
}

void Scene103::setup() {
	setPlayerSpritesPrefix();
	setAAName();

	_scene->_initialVariant = 0;

	if ((_globals[kPrompterStandStatus] == 1) || (_globals[kCurrentYear] == 1881)) {
		_scene->_initialVariant = 1;
		if ((_globals[kJacquesStatus] == 0) && (_globals[kCurrentYear] == 1881))
			_scene->_initialVariant = 2;
		else if ((_globals[kJacquesStatus] >= 1) && (_globals[kCurrentYear] == 1881))
			_scene->_initialVariant = 3;
	}

	_scene->addActiveVocab(NOUN_PROMPTERS_STAND);
	_scene->addActiveVocab(NOUN_JACQUES);
	_scene->addActiveVocab(NOUN_GENTLEMAN);
	_scene->addActiveVocab(NOUN_CLIMB);
}

void Scene103::enter() {
	if (_scene->_priorSceneId != RETURNING_FROM_LOADING) {
		_anim0ActvFl = false;
		_anim1ActvFl = false;
		_anim2ActvFl = false;
		_anim3ActvFl = false;
		_anim4ActvFl = false;
		_anim5ActvFl = false;
		_anim6ActvFl = false;
		_climbThroughTrapFl = false;
		_guardFrameFl = false;
		_sitFl = false;
		_jacquesAction = 1;
		_lastRandom = 0;
		_standPosition = 0;
	}

	if (_globals[kJacquesStatus] >= 1) {
		if (_game._objects.isInRoom(OBJ_KEY)) {
			_globals._spriteIndexes[5] = _scene->_sprites.addSprites(formAnimName('x', 2), false);
			_globals._spriteIndexes[8] = _scene->_sprites.addSprites("*RRD_9", false);
		}
		_globals._spriteIndexes[3] = _scene->_sprites.addSprites(formAnimName('f', 3), false);
		_globals._spriteIndexes[6] = _scene->_sprites.addSprites(formAnimName('c', 1), false);
		_globals._spriteIndexes[7] = _scene->_sprites.addSprites(formAnimName('f', 1), false);
	} else {
		_globals._spriteIndexes[3] = _scene->_sprites.addSprites(formAnimName('f', 0), false);
		_globals._spriteIndexes[10] = _scene->_sprites.addSprites(formAnimName('a', 3), false);
	}

	_globals._spriteIndexes[9] = _scene->_sprites.addSprites(formAnimName('x', 3), false);
	_globals._spriteIndexes[11] = _scene->_sprites.addSprites(formAnimName('a', 2), false);
	_globals._spriteIndexes[12] = _scene->_sprites.addSprites(formAnimName('f', 2), false);
	_globals._spriteIndexes[0] = _scene->_sprites.addSprites(formAnimName('x', 0), false);
	_globals._spriteIndexes[1] = _scene->_sprites.addSprites(formAnimName('x', 1), false);
	_globals._spriteIndexes[2] = _scene->_sprites.addSprites("*RDR_6", false);

	adjustRails(_scene->_initialVariant);

	_scene->_hotspots.activate(NOUN_JACQUES, false);
	_scene->_hotspots.activate(NOUN_KEY,  false);

	_vm->_gameConv->get(12);

	if (_globals[kTrapDoorStatus] == 0) {
		_globals._sequenceIndexes[0] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[0], false, 5);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[0], 14);

		_globals._sequenceIndexes[9] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[9], false, 2);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[9], 3);
	} else if (_globals[kTrapDoorStatus] == 1) {
		_globals._sequenceIndexes[0] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[0], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[0], 14);

		_globals._sequenceIndexes[9] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[9], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[9], 3);
	}

	Common::Point promptPos;
	Facing promptFacing;

	if (_globals[kJacquesStatus] == 0) {
		promptPos = Common::Point(115, 142);
		promptFacing = FACING_NORTHEAST;
	} else {
		promptPos = Common::Point(171, 142);
		promptFacing = FACING_NORTHWEST;
	}

	if ((_globals[kPrompterStandStatus] == 1) || (_globals[kCurrentYear] == 1881)) {
		if (_globals[kJacquesStatus] >= 1) {
			_globals._sequenceIndexes[7] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[7], false, 1);
			_scene->_sequences.setDepth(_globals._sequenceIndexes[7], 1);

			_globals._sequenceIndexes[3] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[3], false, 1);
			_scene->_sequences.setDepth(_globals._sequenceIndexes[3], 4);
			_scene->_sequences.setPosition(_globals._sequenceIndexes[3], Common::Point(154, 139));

		} else {
			_globals._sequenceIndexes[3] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[3], false, 1);
			_scene->_sequences.setDepth(_globals._sequenceIndexes[3], 4);
			_scene->_sequences.setPosition(_globals._sequenceIndexes[3], Common::Point(154, 139));

			_globals._sequenceIndexes[12] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[12], false, 1);
			_scene->_sequences.setDepth(_globals._sequenceIndexes[12], 1);
		}

		_hotspotPrompt1 = _scene->_dynamicHotspots.add(NOUN_PROMPTERS_STAND, VERB_WALKTO, SYNTAX_SINGULAR, -1, Common::Rect(121, 79, 40, 63));
		_scene->_dynamicHotspots[_hotspotPrompt1]._articleNumber = PREP_ON;
		_scene->_dynamicHotspots.setPosition(_hotspotPrompt1, promptPos, promptFacing);

		_hotspotPrompt2 = _scene->_dynamicHotspots.add(NOUN_PROMPTERS_STAND, VERB_WALKTO, SYNTAX_SINGULAR, -1, Common::Rect(161, 67, 16, 75));
		_scene->_dynamicHotspots[_hotspotPrompt2]._articleNumber = PREP_ON;
		_scene->_dynamicHotspots.setPosition(_hotspotPrompt2, promptPos, promptFacing);

		_hotspotPrompt3 = _scene->_dynamicHotspots.add(NOUN_PROMPTERS_STAND, VERB_WALKTO, SYNTAX_SINGULAR, -1, Common::Rect(177, 90, 18, 52));
		_scene->_dynamicHotspots[_hotspotPrompt3]._articleNumber = PREP_ON;
		_scene->_dynamicHotspots.setPosition(_hotspotPrompt3, promptPos, promptFacing);

		_hotspotPrompt4 = _scene->_dynamicHotspots.add(NOUN_PROMPTERS_STAND, VERB_WALKTO, SYNTAX_SINGULAR, -1, Common::Rect(114, 100, 7, 38));
		_scene->_dynamicHotspots[_hotspotPrompt4]._articleNumber = PREP_ON;
		_scene->_dynamicHotspots.setPosition(_hotspotPrompt4, promptPos, promptFacing);

		_hotspotPrompt5 = _scene->_dynamicHotspots.add(NOUN_PROMPTERS_STAND, NOUN_CLIMB, SYNTAX_SINGULAR, -1, Common::Rect(121, 49, 40, 30));
		_scene->_dynamicHotspots[_hotspotPrompt5]._articleNumber = PREP_ON;
		_scene->_dynamicHotspots.setPosition(_hotspotPrompt5, Common::Point(196, 134), FACING_SOUTHWEST);
		_scene->_dynamicHotspots.setCursor(_hotspotPrompt5, CURSOR_GO_UP);

		_hotspotRightFloor1 = _scene->_dynamicHotspots.add(NOUN_FLOOR, VERB_WALK_ACROSS, SYNTAX_SINGULAR, -1, Common::Rect(154, 6, 41, 6));
		_scene->_dynamicHotspots[_hotspotRightFloor1]._articleNumber = PREP_ON;
		_scene->_dynamicHotspots.setPosition(_hotspotRightFloor1, Common::Point(171, 142), FACING_NONE);

		_hotspotRightFloor2 = _scene->_dynamicHotspots.add(NOUN_FLOOR, VERB_WALK_ACROSS, SYNTAX_SINGULAR, -1, Common::Rect(114, 136, 32, 6));
		_scene->_dynamicHotspots[_hotspotRightFloor2]._articleNumber = PREP_ON;
		_scene->_dynamicHotspots.setPosition(_hotspotRightFloor2, Common::Point(127, 140), FACING_NONE);

		if ((_globals[kJacquesStatus] == 0) && (_globals[kCurrentYear] == 1881)) {
			if (_globals[kJacquesNameIsKnown] >= 1) {
				_hotspotGentleman = _scene->_dynamicHotspots.add(NOUN_JACQUES, VERB_WALKTO, SYNTAX_SINGULAR_MASC, -1, Common::Rect(156, 116, 33, 31));
				_scene->_dynamicHotspots[_hotspotGentleman]._articleNumber = PREP_ON;
				_scene->_dynamicHotspots.setPosition(_hotspotGentleman, Common::Point(206, 148), FACING_NORTHWEST);
			} else {
				_hotspotGentleman = _scene->_dynamicHotspots.add(NOUN_GENTLEMAN, VERB_WALKTO, SYNTAX_MASC_NOT_PROPER, -1, Common::Rect(156, 116, 33, 31));
				_scene->_dynamicHotspots[_hotspotGentleman]._articleNumber = PREP_ON;
				_scene->_dynamicHotspots.setPosition(_hotspotGentleman, Common::Point(206, 148), FACING_NORTHWEST);
			}
			int tmpIdx = _scene->_dynamicHotspots.add(NOUN_FLOOR, VERB_WALK_ACROSS, SYNTAX_SINGULAR, -1, Common::Rect(149, 140, 13, 7));
			_scene->_dynamicHotspots[tmpIdx]._articleNumber = PREP_ON;
			_scene->_dynamicHotspots.setPosition(tmpIdx, Common::Point(155, 144), FACING_NONE);

			tmpIdx = _scene->_dynamicHotspots.add(NOUN_FLOOR, VERB_WALK_ACROSS, SYNTAX_SINGULAR, -1, Common::Rect(187, 136, 8, 7));
			_scene->_dynamicHotspots[tmpIdx]._articleNumber = PREP_ON;
			_scene->_dynamicHotspots.setPosition(tmpIdx, Common::Point(195, 139), FACING_NONE);
		} else if ((_globals[kJacquesStatus] >= 1) && (_globals[kCurrentYear] == 1881)) {
			_globals._sequenceIndexes[6] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[6], false, 1);
			_scene->_sequences.setDepth(_globals._sequenceIndexes[6], 3);
			if (_game._objects.isInRoom(OBJ_KEY)) {
				_globals._sequenceIndexes[5] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[5], false, 1);
				_scene->_sequences.setDepth(_globals._sequenceIndexes[5], 14);
				_scene->_hotspots.activate(NOUN_KEY, true);
			}
			_scene->_hotspots.activate(NOUN_JACQUES, true);
			_scene->_dynamicHotspots.remove(_hotspotRightFloor2);

			int tmpIdx = _scene->_dynamicHotspots.add(NOUN_JACQUES, VERB_WALKTO, SYNTAX_SINGULAR_MASC, -1, Common::Rect(114, 132, 30, 10));
			_scene->_dynamicHotspots[tmpIdx]._articleNumber = PREP_ON;
			_scene->_dynamicHotspots.setPosition(tmpIdx, Common::Point(95, 144), FACING_NORTHEAST);

			tmpIdx = _scene->_dynamicHotspots.add(NOUN_FLOOR, VERB_WALK_ACROSS, SYNTAX_SINGULAR, -1, Common::Rect(94, 129, 18, 4));
			_scene->_dynamicHotspots[tmpIdx]._articleNumber = PREP_ON;
			_scene->_dynamicHotspots.setPosition(tmpIdx, Common::Point(95, 133), FACING_NONE);

			tmpIdx = _scene->_dynamicHotspots.add(NOUN_FLOOR, VERB_WALK_ACROSS, SYNTAX_SINGULAR, -1, Common::Rect(94, 132, 3, 9));
			_scene->_dynamicHotspots[tmpIdx]._articleNumber = PREP_ON;
			_scene->_dynamicHotspots.setPosition(tmpIdx, Common::Point(93, 135), FACING_NONE);

			tmpIdx = _scene->_dynamicHotspots.add(NOUN_FLOOR, VERB_WALK_ACROSS, SYNTAX_SINGULAR, -1, Common::Rect(112, 150, 21, 3));
			_scene->_dynamicHotspots[tmpIdx]._articleNumber = PREP_ON;
			_scene->_dynamicHotspots.setPosition(tmpIdx, Common::Point(118, 154), FACING_NONE);

			tmpIdx = _scene->_dynamicHotspots.add(NOUN_FLOOR, VERB_WALK_ACROSS, SYNTAX_SINGULAR, -1, Common::Rect(98, 146, 21, 4));
			_scene->_dynamicHotspots[tmpIdx]._articleNumber = PREP_ON;
			_scene->_dynamicHotspots.setPosition(tmpIdx, Common::Point(104, 148), FACING_NONE);
		}
	} else if (_globals[kPrompterStandStatus] == 0) {
		_globals._sequenceIndexes[3] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[3], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[3], 4);
		_scene->_sequences.setPosition(_globals._sequenceIndexes[3], Common::Point(37, 139));

		_hotspotPrompt1 = _scene->_dynamicHotspots.add(NOUN_PROMPTERS_STAND, VERB_WALKTO, SYNTAX_SINGULAR, -1, Common::Rect(2, 79, 40, 63));
		_scene->_dynamicHotspots[_hotspotPrompt1]._articleNumber = PREP_ON;
		_scene->_dynamicHotspots.setPosition(_hotspotPrompt1, Common::Point(59, 140), FACING_NORTHWEST);

		_hotspotPrompt2 = _scene->_dynamicHotspots.add(NOUN_PROMPTERS_STAND, VERB_WALKTO, SYNTAX_SINGULAR, -1, Common::Rect(42, 67, 16, 75));
		_scene->_dynamicHotspots[_hotspotPrompt2]._articleNumber = PREP_ON;
		_scene->_dynamicHotspots.setPosition(_hotspotPrompt2, Common::Point(59, 140), FACING_NORTHWEST);

		_hotspotPrompt3 = _scene->_dynamicHotspots.add(NOUN_PROMPTERS_STAND, VERB_WALKTO, SYNTAX_SINGULAR, -1, Common::Rect(58, 90, 18, 52));
		_scene->_dynamicHotspots[_hotspotPrompt3]._articleNumber = PREP_ON;
		_scene->_dynamicHotspots.setPosition(_hotspotPrompt3, Common::Point(59, 140), FACING_NORTHWEST);

		_hotspotPrompt5 = _scene->_dynamicHotspots.add(NOUN_PROMPTERS_STAND, NOUN_CLIMB, SYNTAX_SINGULAR, -1, Common::Rect(2, 49, 40, 30));
		_scene->_dynamicHotspots[_hotspotPrompt5]._articleNumber = PREP_ON;
		_scene->_dynamicHotspots.setPosition(_hotspotPrompt5, Common::Point(79, 132), FACING_SOUTHWEST);
		_scene->_dynamicHotspots.setCursor(_hotspotPrompt5, CURSOR_GO_UP);

		_hotspotLeftFloor1 = _scene->_dynamicHotspots.add(NOUN_FLOOR, VERB_WALK_ACROSS, SYNTAX_SINGULAR, -1, Common::Rect(35, 137, 40, 5));
		_scene->_dynamicHotspots[_hotspotLeftFloor1]._articleNumber = PREP_ON;
		_scene->_dynamicHotspots.setPosition(_hotspotLeftFloor1, Common::Point(59, 140), FACING_NONE);
		_hotspotLeftFloor2 = _scene->_dynamicHotspots.add(NOUN_FLOOR, VERB_WALK_ACROSS, SYNTAX_SINGULAR, -1, Common::Rect(76, 129, 6, 6));
		_scene->_dynamicHotspots[_hotspotLeftFloor2]._articleNumber = PREP_ON;
		_scene->_dynamicHotspots.setPosition(_hotspotLeftFloor2, Common::Point(80, 135), FACING_NONE);
	}

	if ((_globals[kJacquesStatus] == 0) && (_globals[kCurrentYear] == 1881)) {
		_globals._animationIndexes[0] = _scene->loadAnimation(formAnimName('j', 1), 1);
		_anim0ActvFl = true;
		_scene->setAnimFrame(_globals._animationIndexes[0], 9);
	}

	if (_scene->_priorSceneId == 104) {
		_game._player._stepEnabled = false;
		_game._player._visible = false;
		if (_globals[kRoom103104Transition] == 0) {
			_globals[kRoom103104Transition] = 1;
			_globals._sequenceIndexes[10] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[10], false, -2);
			_scene->_sequences.setDepth(_globals._sequenceIndexes[10], 4);
			_standPosition = 1;
			_game._player._playerPos = Common::Point(79, 132);
			_scene->_sequences.setTimingTrigger(1, 100);
		} else {
			_standPosition = 2;
			_globals._animationIndexes[3] = _scene->loadAnimation(formAnimName('w', 1), 0);
			_anim3ActvFl = true;
			_game._player._stepEnabled = true;
			_game._player._playerPos = Common::Point(196, 134);
			_scene->setAnimFrame(_globals._animationIndexes[3], 36);

			_scene->deleteSequence(_globals._sequenceIndexes[3]);
			_globals._sequenceIndexes[3] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[3], false, 1);
			_scene->_sequences.setDepth(_globals._sequenceIndexes[3], 13);
			_scene->_sequences.setPosition(_globals._sequenceIndexes[3], Common::Point(154, 139));
		}

		_globals._sequenceIndexes[1] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[1], false, -1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[1], 14);

	} else if (_scene->_priorSceneId == 102) {
		_game._player.firstWalk(Common::Point(-20, 140), FACING_EAST, Common::Point(15, 147), FACING_EAST, true);
		_globals._sequenceIndexes[1] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[1], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[1], 14);

	} else if ((_scene->_priorSceneId == 105) || (_scene->_priorSceneId != RETURNING_FROM_LOADING)) {
		_game._player._playerPos = Common::Point(287, 135);
		_game._player._facing = FACING_WEST;
		_game._player._stepEnabled = false;
		_game._player.walk(Common::Point(252, 134), FACING_WEST);
		_game._player.setWalkTrigger(65);
		_globals._sequenceIndexes[1] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[1], false, 6);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[1], 14);

	} else if (_scene->_priorSceneId == -1) {
		if (_standPosition == 1) {
			_game._player._visible = false;
			_globals._animationIndexes[5] = _scene->loadAnimation(formAnimName('w', 3), 0);
			_anim5ActvFl = true;
			_scene->setAnimFrame(_globals._animationIndexes[5], 33);

		} else if (_standPosition == 2) {

			_scene->deleteSequence(_globals._sequenceIndexes[3]);
			_globals._sequenceIndexes[3] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[3], false, 1);
			_scene->_sequences.setDepth(_globals._sequenceIndexes[3], 13);
			_scene->_sequences.setPosition(_globals._sequenceIndexes[3], Common::Point(154, 139));
			_game._player._visible = false;
			_globals._animationIndexes[3] = _scene->loadAnimation(formAnimName('w', 1), 0);
			_anim3ActvFl = true;
			_game._player._stepEnabled = true;
			_scene->setAnimFrame(_globals._animationIndexes[3], 36);

		} else if (_vm->_gameConv->_restoreRunning == 12) {
			_vm->_gameConv->run(12);
			_vm->_gameConv->exportPointer(&_globals[kPlayerScore]);
			_vm->_gameConv->exportValue(_globals[kMusicSelected]);
			_globals[kWalkerConverse] = _vm->getRandomNumber(1, 4);
		}
		_globals._sequenceIndexes[1] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[1], false, -1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[1], 14);
	}

	sceneEntrySound();
}

void Scene103::step() {
	if (_anim0ActvFl)
		handleJacquesAnim();

	if (_anim3ActvFl)
		climbRightStairs();

	if (_anim5ActvFl)
		climbLeftStairs();

	if (_anim4ActvFl)
		descendRightStairs();

	if (_anim6ActvFl)
		descendLeftStairs();

	if ((_globals[kWalkerConverse] == 2) || (_globals[kWalkerConverse] == 3)) {
		++_convCount;
		if (_convCount > 200)
			_globals[kWalkerConverse] = _vm->getRandomNumber(1, 4);
	}

	switch (_game._trigger) {
	case 65:
		_scene->deleteSequence(_globals._sequenceIndexes[1]);
		_globals._sequenceIndexes[1] = _scene->_sequences.addReverseSpriteCycle(_globals._spriteIndexes[1], false, 8, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[1], 14);
		_scene->_sequences.setRange(_globals._sequenceIndexes[1], 1, 6);
		_scene->_sequences.setTrigger(_globals._sequenceIndexes[1], 0, 0, 66);
		_vm->_sound->command(66);
		break;

	case 66: {
		int syncIdx = _globals._sequenceIndexes[1];
		_vm->_sound->command(25);
		_globals._sequenceIndexes[1] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[1], false, 1);
		_game.syncTimers(1, _globals._sequenceIndexes[1], 1, syncIdx);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[1], 14);
		_game._player._stepEnabled = true;
		}
		break;

	case 80:
		_scene->_nextSceneId = 104;
		_scene->_reloadSceneFlag = true;
		break;

	default:
		break;
	}

	switch (_game._trigger) {
	case 100:
		_scene->deleteSequence(_globals._sequenceIndexes[10]);
		_globals._sequenceIndexes[10] = _scene->_sequences.addReverseSpriteCycle(_globals._spriteIndexes[10], false, 6, 1);
		_scene->_sequences.setTrigger(_globals._sequenceIndexes[10], 0, 0, 101);
		_scene->_sequences.setRange(_globals._sequenceIndexes[10], -1, -2);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[10], 4);
		break;

	case 101:
		_globals._animationIndexes[5] = _scene->loadAnimation(formAnimName('w', 3), 0);
		_anim5ActvFl = true;
		_scene->setAnimFrame(_globals._animationIndexes[5], 33);
		_game._player._stepEnabled = true;
		_game.syncTimers(3, _globals._animationIndexes[5], 1, _globals._sequenceIndexes[10]);
		break;

	default:
		break;
	}

	switch (_game._trigger) {
	case 0:
		if (_anim1ActvFl) {
			if ((_scene->getAnimFrame(_globals._animationIndexes[1]) == 10) && !_guardFrameFl) {
				_vm->_sound->command(64);
				_guardFrameFl = true;
				_scene->deleteSequence(_globals._sequenceIndexes[0]);
				_scene->deleteSequence(_globals._sequenceIndexes[9]);
				_globals._sequenceIndexes[0] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[0], false, 6, 1);
				_scene->_sequences.setDepth(_globals._sequenceIndexes[0], 14);
				_scene->_sequences.setRange(_globals._sequenceIndexes[0], -1, -2);
				_scene->_sequences.setTrigger(_globals._sequenceIndexes[0], 0, 0, 105);
			}
		} else if (_anim2ActvFl) {
			if ((_scene->getAnimFrame(_globals._animationIndexes[2]) == 7) && !_guardFrameFl) {
				_vm->_sound->command(64);
				_guardFrameFl = true;
				_scene->deleteSequence(_globals._sequenceIndexes[0]);
				_scene->deleteSequence(_globals._sequenceIndexes[9]);
				_globals._sequenceIndexes[0] = _scene->_sequences.addReverseSpriteCycle(_globals._spriteIndexes[0], false, 6, 1);
				_scene->_sequences.setDepth(_globals._sequenceIndexes[0], 14);
				_scene->_sequences.setRange(_globals._sequenceIndexes[0], -1, -2);
				_scene->_sequences.setTrigger(_globals._sequenceIndexes[0], 0, 0, 110);
			}
		}
		break;

	case 105:
		_globals._sequenceIndexes[0] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[0], false, -2);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[0], 14);
		_globals._sequenceIndexes[9] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[9], false, -2);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[9], 2);
		_guardFrameFl = false;
		break;

	case 110:
		_globals._sequenceIndexes[0] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[0], false, -1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[0], 14);
		_globals._sequenceIndexes[9] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[9], false, -1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[9], 2);
		_guardFrameFl = false;
		break;

	default:
		break;
	}

	if (_globals[kJacquesStatus] == 3) {
		warning("TODO: add a check on the return value of _vm->_sound->command ???");
		_vm->_sound->command(38);
	}
}

void Scene103::preActions() {
	if (_action.isAction(VERB_OPEN, NOUN_DOOR))
		_game._player._needToWalk = true;

	if (_action.isAction(VERB_OPEN, NOUN_DOOR_TO_PIT)) {
		_game._player.walk(Common::Point(0, 150), FACING_WEST);
		_game._player._walkOffScreenSceneId = 102;
	}

	if ((_standPosition != 0) && !_action.isAction(VERB_CLIMB_THROUGH, NOUN_TRAP_DOOR) &&
		!_action.isAction(VERB_OPEN, NOUN_TRAP_DOOR) &&
		!_action.isAction(NOUN_CLIMB, NOUN_PROMPTERS_STAND) &&
		!_action.isAction(VERB_PUSH, NOUN_TRAP_DOOR) &&
		!_action.isAction(VERB_LOOK_THROUGH, NOUN_PROMPTERS_BOX) &&
		!_action.isAction(VERB_PULL, NOUN_TRAP_DOOR)) {

		if (_action.isAction(VERB_PULL) || _action.isAction(VERB_PUSH)) {
			if (!_action.isAction(NOUN_LEVER) && !_game._trigger)
				_game._player._needToWalk = false;
		}

		if ((_action.isAction(VERB_PUSH, NOUN_PROMPTERS_STAND)) || (_action.isAction(VERB_PULL, NOUN_PROMPTERS_STAND))) {
			if (!_game._trigger)
				_game._player._needToWalk = true;
		}

		if (_game._player._needToWalk) {
			if (_globals[kPrompterStandStatus] == 0) {
				switch (_game._trigger) {
				case 0:
					_scene->freeAnimation(5);
					_game._player._readyToWalk = false;
					_game._player._visible = false;
					_game._player._stepEnabled = false;
					_anim5ActvFl = false;
					_anim6ActvFl = true;
					_globals._animationIndexes[6] = _scene->loadAnimation(formAnimName('w', 4), 1);
					break;

				case 1:
					_game._player._playerPos = Common::Point(79, 132);
					_game._player._stepEnabled = true;
					_game._player._visible = true;
					_standPosition = 0;
					_game._player._readyToWalk = true;
					if (_action.isAction(VERB_PUSH, NOUN_PROMPTERS_STAND))
						_game._player._needToWalk = false;
					_anim6ActvFl = false;
					_game._player.resetFacing(FACING_NORTHEAST);
					_game.syncTimers(2, 0, 3, _globals._animationIndexes[6]);
					break;

				default:
					break;
				}
			} else {
				switch (_game._trigger) {
				case 0:
					_scene->freeAnimation(3);
					_game._player._readyToWalk = false;
					if (_action.isAction(VERB_PUSH, NOUN_PROMPTERS_STAND))
						_game._player._needToWalk = true;
					_game._player._visible = false;
					_game._player._stepEnabled = false;
					_anim3ActvFl = false;
					_anim4ActvFl = true;
					_globals._animationIndexes[4] = _scene->loadAnimation(formAnimName('w', 2), 1);
					break;

				case 1:
					_game._player._playerPos = Common::Point(196, 134);
					_game._player._stepEnabled = true;
					_game._player._readyToWalk = true;
					if (_action.isAction(VERB_PUSH, NOUN_PROMPTERS_STAND))
						_game._player._needToWalk = false;
					_game._player._visible = true;
					_standPosition = 0;
					_anim4ActvFl = false;
					_game._player.resetFacing(FACING_NORTHEAST);
					_game.syncTimers(2, 0, 3, _globals._animationIndexes[4]);
					break;

				default:
					break;
				}
			}
		}
	}

	if (_standPosition == 0) {
		if ((_action.isAction(VERB_PUSH, NOUN_PROMPTERS_STAND)) || (_action.isAction(VERB_PULL, NOUN_PROMPTERS_STAND))) {
			if (_globals[kCurrentYear] == 1993) {
				if (_globals[kPrompterStandStatus] == 0)
					_game._player.walk(Common::Point(2, 138), FACING_WEST);
				else
					_game._player.walk(Common::Point(176, 142), FACING_WEST);
			}
		}
	}

	if (_action.isAction(VERB_LOOK_THROUGH, NOUN_PROMPTERS_BOX) && (_standPosition == 0) && (_globals[kPrompterStandStatus] == 0) && (_globals[kCurrentYear] == 1993))
		_game._player.walk(Common::Point(79, 132), FACING_SOUTHWEST);

	if (_action.isAction(VERB_CLIMB_THROUGH, NOUN_TRAP_DOOR) && (_standPosition == 0) && (_globals[kPrompterStandStatus] == 1) && (_globals[kTrapDoorStatus] == 0))
		_game._player.walk(Common::Point(196, 134), FACING_SOUTHWEST);

	if (_action.isAction(VERB_WALK_THROUGH, NOUN_DOOR_TO_PIT))
		_game._player._walkOffScreenSceneId = 102;

	if ((_action.isAction(VERB_OPEN, NOUN_DOOR) || _action.isAction(VERB_UNLOCK, NOUN_DOOR) || _action.isAction(NOUN_LOCK, NOUN_DOOR)) && (_standPosition == 0))
		_game._player.walk(Common::Point(252, 134), FACING_EAST);
}

void Scene103::actions() {
	if (_action.isAction(VERB_OPEN, NOUN_DOOR_TO_PIT)) {
		_scene->_nextSceneId = 102;
		_action._inProgress = false;
		return;
	}

	if (_vm->_gameConv->_running == 12) {
		process_conv_jacques();
		_action._inProgress = false;
		return;
	}

	if (_action.isAction(VERB_CLIMB_THROUGH, NOUN_TRAP_DOOR)) {
		if (_globals[kTrapDoorStatus] == 1) {
			_vm->_dialogs->show(10333);
			_action._inProgress = false;
			return;
		} else if (_globals[kPrompterStandStatus] == 0) {
			_vm->_dialogs->show(10341);
			_action._inProgress = false;
			return;
		}
	}

	if ((_action.isAction(VERB_LOOK_THROUGH, NOUN_PROMPTERS_BOX)) && (_globals[kPrompterStandStatus] == 1)) {
		_vm->_dialogs->show(10342);
		_action._inProgress = false;
		return;
	}

	if ((_action.isAction(NOUN_CLIMB, NOUN_PROMPTERS_STAND) && _standPosition == 0) ||
		(_action.isAction(VERB_LOOK_THROUGH, NOUN_PROMPTERS_BOX) && (_standPosition == 0)) ||
		(_action.isAction(VERB_CLIMB_THROUGH, NOUN_TRAP_DOOR) && (_standPosition == 0))) {
		if (_globals[kPrompterStandStatus] == 0) {
			if (_game._trigger == 0) {
				if (_action.isAction(VERB_LOOK_THROUGH, NOUN_PROMPTERS_BOX)) {
					_sitFl = true;
					_globals._animationIndexes[5] = _scene->loadAnimation(formAnimName('w', 3), 115);
				} else
					_globals._animationIndexes[5] = _scene->loadAnimation(formAnimName('w', 3), 0);

				_game._player._visible = false;
				_game._player._stepEnabled = false;
				_anim5ActvFl = true;
				_game.syncTimers(3, _globals._animationIndexes[5], 2, 0);
				_action._inProgress = false;
				return;
			}
		} else if (_game._trigger == 0) {
			if (_action.isAction(VERB_CLIMB_THROUGH, NOUN_TRAP_DOOR)) {
				_climbThroughTrapFl = true;
				_globals._animationIndexes[3] = _scene->loadAnimation(formAnimName('w', 1), 120);
			} else
				_globals._animationIndexes[3] = _scene->loadAnimation(formAnimName('w', 1), 0);

			_game._player._visible = false;
			_game._player._stepEnabled = false;
			_anim3ActvFl = true;
			_game.syncTimers(3, _globals._animationIndexes[3], 2, 0);
			_action._inProgress = false;
			return;
		}
	}

	if (_action.isAction(NOUN_CLIMB, NOUN_PROMPTERS_STAND)) {
		if (_standPosition != 0) {
			_action._inProgress = false;
			return;
		}
	}

	if ((_action.isAction(VERB_PUSH, NOUN_PROMPTERS_STAND)) || (_action.isAction(VERB_PULL, NOUN_PROMPTERS_STAND))) {
		if (_globals[kCurrentYear] == 1993) {
			if (_globals[kPrompterStandStatus] == 0) {
				switch (_game._trigger) {
				case 0:
					if (_globals[kPrompterStandStatus] == 0) {
						_game._player._stepEnabled = false;
						_game._player._visible = false;
						_globals._animationIndexes[0] = _scene->loadAnimation(formAnimName('s', 1), 75);
						_game._player._priorTimer = _scene->_frameStartTime;
						_game.syncTimers(3, _globals._animationIndexes[0], 2, 0);
						_scene->deleteSequence(_globals._sequenceIndexes[3]);
					}
					break;

				case 75:
					if (_globals[kPrompterStandStatus] == 0) {
						_globals._sequenceIndexes[12] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[12], false, 1);
						_scene->_sequences.setDepth(_globals._sequenceIndexes[12], 1);
						_globals._sequenceIndexes[3] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[3], false, 1);
						_scene->_sequences.setDepth(_globals._sequenceIndexes[3], 4);
						_scene->_sequences.setPosition(_globals._sequenceIndexes[3], Common::Point(154, 139));
						_game.syncTimers(1, _globals._sequenceIndexes[3], 3, _globals._animationIndexes[0]);
						_globals[kPrompterStandStatus] = 1;
						_game._player._stepEnabled = true;
						_game._player._visible = true;
						_game._player._playerPos = Common::Point(117, 139);
						_game._player.resetFacing(FACING_EAST);
						_game.syncTimers(2, 0, 3, _globals._animationIndexes[0]);

						_scene->_dynamicHotspots.remove(_hotspotPrompt1);
						_scene->_dynamicHotspots.remove(_hotspotPrompt2);
						_scene->_dynamicHotspots.remove(_hotspotPrompt3);
						_scene->_dynamicHotspots.remove(_hotspotPrompt5);
						_scene->_dynamicHotspots.remove(_hotspotLeftFloor1);
						_scene->_dynamicHotspots.remove(_hotspotLeftFloor2);

						_hotspotPrompt1 = _scene->_dynamicHotspots.add(NOUN_PROMPTERS_STAND, VERB_WALKTO, SYNTAX_SINGULAR, -1, Common::Rect(121, 79, 40, 63));
						_scene->_dynamicHotspots[_hotspotPrompt1]._articleNumber = PREP_ON;
						_scene->_dynamicHotspots.setPosition(_hotspotPrompt1, Common::Point(171, 142), FACING_NORTHWEST);

						_hotspotPrompt2 = _scene->_dynamicHotspots.add(NOUN_PROMPTERS_STAND, VERB_WALKTO, SYNTAX_SINGULAR, -1, Common::Rect(161, 67, 16, 75));
						_scene->_dynamicHotspots[_hotspotPrompt2]._articleNumber = PREP_ON;
						_scene->_dynamicHotspots.setPosition(_hotspotPrompt2, Common::Point(171, 142), FACING_NORTHWEST);

						_hotspotPrompt3 = _scene->_dynamicHotspots.add(NOUN_PROMPTERS_STAND, VERB_WALKTO, SYNTAX_SINGULAR, -1, Common::Rect(177, 90, 18, 52));
						_scene->_dynamicHotspots[_hotspotPrompt3]._articleNumber = PREP_ON;
						_scene->_dynamicHotspots.setPosition(_hotspotPrompt3, Common::Point(171, 142), FACING_NORTHWEST);

						_hotspotPrompt4 = _scene->_dynamicHotspots.add(NOUN_PROMPTERS_STAND, VERB_WALKTO, SYNTAX_SINGULAR, -1, Common::Rect(114, 100, 7, 38));
						_scene->_dynamicHotspots[_hotspotPrompt4]._articleNumber = PREP_ON;
						_scene->_dynamicHotspots.setPosition(_hotspotPrompt4, Common::Point(171, 142), FACING_NORTHWEST);

						_hotspotPrompt5 = _scene->_dynamicHotspots.add(NOUN_PROMPTERS_STAND, NOUN_CLIMB, SYNTAX_SINGULAR, -1, Common::Rect(121, 49, 40, 30));
						_scene->_dynamicHotspots[_hotspotPrompt5]._articleNumber = PREP_ON;
						_scene->_dynamicHotspots.setPosition(_hotspotPrompt5, Common::Point(196, 134), FACING_SOUTHWEST);
						_scene->_dynamicHotspots.setCursor(_hotspotPrompt5, CURSOR_GO_UP);

						_hotspotRightFloor1 = _scene->_dynamicHotspots.add(NOUN_FLOOR, VERB_WALK_ACROSS, SYNTAX_SINGULAR, -1, Common::Rect(154, 6, 41, 6));
						_scene->_dynamicHotspots[_hotspotRightFloor1]._articleNumber = PREP_ON;
						_scene->_dynamicHotspots.setPosition(_hotspotRightFloor1, Common::Point(171, 142), FACING_NONE);

						_hotspotRightFloor2 = _scene->_dynamicHotspots.add(NOUN_FLOOR, VERB_WALK_ACROSS, SYNTAX_SINGULAR, -1, Common::Rect(114, 136, 32, 6));
						_scene->_dynamicHotspots[_hotspotRightFloor2]._articleNumber = PREP_ON;
						_scene->_dynamicHotspots.setPosition(_hotspotRightFloor2, Common::Point(127, 140), FACING_NONE);

						_scene->changeVariant(1);
						adjustRails(1);
					}
					break;

				default:
					break;
				}
			} else {
				switch (_game._trigger) {
				case 0:
					if (_globals[kPrompterStandStatus] == 1) {
						_game._player._stepEnabled = false;
						_game._player._visible = false;
						_globals._animationIndexes[0] = _scene->loadAnimation(formAnimName('s', 2), 75);
						_game._player._priorTimer = _scene->_frameStartTime;
						_game.syncTimers(3, _globals._animationIndexes[0], 2, 0);
						_scene->deleteSequence(_globals._sequenceIndexes[12]);
						_scene->deleteSequence(_globals._sequenceIndexes[3]);
					}
					break;

				case 75:
					if (_globals[kPrompterStandStatus] == 1) {
						_globals._sequenceIndexes[3] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[3], false, 1);
						_scene->_sequences.setDepth(_globals._sequenceIndexes[3], 4);
						_scene->_sequences.setPosition(_globals._sequenceIndexes[3], Common::Point(37, 139));
						_game.syncTimers(1, _globals._sequenceIndexes[3], 3, _globals._animationIndexes[0]);
						_globals[kPrompterStandStatus] = 0;
						_game._player._stepEnabled = true;
						_game._player._visible = true;
						_game._player._playerPos = Common::Point(62, 142);
						_game._player.resetFacing(FACING_WEST);
						_game.syncTimers(2, 0, 3, _globals._animationIndexes[0]);
						_scene->_dynamicHotspots.remove(_hotspotPrompt1);
						_scene->_dynamicHotspots.remove(_hotspotPrompt2);
						_scene->_dynamicHotspots.remove(_hotspotPrompt3);
						_scene->_dynamicHotspots.remove(_hotspotPrompt4);
						_scene->_dynamicHotspots.remove(_hotspotPrompt5);
						_scene->_dynamicHotspots.remove(_hotspotRightFloor1);
						_scene->_dynamicHotspots.remove(_hotspotRightFloor2);

						_hotspotPrompt1 = _scene->_dynamicHotspots.add(NOUN_PROMPTERS_STAND, VERB_WALKTO, SYNTAX_SINGULAR, -1, Common::Rect(2, 79, 40, 63));
						_scene->_dynamicHotspots[_hotspotPrompt1]._articleNumber = PREP_ON;
						_scene->_dynamicHotspots.setPosition(_hotspotPrompt1, Common::Point(59, 140), FACING_NORTHWEST);

						_hotspotPrompt2 = _scene->_dynamicHotspots.add(NOUN_PROMPTERS_STAND, VERB_WALKTO, SYNTAX_SINGULAR, -1, Common::Rect(42, 67, 16, 75));
						_scene->_dynamicHotspots[_hotspotPrompt2]._articleNumber = PREP_ON;
						_scene->_dynamicHotspots.setPosition(_hotspotPrompt2, Common::Point(59, 140), FACING_NORTHWEST);

						_hotspotPrompt3 = _scene->_dynamicHotspots.add(NOUN_PROMPTERS_STAND, VERB_WALKTO, SYNTAX_SINGULAR, -1, Common::Rect(58, 90, 18, 52));
						_scene->_dynamicHotspots[_hotspotPrompt3]._articleNumber = PREP_ON;
						_scene->_dynamicHotspots.setPosition(_hotspotPrompt3, Common::Point(59, 140), FACING_NORTHWEST);

						_hotspotLeftFloor1 = _scene->_dynamicHotspots.add(NOUN_FLOOR, VERB_WALK_ACROSS, SYNTAX_SINGULAR, -1, Common::Rect(35, 137, 40, 5));
						_scene->_dynamicHotspots[_hotspotLeftFloor1]._articleNumber = PREP_ON;
						_scene->_dynamicHotspots.setPosition(_hotspotLeftFloor1, Common::Point(59, 140), FACING_NONE);

						_hotspotLeftFloor2 = _scene->_dynamicHotspots.add(NOUN_FLOOR, VERB_WALK_ACROSS, SYNTAX_SINGULAR, -1, Common::Rect(76, 129, 6, 6));
						_scene->_dynamicHotspots[_hotspotLeftFloor2]._articleNumber = PREP_ON;
						_scene->_dynamicHotspots.setPosition(_hotspotLeftFloor2, Common::Point(80, 135), FACING_NONE);

						_hotspotPrompt5 = _scene->_dynamicHotspots.add(NOUN_PROMPTERS_STAND, NOUN_CLIMB, SYNTAX_SINGULAR, -1, Common::Rect(2, 49, 40, 30));
						_scene->_dynamicHotspots[_hotspotPrompt5]._articleNumber = PREP_ON;
						_scene->_dynamicHotspots.setPosition(_hotspotPrompt5, Common::Point(79, 132), FACING_SOUTHWEST);
						_scene->_dynamicHotspots.setCursor(_hotspotPrompt5, CURSOR_GO_UP);

						_scene->changeVariant(0);
						adjustRails(0);
					}
					break;

				default:
					break;
				}
			}
		} else {
			if (_globals[kJacquesNameIsKnown])
				_vm->_dialogs->show(10340);
			else
				_vm->_dialogs->show(10350);
		}
		_action._inProgress = false;
		return;
	}

	if (_action.isAction(VERB_WALK_THROUGH, NOUN_DOOR) || _action.isAction(VERB_OPEN, NOUN_DOOR) || _action.isAction(VERB_UNLOCK, NOUN_DOOR) || _action.isAction(NOUN_LOCK, NOUN_DOOR)) {
		if ((_globals[kCurrentYear] == 1881) && !_action.isAction(NOUN_LOCK, NOUN_DOOR) && !_action.isAction(VERB_UNLOCK, NOUN_DOOR)) {
			switch (_game._trigger) {
			case (0):
				_game._player._stepEnabled = false;
				_game._player._visible = false;
				_globals._sequenceIndexes[2] = _scene->_sequences.startPingPongCycle(_globals._spriteIndexes[2], false, 5, 0, 0, 2);
				_scene->_sequences.setRange(_globals._sequenceIndexes[2], -1, -2);
				_scene->_sequences.setSeqPlayer(_globals._sequenceIndexes[2], true);
				_scene->_sequences.setTrigger(_globals._sequenceIndexes[2], 0, 0, 2);
				_scene->_sequences.setTrigger(_globals._sequenceIndexes[2], 2, 3, 70);
				break;

			case 2:
				_game._player._visible = true;
				_game.syncTimers(2, 0, 1, _globals._sequenceIndexes[2]);
				_game._player.walk(Common::Point(295, 132), FACING_WEST);
				_scene->_sequences.setTimingTrigger(180, 3);
				break;

			case 3:
				_scene->_nextSceneId = 105;
				break;

			case 70:
				_vm->_sound->command(24);
				_scene->deleteSequence(_globals._sequenceIndexes[1]);
				_globals._sequenceIndexes[1] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[1], false, 8, 1);
				_scene->_sequences.setDepth(_globals._sequenceIndexes[1], 14);
				_scene->_sequences.setRange(_globals._sequenceIndexes[1], -1, -2);
				_scene->_sequences.setTrigger(_globals._sequenceIndexes[1], 0, 0, 71);
				_vm->_sound->command(66);
				break;

			case 71: {
				int oldIdx = _globals._sequenceIndexes[1];
				_globals._sequenceIndexes[1] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[1], false, -2);
				_game.syncTimers(1, _globals._sequenceIndexes[1], 1, oldIdx);
				_scene->_sequences.setDepth(_globals._sequenceIndexes[1], 14);
				}
				break;

			default:
				break;
			}
		} else {
			switch (_game._trigger) {
			case (0):
				_game._player._stepEnabled = false;
				_game._player._visible = false;
				_globals._sequenceIndexes[2] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[2], false, 5, 1);
				_scene->_sequences.setRange(_globals._sequenceIndexes[2], 1, 4);
				_scene->_sequences.setSeqPlayer(_globals._sequenceIndexes[2], true);
				_scene->_sequences.setTrigger(_globals._sequenceIndexes[2], 0, 0, 1);
				break;

			case 1: {
				int oldIdx = _globals._sequenceIndexes[2];
				_globals._sequenceIndexes[2] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[2], false, 4);
				_game.syncTimers(1, _globals._sequenceIndexes[2], 1, oldIdx);
				_scene->_sequences.setSeqPlayer(_globals._sequenceIndexes[2], false);
				_scene->_sequences.setTimingTrigger(15, 2);
				_vm->_sound->command(73);
				}
				break;

			case 2:
				_scene->deleteSequence(_globals._sequenceIndexes[2]);
				_globals._sequenceIndexes[2] = _scene->_sequences.addReverseSpriteCycle(_globals._spriteIndexes[2], false, 5, 1);
				_scene->_sequences.setRange(_globals._sequenceIndexes[2], 1, 4);
				_scene->_sequences.setSeqPlayer(_globals._sequenceIndexes[2], false);
				_scene->_sequences.setTrigger(_globals._sequenceIndexes[2], 0, 0, 3);
				break;

			case 3:
				_game.syncTimers(2, 0, 1, _globals._sequenceIndexes[2]);
				_game._player._visible = true;
				if (_action.isAction(NOUN_LOCK) || _action.isAction(VERB_UNLOCK))
					_vm->_dialogs->show(00032);
				else
					_vm->_dialogs->show(10335);

				_game._player._stepEnabled = true;
				break;

			default:
				break;
			}
		}
		_action._inProgress = false;
		return;
	}

	if (_action.isAction(VERB_TAKE, NOUN_KEY) && (_game._objects.isInRoom(OBJ_KEY) || _game._trigger)) {
		switch (_game._trigger) {
		case (0):
			_game._player._stepEnabled = false;
			_game._player._visible = false;
			_globals._sequenceIndexes[8] = _scene->_sequences.startPingPongCycle(_globals._spriteIndexes[8], false, 5, 0, 0, 2);
			_scene->_sequences.setRange(_globals._sequenceIndexes[8], 1, 5);
			_scene->_sequences.setSeqPlayer(_globals._sequenceIndexes[8], true);
			_scene->_sequences.setTrigger(_globals._sequenceIndexes[8], 2, 5, 1);
			_scene->_sequences.setTrigger(_globals._sequenceIndexes[8], 0, 0, 2);
			_action._inProgress = false;
			return;
			break;

		case 1:
			_scene->deleteSequence(_globals._sequenceIndexes[5]);
			_scene->_hotspots.activate(NOUN_KEY, false);
			_game._objects.addToInventory(OBJ_KEY);
			_vm->_sound->command(26);
			_action._inProgress = false;
			return;
			break;

		case 2:
			_game.syncTimers(2, 0, 1, _globals._sequenceIndexes[8]);
			_game._player._visible = true;
			_scene->_sequences.setTimingTrigger(20, 3);
			_action._inProgress = false;
			return;
			break;

		case 3:
			_globals[kPlayerScore] += 5;
			_vm->_dialogs->showItem(OBJ_KEY, 800, 0);
			_game._player._stepEnabled = true;
			_action._inProgress = false;
			return;
			break;

		default:
			break;
		}
	}

	if (_action.isAction(VERB_TALK_TO, NOUN_GENTLEMAN) || _action.isAction(VERB_TALK_TO, NOUN_JACQUES)) {
		if (_globals[kJacquesStatus] == 0) {
			_vm->_gameConv->run(12);
			_vm->_gameConv->exportPointer(&_globals[kPlayerScore]);
			_vm->_gameConv->exportValue(_globals[kMusicSelected]);
		} else
			_vm->_dialogs->show(10343);

		_action._inProgress = false;
		return;
	}

	if (_action.isAction(VERB_CLIMB_THROUGH, NOUN_TRAP_DOOR) || _climbThroughTrapFl) {
		if ((_standPosition == 2) && (_globals[kTrapDoorStatus] == 0))  {
			switch (_game._trigger) {
			case 0:
			case 120:
				if (!(_globals[kPlayerScoreFlags] & 1)) {
					_globals[kPlayerScoreFlags] |= 1;
					_globals[kPlayerScore] += 3;
				}

				_scene->freeAnimation(3);
				_anim3ActvFl = false;
				_climbThroughTrapFl = false;
				_game._player._stepEnabled = false;
				_globals._sequenceIndexes[11] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[11], false, 6, 1);
				_scene->_sequences.setDepth(_globals._sequenceIndexes[11], 5);
				_scene->_sequences.setRange(_globals._sequenceIndexes[11], -1, -2);
				_scene->_sequences.setTrigger(_globals._sequenceIndexes[11], 0, 0, 1);
				break;

			case 1:
				_globals[kRoom103104Transition] = 1;
				_scene->_nextSceneId = 104;
				break;

			default:
				break;
			}
		}
		_action._inProgress = false;
		return;
	}

	if (_action.isAction(VERB_LOOK_THROUGH, NOUN_PROMPTERS_BOX) || _sitFl) {
		if (_standPosition == 1) {
			switch (_game._trigger) {
			case 0:
			case 115:
				_scene->freeAnimation(5);
				_anim5ActvFl = false;
				_game._player._stepEnabled = false;
				_globals._sequenceIndexes[10] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[10], false, 6, 1);
				_scene->_sequences.setDepth(_globals._sequenceIndexes[10], 4);
				_scene->_sequences.setRange(_globals._sequenceIndexes[10], -1, -2);
				_scene->_sequences.setTrigger(_globals._sequenceIndexes[10], 0, 0, 1);
				if (!_sitFl)
					_game.syncTimers(1, _globals._sequenceIndexes[10], 3, _globals._animationIndexes[5]);
				_sitFl = false;
				break;

			case 1:
				_globals[kRoom103104Transition] = 0;
				_scene->_nextSceneId = 104;
				break;

			default:
				break;
			}
		}
		_action._inProgress = false;
		return;
	}

	if (_action.isAction(VERB_PUSH, NOUN_LEVER) || _action.isAction(VERB_PULL, NOUN_LEVER)) {
		if (_globals[kTrapDoorStatus] == 1) {
			switch (_game._trigger) {
			case 0:
				_globals._animationIndexes[1] = _scene->loadAnimation(formAnimName('l', 1), 1);
				_anim1ActvFl   = true;
				_game._player._visible = false;
				_game._player._stepEnabled = false;
				_game.syncTimers(3, _globals._animationIndexes[1], 2, 0);
				break;

			case 1:
				_anim1ActvFl    = false;
				_game._player._visible = true;
				_globals[kTrapDoorStatus] = 0;
				_game._player._stepEnabled = true;
				_game.syncTimers(2, 0, 3, _globals._animationIndexes[1]);
				break;

			default:
				break;
			}
		} else {
			switch (_game._trigger) {
			case 0:
				_globals._animationIndexes[2] = _scene->loadAnimation(formAnimName('l', 2), 1);
				_anim2ActvFl = true;
				_game._player._visible = false;
				_game._player._stepEnabled = false;
				_game.syncTimers(3, _globals._animationIndexes[2], 2, 0);
				break;

			case 1:
				_anim2ActvFl = false;
				_game._player._visible = true;
				_globals[kTrapDoorStatus] = 1;
				_game._player._stepEnabled = true;
				_game.syncTimers(2, 0, 3, _globals._animationIndexes[2]);
				break;

			default:
				break;
			}
		}
		_action._inProgress = false;
		return;
	}

	if (_action._lookFlag) {
		if ((_globals[kCurrentYear] == 1881) && (_globals[kJacquesStatus] >= 1))
			_vm->_dialogs->show(10311);
		else
			_vm->_dialogs->show(10310);
		_action._inProgress = false;
		return;
	}

	if (_action.isAction(VERB_LOOK) || _action.isAction(VERB_LOOK_AT)) {
		if (_action.isAction(NOUN_PROMPTERS_STAND)) {
			if (_globals[kJacquesStatus] >= 1)
				_vm->_dialogs->show(10349);
			else if (_globals[kCurrentYear] == 1993)
				_vm->_dialogs->show(10312);
			else
				_vm->_dialogs->show(10345);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_FLOOR)) {
			_vm->_dialogs->show(10313);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_TRAP_CEILING)) {
			_vm->_dialogs->show(10314);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_DOOR)) {
			_vm->_dialogs->show(10315);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_DOOR_TO_PIT)) {
			_vm->_dialogs->show(10316);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_WALL)) {
			_vm->_dialogs->show(10317);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_PROMPTERS_BOX)) {
			_vm->_dialogs->show(10318);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_TRAP_DOOR)) {
			_vm->_dialogs->show(10319);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_JUNK)) {
			if (_globals[kCurrentYear] == 1993)
				_vm->_dialogs->show(10320);
			else
				_vm->_dialogs->show(10346);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_CARTON)) {
			if (_globals[kCurrentYear] == 1993)
				_vm->_dialogs->show(10321);
			else
				_vm->_dialogs->show(10347);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_GARBAGE_CAN)) {
			if (_globals[kCurrentYear] == 1993)
				_vm->_dialogs->show(10322);
			else
				_vm->_dialogs->show(10348);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_CABLE)) {
			_vm->_dialogs->show(10323);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_JACQUES) || _action.isAction(NOUN_GENTLEMAN)) {
			if (_globals[kJacquesStatus] == 0)
				_vm->_dialogs->show(10324);
			else
				_vm->_dialogs->show(10325);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_KEY)) {
			if (_game._objects.isInRoom(OBJ_KEY)) {
				_vm->_dialogs->show(10326);
				_action._inProgress = false;
				return;
			}
		}

		if (_action.isAction(NOUN_STAIR_UNIT)) {
			_vm->_dialogs->show(10327);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_EXPOSED_BRICK)) {
			_vm->_dialogs->show(10328);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_WATER_PIPE)) {
			_vm->_dialogs->show(10329);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_PROMPTERS_SEAT)) {
			_vm->_dialogs->show(10338);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_LEVER)) {
			_vm->_dialogs->show(10339);
			_action._inProgress = false;
			return;
		}

	}

	if (_action.isAction(VERB_CLOSE, NOUN_DOOR_TO_PIT)) {
		_vm->_dialogs->show(10331);
		_action._inProgress = false;
		return;
	}

	if (_action.isAction(VERB_CLOSE, NOUN_DOOR)) {
		_vm->_dialogs->show(10331);
		_action._inProgress = false;
		return;
	}

	if ((_action.isAction(VERB_OPEN, NOUN_TRAP_DOOR)) || (_action.isAction(VERB_CLOSE, NOUN_TRAP_DOOR))) {
		_vm->_dialogs->show(10344);
		_action._inProgress = false;
		return;
	}

	if (_action.isAction(VERB_TAKE, NOUN_JACQUES) || _action.isAction(VERB_TAKE, NOUN_GENTLEMAN)) {
		if (_globals[kJacquesStatus] == 0) {
			if (_globals[kJacquesNameIsKnown])
				_vm->_dialogs->show(10336);
			else
				_vm->_dialogs->show(10351);
		} else
			_vm->_dialogs->show(10337);
		_action._inProgress = false;
		return;
	}
}

void Scene103::adjustRails(int variant) {
	switch (variant) {
	case 0:
		_scene->_rails.disableNode(3);
		_scene->_rails.disableNode(4);
		_scene->_rails.disableNode(5);
		_scene->_rails.disableNode(6);
		_scene->_rails.disableNode(12);
		_scene->_rails.disableNode(13);
		_scene->_rails.disableNode(14);
		break;

	case 1:
		_scene->_rails.disableNode(1);
		_scene->_rails.disableNode(2);
		_scene->_rails.disableNode(3);
		_scene->_rails.disableNode(4);
		_scene->_rails.disableNode(5);
		_scene->_rails.disableNode(6);
		_scene->_rails.disableNode(7);
		_scene->_rails.disableNode(9);
		_scene->_rails.disableNode(10);
		_scene->_rails.disableNode(11);
		break;

	case 2:
		_scene->_rails.disableNode(1);
		_scene->_rails.disableNode(2);
		_scene->_rails.disableNode(5);
		_scene->_rails.disableNode(6);
		_scene->_rails.disableNode(7);
		_scene->_rails.disableNode(9);
		_scene->_rails.disableNode(10);
		_scene->_rails.disableNode(11);
		break;

	case 3:
		_scene->_rails.disableNode(1);
		_scene->_rails.disableNode(2);
		_scene->_rails.disableNode(3);
		_scene->_rails.disableNode(4);
		_scene->_rails.disableNode(10);
		_scene->_rails.disableNode(11);
		break;

	default:
		break;
	}
}

void Scene103::handleJacquesAnim() {
	if (_scene->getAnimFrame(_globals._animationIndexes[0]) == _lastJacquesFrame)
		return;

	_lastJacquesFrame = _scene->getAnimFrame(_globals._animationIndexes[0]);
	int resetFrame = -1;
	int random;

	switch (_lastJacquesFrame) {
	case 1:
	case 2:
	case 3:
	case 9:
	case 17:
	case 23:
	case 33:
	case 51:
		switch (_jacquesAction) {
		case 2:
			random = 4;
			_jacquesAction = 0;
			break;

		case 3:
			random = 5;
			_jacquesAction = 0;
			break;

		case 4:
			random = 6;
			break;

		case 0:
			random = _vm->getRandomNumber(1, 3);
			++_talkCount;
			if (_talkCount > 22) {
				_jacquesAction = 1;
				random = 9;
			}
			break;

		default:
			random = _vm->getRandomNumber(6, 50);
			while (_lastRandom == random)
				random = _vm->getRandomNumber(6, 50);

			_lastRandom = random;
			break;
		}

		switch (random) {
		case 1:
			resetFrame = 0;
			break;

		case 2:
			resetFrame = 1;
			break;

		case 3:
			resetFrame = 2;
			break;

		case 4:
			resetFrame = 4;
			break;

		case 5:
			resetFrame = 10;
			break;

		case 6:
			resetFrame = 34;
			break;

		case 7:
			resetFrame = 24;
			break;

		case 8:
			resetFrame = 18;
			break;

		default:
			resetFrame = 8;
			break;
		}
		break;

	case 36:
	case 40:
	case 48:
		switch (_jacquesAction) {
		case 0:
		case 2:
		case 3:
			random = 2;
			break;

		case 4:
			random = 1;
			_jacquesAction = 0;
			break;

		default:
			random = _vm->getRandomNumber(2, 50);
			while (_lastRandom == random)
				random = _vm->getRandomNumber(2, 50);
			_lastRandom = random;
			break;
		}

		switch (random) {
		case 1:
			resetFrame = 37;
			break;

		case 2:
			resetFrame = 49;
			break;

		case 3:
			resetFrame = 41;
			break;

		default:
			resetFrame = 35;
			break;
		}
		break;

	case 44:
		random = _vm->getRandomNumber(1, 50);
		while (_lastRandom == random) {
			random = _vm->getRandomNumber(1, 50);
		}
		_lastRandom = random;

		switch (_jacquesAction) {
		case 0:
		case 2:
		case 3:
		case 4:
			random = 1;
			break;

		default:
			random = _vm->getRandomNumber(1, 50);
			while (_lastRandom == random)
				random = _vm->getRandomNumber(1, 50);
			_lastRandom = random;
			break;
		}

		switch (random) {
		case 1:
			resetFrame = 45;
			break;

		default:
			resetFrame = 43;
			break;
		}
		break;
	}

	if (resetFrame >= 0) {
		_scene->setAnimFrame(_globals._animationIndexes[0], resetFrame);
		_lastJacquesFrame = resetFrame;
	}
}

void Scene103::climbRightStairs() {
	if (_scene->getAnimFrame(_globals._animationIndexes[3]) == _lastStairFrame)
		return;

	_lastStairFrame = _scene->getAnimFrame(_globals._animationIndexes[3]);
	int stairs_reset_frame = -1;

	if (_lastStairFrame == 37) {
		stairs_reset_frame = 36;
		_standPosition = 2;
		_game._player._stepEnabled = true;
	}

	if (_lastStairFrame == 2) {
		_scene->deleteSequence(3);
		_globals._sequenceIndexes[3] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[3], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[3], 13);
		_scene->_sequences.setPosition(_globals._sequenceIndexes[3], Common::Point(154, 139));
	}

	if (stairs_reset_frame >= 0) {
		_scene->setAnimFrame(_globals._animationIndexes[3], stairs_reset_frame);
		_lastStairFrame = stairs_reset_frame;
	}
}

void Scene103::climbLeftStairs() {
	if (_scene->getAnimFrame(_globals._animationIndexes[5]) == _lastStairFrame)
		return;

	_lastStairFrame = _scene->getAnimFrame(_globals._animationIndexes[5]);
	int stairs_reset_frame  = -1;

	if (_lastStairFrame == 34) {
		stairs_reset_frame = 33;
		_standPosition = 1;
		_game._player._stepEnabled = true;
	}

	if (_lastStairFrame == 2) {
		_scene->deleteSequence(3);
		_globals._sequenceIndexes[3] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[3], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[3], 13);
		_scene->_sequences.setPosition(_globals._sequenceIndexes[3], Common::Point(37, 139));
	}

	if (stairs_reset_frame >= 0) {
		_scene->setAnimFrame(_globals._animationIndexes[5], stairs_reset_frame);
		_lastStairFrame = stairs_reset_frame;
	}
}

void Scene103::descendRightStairs() {
	if (_scene->getAnimFrame(_globals._animationIndexes[4]) == _lastStairFrame)
		return;

	// CHECKME: use of this local variable
	int stairs_reset_frame = -1;
	_lastStairFrame = _scene->getAnimFrame(_globals._animationIndexes[4]);

	if (_lastStairFrame == 2) {
		_scene->deleteSequence(3);
		_globals._sequenceIndexes[3] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[3], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[3], 4);
		_scene->_sequences.setPosition(_globals._sequenceIndexes[3], Common::Point(154, 139));
	}

	if (stairs_reset_frame >= 0) {
		_scene->setAnimFrame(_globals._animationIndexes[4], stairs_reset_frame);
		_lastStairFrame = stairs_reset_frame;
	}
}

void Scene103::descendLeftStairs() {
	if (_scene->getAnimFrame(_globals._animationIndexes[6]) == _lastStairFrame)
		return;

	// CHECKME: use of this local variable
	int stairs_reset_frame = -1;
	_lastStairFrame = _scene->getAnimFrame(_globals._animationIndexes[6]);

	if (_lastStairFrame == 2) {
		_scene->deleteSequence(3);
		_globals._sequenceIndexes[3] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[3], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[3], 4);
		_scene->_sequences.setPosition(_globals._sequenceIndexes[3], Common::Point(37, 139));
	}

	if (stairs_reset_frame >= 0) {
		_scene->setAnimFrame(_globals._animationIndexes[6], stairs_reset_frame);
		_lastStairFrame = stairs_reset_frame;
	}
}

void Scene103::process_conv_jacques() {
	int quit_converse = false;
	int *value_1;

	switch (_action._activeAction._verbId) {
	case 1:
		_vm->_gameConv->setInterlocutorTrigger(96);
		if (_globals[kJacquesNameIsKnown] == 0) {
			_globals[kJacquesNameIsKnown] = 1;
			_scene->_dynamicHotspots.remove(_hotspotGentleman);
			_hotspotGentleman = _scene->_dynamicHotspots.add(NOUN_JACQUES, VERB_WALKTO, SYNTAX_SINGULAR_MASC, -1, Common::Rect(156, 116, 33, 31));
			_scene->_dynamicHotspots[_hotspotGentleman]._articleNumber = PREP_ON;
			_scene->_dynamicHotspots.setPosition(_hotspotGentleman, Common::Point(206, 148), FACING_NORTHWEST);
		}
		break;

	case 4:
	case 6:
	case 7:
	case 10:
	case 19:
	case 30:
		_globals[kWalkerConverse] = 0;
		value_1 = _vm->_gameConv->getVariable(26);
		if (*value_1)
			_globals[kJacquesNameIsKnown] = 2;

		quit_converse = true;
		break;

	case 8:
		_vm->_gameConv->setInterlocutorTrigger(94);
		break;

	case 12:
		_vm->_gameConv->setInterlocutorTrigger(96);
		break;

	case 29:
		_vm->_gameConv->setInterlocutorTrigger(98);
		break;

	default:
		break;
	}

	if ((_action._activeAction._verbId != 1) && (_action._activeAction._verbId != 8) && (_action._activeAction._verbId != 12) && (_action._activeAction._verbId != 29))
		_vm->_gameConv->setInterlocutorTrigger(90);

	_vm->_gameConv->setHeroTrigger(92);

	switch (_game._trigger) {
	case 90:
		if (!quit_converse)
			_globals[kWalkerConverse] = _vm->getRandomNumber(1, 4);
		_jacquesAction = 0;
		break;

	case 92:
		if (!quit_converse)
			_globals[kWalkerConverse] = _vm->getRandomNumber(2, 3);
		_convCount = 0;
		_jacquesAction = 1;
		break;

	case 94:
		if (!quit_converse)
			_globals[kWalkerConverse] = _vm->getRandomNumber(1, 4);
		_jacquesAction = 2;
		break;

	case 96:
		if (!quit_converse)
			_globals[kWalkerConverse] = _vm->getRandomNumber(1, 4);
		_jacquesAction = 3;
		break;

	case 98:
		if (!quit_converse)
			_globals[kWalkerConverse] = _vm->getRandomNumber(1, 4);
		_jacquesAction = 4;
		break;

	default:
		break;
	}

	_talkCount = 0;
}

/*------------------------------------------------------------------------*/

Scene104::Scene104(MADSEngine *vm) : Scene1xx(vm) {
	_anim0ActvFl = _anim1ActvFl = _anim2ActvFl = false;
	_needToTalk = false;
	_needToGetUp = false;
	_sittingUp = false;
	_beforeHeLeaves = false;
	_beforeSheLeaves = false;
	_needToStandUp = false;

	_walkStatus = -1;
	_walkFrame = -1;
	_coupleStatus = -1;
	_richStatus = -1;
	_richTalkCount = -1;
	_manTalkCount = -1;
	_womanTalkCount = -1;
	_lookCount = -1;
	_coupleFrame = -1;
	_lastPlayerFrame = -1;
	_richFrame = -1;
}

void Scene104::synchronize(Common::Serializer &s) {
	Scene1xx::synchronize(s);

	s.syncAsByte(_anim0ActvFl);
	s.syncAsByte(_anim1ActvFl);
	s.syncAsByte(_anim2ActvFl);
	s.syncAsByte(_needToTalk);
	s.syncAsByte(_needToGetUp);
	s.syncAsByte(_sittingUp);
	s.syncAsByte(_beforeHeLeaves);
	s.syncAsByte(_beforeSheLeaves);
	s.syncAsByte(_needToStandUp);

	s.syncAsSint16LE(_walkStatus);
	s.syncAsSint16LE(_walkFrame);
	s.syncAsSint16LE(_coupleStatus);
	s.syncAsSint16LE(_richStatus);
	s.syncAsSint16LE(_richTalkCount);
	s.syncAsSint16LE(_manTalkCount);
	s.syncAsSint16LE(_womanTalkCount);
	s.syncAsSint16LE(_lookCount);
	s.syncAsSint16LE(_coupleFrame);
	s.syncAsSint16LE(_lastPlayerFrame);
}

void Scene104::setup() {
	setPlayerSpritesPrefix();
	setAAName();

	if (_globals[kTrapDoorStatus] == 1)
		_scene->_initialVariant = 1;

	_scene->addActiveVocab(NOUN_MONSIEUR_RICHARD);
}

void Scene104::enter() {
	_vm->_disableFastwalk = true;

	if (_scene->_priorSceneId != RETURNING_FROM_LOADING) {
		_anim0ActvFl = false;
		_anim1ActvFl = false;
		_anim2ActvFl = false;
		_needToTalk = false;
		_needToGetUp = false;
		_sittingUp = false;
		_beforeSheLeaves = false;
		_needToStandUp = false;
	}

	_globals._spriteIndexes[1] = _scene->_sprites.addSprites(formAnimName('a', 0), false);
	_globals._spriteIndexes[3] = _scene->_sprites.addSprites(formAnimName('a', 6), false);
	_globals._spriteIndexes[0] = _scene->_sprites.addSprites(formAnimName('x', 0), false);

	if (_globals[kCurrentYear] == 1993)
		_globals._spriteIndexes[2] = _scene->_sprites.addSprites(formAnimName('z', 0), false);

	_vm->_gameConv->get(7);

	if (_globals[kTrapDoorStatus] == 1) {
		_globals._sequenceIndexes[0] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[0], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[0], 15);
	} else {
		_globals._sequenceIndexes[0] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[0], false, 2);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[0], 15);
	}

	if (_scene->_priorSceneId == RETURNING_FROM_LOADING) {
		if (_vm->_gameConv->_restoreRunning == 7) {
			_globals._animationIndexes[1] = _scene->loadAnimation(formAnimName('r', 1), 1);
			_globals._animationIndexes[2] = _scene->loadAnimation(formAnimName('d', 1), 1);
			_walkStatus = 0;
			_globals._animationIndexes[0] = _scene->loadAnimation(formAnimName('m', 1), 1);
			if (_coupleStatus < 11) {
				_coupleStatus = 1;
				_richStatus = 0;
				_scene->setAnimFrame(_globals._animationIndexes[1], 14);
			} else {
				_coupleStatus = 17;
				_richStatus = 4;
				_scene->setAnimFrame(_globals._animationIndexes[1], 105);
				_scene->setAnimFrame(_globals._animationIndexes[0], 216);
			}

			_vm->_gameConv->run(7);
			_vm->_gameConv->exportPointer(&_globals[kPlayerScore]);
			_game._player._visible = false;
		}

	} else if (_scene->_priorSceneId == 301) {
		_globals._animationIndexes[1] = _scene->loadAnimation(formAnimName('r', 1), 1);
		_anim1ActvFl = true;
		_coupleStatus = 11;

		_globals._animationIndexes[2] = _scene->loadAnimation(formAnimName('d', 1), 1);
		_anim2ActvFl = true;
		_walkStatus = 0;

		_globals._animationIndexes[0] = _scene->loadAnimation(formAnimName('m', 1), 1);
		_anim0ActvFl = true;
		_richStatus = 0;

		cleanInventory ();

		_game._player._visible = false;
		_game._visitedScenes.pop_back();
		_globals[kTrapDoorStatus] = 1;
		_globals[kCurrentYear] = 1881;
		_globals[kPrompterStandStatus] = 1;
		_globals[kTicketPeoplePresent] = 1;
		_globals[kMakeBrieLeave203] = false;
		_game._player._playerPos.x = 161;

		_game._visitedScenes.add(301);
		_game._visitedScenes.add(101);

		_scene->setCamera(Common::Point(60, 0));
		_scene->_sequences.setTimingTrigger(1, 91);

	} else if (_scene->_priorSceneId == 103) {
		if (_globals[kRoom103104Transition] == 0) {
			_scene->_userInterface.emptyConversationList();
			_scene->_userInterface.setup(kInputConversation);

			if (!_globals[kObservedPhan104]) {
				_globals._animationIndexes[4] = _scene->loadAnimation(formAnimName('p', 1), 93);
				_game._player._playerPos.x = 319;
				_game._player._stepEnabled = false;
				_game._player._visible = false;
				_scene->setCamera(Common::Point(158, 0));
			} else {
				_globals._animationIndexes[5] = _scene->loadAnimation(formAnimName('p', 2), 94);
				_game._player._playerPos.x = 319;
				_game._player._stepEnabled = false;
				_game._player._visible = false;
				_scene->setCamera(Common::Point(158, 0));
			}
		} else {
			_game._player._playerPos = Common::Point(319, 96);
			_game._player._facing = FACING_SOUTH;
			_scene->setCamera(Common::Point(158, 0));
		}
	} else if (_scene->_priorSceneId == 102) {
		switch (_globals[kDeathLocation]) {
		case 0:
			_game._player._playerPos = Common::Point(496, 79);
			_scene->setCamera(Common::Point(320, 0));
			break;

		case 1:
			_game._player._playerPos = Common::Point(346, 71);
			_scene->setCamera(Common::Point(158, 0));
			break;

		case 2:
			_game._player._playerPos = Common::Point(172, 73);
			break;

		default:
			break;
		}
	} else if (_scene->_priorSceneId == 108) {
		if (_game._player._playerPos.x > 213)
			_game._player._playerPos.y = 97;
		else if (_game._player._playerPos.x > 110)
			_game._player._playerPos.y = 128;
		else
			_game._player._playerPos.y = 148;

		_game._player.firstWalk(Common::Point(-20, _game._player._playerPos.y), FACING_EAST, Common::Point(12, _game._player._playerPos.y), FACING_EAST, true);
	} else if ((_scene->_priorSceneId == 107) || (_scene->_priorSceneId != RETURNING_FROM_LOADING)) {
		if (_game._player._playerPos.x > 191)
			_game._player._playerPos.y = 142;
		else if (_game._player._playerPos.x > 104)
			_game._player._playerPos.y = 120;
		else
			_game._player._playerPos.y = 95;

		_game._player.firstWalk(Common::Point(655, _game._player._playerPos.y), FACING_WEST, Common::Point(627, _game._player._playerPos.y), FACING_WEST, true);
		_scene->setCamera(Common::Point(320, 0));
	}

	if (_globals[kCurrentYear] == 1993) {
		_globals._sequenceIndexes[2] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[2], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[2], 14);
	} else
		_scene->_hotspots.activate(NOUN_CHANDELIER, false);

	if (_globals[kTrapDoorStatus] == 1) {
		_globals._sequenceIndexes[0] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[0], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[0], 15);
	} else {
		_globals._sequenceIndexes[0] = _scene->_sequences.addStampCycle(_globals._spriteIndexes[0], false, 2);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[0], 15);
	}

	sceneEntrySound();
}

void Scene104::step() {
	if (_anim0ActvFl)
		handleRichAnimations();

	if (_anim1ActvFl)
		handleCoupleAnimations();

	if (_anim2ActvFl)
		handleWalkAnimation();

	if (_game._player._moving)
		handlePlayerWalk();

	if (_game._trigger == 91) {
		_vm->_dialogs->show(10434);
		_vm->_gameConv->run(7);
		_vm->_gameConv->exportPointer(&_globals[kPlayerScore]);
	}

	if (_game._trigger == 93) {
		_scene->_nextSceneId = 103;
		_game._player._playerPos.x = 400;
		_globals[kRoom103104Transition] = 0;
	}

	if (_game._trigger == 94) {
		_scene->_nextSceneId = 103;
		_globals[kRoom103104Transition] = 0;
	}
}

void Scene104::preActions() {
	if (_action.isAction(NOUN_EXIT, NOUN_STAGE_LEFT))
		_game._player._walkOffScreenSceneId = 108;

	if (_action.isAction(NOUN_EXIT, NOUN_STAGE_RIGHT))
		_game._player._walkOffScreenSceneId = 107;

	if (_action.isAction(VERB_OPEN, NOUN_TRAP_DOOR) || _action.isAction(VERB_CLOSE, NOUN_TRAP_DOOR))
		_game._player.walk(Common::Point(320, 92), FACING_NORTH);
}

void Scene104::actions() {
	if (_vm->_gameConv->_running == 7) {
		processConversations();
		_action._inProgress = false;
		return;
	}

	if (_action.isAction(VERB_CLIMB_THROUGH, NOUN_TRAP_DOOR)) {
		if (_globals[kTrapDoorStatus] == 0) {
			switch (_game._trigger) {
			case 0:
				_game._player._visible = false;
				_globals._sequenceIndexes[1] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[1], false, 8, 0, 0, 1);
				_scene->_sequences.setDepth(_globals._sequenceIndexes[1], 13);
				_scene->_sequences.setRange(_globals._sequenceIndexes[1], 1, 16);
				_scene->_sequences.setTrigger(_globals._sequenceIndexes[1], SEQUENCE_TRIGGER_EXPIRE, 0, 60);
				break;

			case 60:
				_scene->_nextSceneId = 103;
				_globals[kRoom103104Transition] = 1;
				_game._player._stepEnabled = true;
				break;

			default:
				break;
			}
		} else {
			_vm->_dialogs->show(10429);
		}
		_action._inProgress = false;
		return;
	}

	if (_action.isAction(VERB_JUMP_INTO, NOUN_ORCHESTRA_PIT)) {
		switch (_game._trigger) {
		case 0:
			if (_game._player._playerPos.x > 400)
				_globals[kDeathLocation] = 0;
			else if (_game._player._playerPos.x > 200)
				_globals[kDeathLocation] = 1;
			else
				_globals[kDeathLocation] = 2;

			_scene->changeVariant(2);

			if (_globals[kTrapDoorStatus] == 1)
				_scene->drawToBackground(_globals._spriteIndexes[0], 1, Common::Point(-32000, -32000), 0, 100);
			else
				_scene->drawToBackground(_globals._spriteIndexes[0], 2, Common::Point(-32000, -32000), 0, 100);

			_vm->_dialogs->show(10426);
			_game._player._visible = false;
			_game._player._stepEnabled = false;
			_globals._sequenceIndexes[3] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[3], false, 7, 0, 0, 1);
			_scene->_sequences.setDepth(_globals._sequenceIndexes[3], 15);
			_scene->_sequences.setRange(_globals._sequenceIndexes[3], -1, 4);
			_scene->_sequences.setPosition(_globals._sequenceIndexes[3], _game._player._playerPos);
			_scene->_sequences.setSeqPlayer(_globals._sequenceIndexes[3], true);
			_scene->_sequences.setTrigger(_globals._sequenceIndexes[3], SEQUENCE_TRIGGER_EXPIRE, 0, 1);
			break;

		case 1:
			_globals._sequenceIndexes[3] = _scene->_sequences.startPingPongCycle(_globals._spriteIndexes[3], false, 1, 0, 0, 10);
			_scene->_sequences.setTimingTrigger(60, 2);
			_scene->_sequences.setRange(_globals._sequenceIndexes[3], 4, 4);
			_scene->_sequences.setDepth(_globals._sequenceIndexes[3], 15);
			_scene->_sequences.setSeqPlayer(_globals._sequenceIndexes[3], true);
			_scene->_sequences.setMotion(_globals._sequenceIndexes[3], 0, 0, 200);
			break;

		case 2:
			_vm->_sound->command(1);
			_vm->_sound->command(67);
			_scene->_nextSceneId = 102;
			break;

		default:
			break;
		}
		_action._inProgress = false;
		return;
	}

	if (_action._lookFlag) {
		if (_globals[kCurrentYear] == 1993)
			_vm->_dialogs->show(10410);
		else
			_vm->_dialogs->show(10411);

		_action._inProgress = false;
		return;
	}

	if (_action.isAction(VERB_LOOK) || _action.isAction(VERB_LOOK_AT)) {
		if (_action.isAction(NOUN_STAGE)) {
			_vm->_dialogs->show(10412);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_APRON)) {
			_vm->_dialogs->show(10413);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_PROSCENIUM_ARCH)) {
			_vm->_dialogs->show(10414);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_ACT_CURTAIN)) {
			_vm->_dialogs->show(10415);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_ORCHESTRA_PIT)) {
			_vm->_dialogs->show(10416);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_CONDUCTORS_STAND)) {
			_vm->_dialogs->show(10417);
			_action._inProgress = false;
			return;
		}

		if ((_action.isAction(NOUN_MUSIC_STAND)) || (_action.isAction(NOUN_MUSIC_STANDS))) {
			_vm->_dialogs->show(10418);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_PROMPTERS_BOX)) {
			_vm->_dialogs->show(10419);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_TRAP_DOOR)) {
			_vm->_dialogs->show(10420);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_HOUSE)) {
			if (_globals[kCurrentYear] == 1881)
				_vm->_dialogs->show(10421);
			else
				_vm->_dialogs->show(10427);

			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_STAGE_LEFT)) {
			_vm->_dialogs->show(10422);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_STAGE_RIGHT)) {
			_vm->_dialogs->show(10423);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_CHANDELIER)) {
			_vm->_dialogs->show(10428);
			_action._inProgress = false;
			return;
		}

		if (_action.isAction(NOUN_MONSIEUR_RICHARD)) {
			_vm->_dialogs->show(10433);
			_action._inProgress = false;
			return;
		}
	}

	if (_action.isAction(VERB_JUMP_INTO, NOUN_ORCHESTRA_PIT)) {
		_vm->_dialogs->show(10426);
		_scene->_nextSceneId = 102;
		_action._inProgress = false;
		return;
	}

	if (_action.isAction(VERB_OPEN, NOUN_PROMPTERS_BOX) || _action.isAction(VERB_CLOSE, NOUN_PROMPTERS_BOX)) {
		_vm->_dialogs->show(10430);
		_action._inProgress = false;
		return;
	}

	if (_action.isAction(VERB_OPEN, NOUN_TRAP_DOOR)) {
		if (_globals[kTrapDoorStatus] == 0)
			_vm->_dialogs->show(10424);
		else
			_vm->_dialogs->show(10432);

		_action._inProgress = false;
		return;
	}

	if (_action.isAction(VERB_CLOSE, NOUN_TRAP_DOOR)) {
		if (_globals[kTrapDoorStatus] == 1)
			_vm->_dialogs->show(10425);
		else
			_vm->_dialogs->show(10433);

		_action._inProgress = false;
		return;
	}

	if (_action.isAction(VERB_TAKE, NOUN_CHANDELIER)) {
		_vm->_dialogs->show(10435);
		_action._inProgress = false;
		return;
	}
}

void Scene104::cleanInventory() {
	if (_game._objects.isInInventory(OBJ_LARGE_NOTE))
		_game._objects.setRoom(OBJ_LARGE_NOTE, NOWHERE);

	if (_game._objects.isInInventory(OBJ_SANDBAG))
		_game._objects.setRoom(OBJ_SANDBAG, NOWHERE);

	if (_game._objects.isInInventory(OBJ_SMALL_NOTE))
		_game._objects.setRoom(OBJ_SMALL_NOTE, NOWHERE);

	if (_game._objects.isInInventory(OBJ_PARCHMENT))
		_game._objects.setRoom(OBJ_PARCHMENT, NOWHERE);

	if (_game._objects.isInInventory(OBJ_BOOK))
		_game._objects.setRoom(OBJ_BOOK, NOWHERE);

	if (_game._objects.isInInventory(OBJ_RED_FRAME))
		_game._objects.setRoom(OBJ_RED_FRAME, 105);

	if (_game._objects.isInInventory(OBJ_YELLOW_FRAME))
		_game._objects.setRoom(OBJ_YELLOW_FRAME, 107);

	if (_game._objects.isInInventory(OBJ_BLUE_FRAME))
		_game._objects.setRoom(OBJ_BLUE_FRAME, 302);

	if (_game._objects.isInInventory(OBJ_GREEN_FRAME))
		_game._objects.setRoom(OBJ_GREEN_FRAME, 307);
}

void Scene104::processConversations() {
	bool interlocutorTriggerFl = false;
	bool heroTriggerFl = false;

	switch (_action._activeAction._verbId) {
	case 2:
	case 10:
	case 12:
	case 16:
	case 20:
	case 21:
	case 24:
		_vm->_gameConv->setInterlocutorTrigger(75);
		interlocutorTriggerFl = true;
		break;

	case 3:
		if (!_needToGetUp) {
			_vm->_gameConv->setInterlocutorTrigger(67);
			interlocutorTriggerFl = true;
			_needToGetUp = true;
		}
		break;

	case 8:
		_vm->_gameConv->setInterlocutorTrigger(87);
		interlocutorTriggerFl = true;
		break;

	case 11:
		_vm->_gameConv->setInterlocutorTrigger(77);
		interlocutorTriggerFl = true;
		break;

	case 14:
	case 25:
		if (!_game._trigger) {
			_richStatus = 0;
			_coupleStatus = 5;
			_vm->_gameConv->hold();
		}
		break;

	case 22:
		_vm->_gameConv->setInterlocutorTrigger(75);
		_vm->_gameConv->setHeroTrigger(79);
		interlocutorTriggerFl = true;
		heroTriggerFl  = true;
		break;

	case 23:
		_vm->_gameConv->setInterlocutorTrigger(89);
		interlocutorTriggerFl = true;
		break;

	case 28:
		_vm->_gameConv->setInterlocutorTrigger(81);
		interlocutorTriggerFl = true;
		break;

	case 30:
		if (!_game._trigger) {
			_vm->_gameConv->hold();
			_richStatus = 4;
		}
		break;

	case 32:
		_coupleStatus = 14;
		heroTriggerFl = true;
		interlocutorTriggerFl = true;
		_vm->_gameConv->hold();
		break;

	default:
		break;
	}

	switch (_game._trigger) {
	case 67:
		_vm->_gameConv->hold();
		_coupleStatus = 12;
		break;

	case 69:
		if (!_beforeSheLeaves && !_beforeHeLeaves && (_coupleStatus != 14) && !_needToStandUp) {
			_richStatus = 0;
			if (_sittingUp)
				_coupleStatus = 4;
			else
				_coupleStatus = 12;
		}
		break;

	case 71:
		if (!_beforeSheLeaves && !_beforeHeLeaves && (_coupleStatus != 14) && !_needToStandUp) {
			_richStatus = 0;
			if (_sittingUp && !_beforeSheLeaves) {
				_coupleStatus = 3;
				_richStatus = 0;
			}
		}
		break;

	case 75:
		_richStatus = 1;

		if (_sittingUp) {
			if (_action._activeAction._verbId == 20) {
				_lookCount = 0;
				_coupleStatus = 9;
			} else if ((_action._activeAction._verbId == 21) || (_action._activeAction._verbId == 22)) {
				_lookCount = 0;
				_coupleStatus = 10;
			} else {
				_coupleStatus = 1;
			}
		} else {
			_coupleStatus = 11;
		}
		break;

	case 77:
		_richStatus = 0;
		_coupleStatus = 8;
		break;

	case 79:
		_richStatus = 0;
		_coupleStatus = 7;
		break;

	case 81:
		_richStatus = 1;
		_beforeHeLeaves = true;
		_coupleStatus = 15;
		break;

	case 83:
		_vm->_gameConv->release();
		if (_coupleStatus != 17)
			_game._player._stepEnabled = false;
		break;

	case 87:
		_richStatus = 3;
		break;

	case 89:
		_richStatus = 2;
		break;

	default:
		break;
	}

	if (!heroTriggerFl && !_beforeSheLeaves)
		_vm->_gameConv->setHeroTrigger(71);

	if (!interlocutorTriggerFl)
		_vm->_gameConv->setInterlocutorTrigger(69);

	_richTalkCount = 0;
	_manTalkCount = 0;
	_womanTalkCount = 0;
}

void Scene104::handleWalkAnimation() {
	if (_scene->_animation[_globals._animationIndexes[2]]->getCurrentFrame() == _walkFrame)
		return;

	_walkFrame = _scene->_animation[_globals._animationIndexes[2]]->getCurrentFrame();
	int daae_walk_reset_frame = -1;

	switch (_walkFrame) {
	case 1:
		if (_walkStatus == 0) {
			daae_walk_reset_frame = 0;
		} else {
			_game.syncTimers(3, _globals._animationIndexes[2], 3, _globals._animationIndexes[1]);
			daae_walk_reset_frame = 1;
		}
		break;

	case 138:
		_walkStatus = 0;
		daae_walk_reset_frame = 0;
		break;

	default:
		break;
	}

	if (daae_walk_reset_frame >= 0) {
		_scene->setAnimFrame(_globals._animationIndexes[2], daae_walk_reset_frame);
		_walkFrame = daae_walk_reset_frame;
	}
}

void Scene104::handleRichAnimations() {
	if (_scene->_animation[_globals._animationIndexes[0]]->getCurrentFrame() == _richFrame)
		return;

	_richFrame = _scene->_animation[_globals._animationIndexes[0]]->getCurrentFrame();
	int random;
	int resetFrame = -1;

	switch (_richFrame) {
	case 1:
	case 2:
	case 3:
	case 4:
	case 8:
	case 14:
	case 22:
	case 34:
	case 40:
	case 44:
	case 48:
		random = -1;
		if (_richStatus == 1) {
			random = _vm->getRandomNumber(1, 3);
			++_richTalkCount;
			if (_richTalkCount > 15) {
				_richStatus = 0;
				random = 40;
			}
		}

		if (_richStatus == 0)
			random = _vm->getRandomNumber(7, 80);

		if (_richStatus == 2) {
			random = 4;
			_richStatus = 1;
			_richTalkCount = 8;
		}

		if (_richStatus == 3) {
			random = 5;
			_richStatus = 1;
			_richTalkCount = 8;
		}

		if (_richStatus == 4)
			random = 6;

		if (_richStatus == 5)
			random = 7;

		switch (random) {
		case 1:
			resetFrame = 1;
			break;

		case 2:
			resetFrame = 2;
			break;

		case 3:
			resetFrame = 3;
			break;

		case 4:
			resetFrame = 23;
			break;

		case 5:
			resetFrame = 35;
			break;

		case 6:
			resetFrame = 49;
			break;

		case 7:
			resetFrame = 41;
			break;

		case 8:
			resetFrame = 45;
			break;

		case 9:
			resetFrame = 9;
			break;

		case 10:
			resetFrame = 5;
			break;

		case 11:
			resetFrame = 15;
			break;

		default:
			resetFrame = 0;
			break;
		}
		break;

	case 117:
		_coupleStatus = 13;
		break;

	case 125:
		resetFrame = 124;
		break;

	default:
		break;
	}

	if (resetFrame >= 0) {
		_scene->setAnimFrame(_globals._animationIndexes[0], resetFrame);
		_richFrame = resetFrame;
	}
}

void Scene104::handleCoupleAnimations() {
	if (_scene->_animation[_globals._animationIndexes[1]]->getCurrentFrame() == _coupleFrame)
		return;

	_coupleFrame = _scene->_animation[_globals._animationIndexes[1]]->getCurrentFrame();
	int resetFrame = -1;
	int random;

	switch (_coupleFrame) {
	case 1:
	case 103:
	case 104:
	case 105:
		if (_coupleStatus == 11)
			resetFrame = 0;
		else {
			resetFrame = _vm->getRandomNumber(102, 104);
			++_womanTalkCount;
			if (_womanTalkCount > 15) {
				if (_needToGetUp) {
					_coupleStatus = 6;
					resetFrame   = 1;
				} else {
					_coupleStatus = 11;
					resetFrame   = 0;
				}
			}
		}
		break;

	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 25:
	case 33:
	case 41:
		switch (_coupleFrame) {
		case 9:
			_coupleStatus = 6;
			break;

		case 33:
			_vm->_gameConv->release();
			if (_action._activeAction._verbId == 13)
				_coupleStatus = 4;

			break;

		case 41:
			_vm->_gameConv->release();
			_sittingUp = true;
			if (_needToTalk)
				_coupleStatus = 3;
			else
				_coupleStatus = 1;
			break;
		}

		random = -1;

		switch (_coupleStatus) {
		case 1:
			random = 12;
			break;

		case 2:
		case 7:
		case 8:
			random = 11;
			break;

		case 3:
			random = _vm->getRandomNumber(4, 6);
			++_manTalkCount;
			if (_manTalkCount > 15) {
				_coupleStatus = 1;
				random = 12;
			}
			break;

		case 4:
			if (_beforeSheLeaves) {
				random = 10;
			} else {
				random = _vm->getRandomNumber(1, 3);
				++_womanTalkCount;
				if (_womanTalkCount > 15) {
					_coupleStatus = 1;
					random = 12;
				}
			}
			break;

		case 5:
			_coupleStatus = 1;
			random = 8;
			break;

		case 6:
			_coupleStatus = 1;
			random = 7;
			break;

		case 13:
			random = 9;
			break;

		case 15:
			random = 10;
			break;

		default:
			break;
		}

		switch (random) {
		case 1:
			resetFrame = 12;
			break;

		case 2:
			resetFrame = 13;
			break;

		case 3:
			resetFrame = 14;
			break;

		case 4:
			resetFrame = 9;
			break;

		case 5:
			resetFrame = 10;
			break;

		case 6:
			resetFrame = 11;
			break;

		case 7:
			resetFrame = 33;
			break;

		case 8:
			resetFrame = 25;
			break;

		case 9:
			resetFrame = 54;
			break;

		case 10:
			resetFrame = 41;
			break;

		case 11:
			resetFrame = 15;
			break;

		case 12:
			resetFrame = 14;
			break;

		default:
			break;
		}
		break;

	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
		random = -1;
		switch (_coupleStatus) {
		case 1:
		case 3:
		case 4:
		case 5:
		case 6:
		case 15:
		case 16:
			random = 7;
			break;

		case 2:
			random = 8;
			break;

		case 7:
			random = _vm->getRandomNumber(4, 6);
			++_manTalkCount;
			if (_manTalkCount > 15) {
				_coupleStatus = 2;
				random = 8;
			}
			break;

		case 8:
			random = _vm->getRandomNumber(1, 3);
			++_womanTalkCount;
			if (_womanTalkCount > 15) {
				_coupleStatus = 1;
				random = 7;
			}
			break;

		case 9:
			random = 1;
			++_lookCount;
			if (_lookCount > 6) {
				_coupleStatus = 1;
				random = 7;
			}
			break;

		case 10:
			random = 1;
			break;

		default:
			break;
		}

		switch (random) {
		case 1:
			resetFrame = 20;
			break;

		case 2:
			resetFrame = 21;
			break;

		case 3:
			resetFrame = 22;
			break;

		case 4:
			resetFrame = 17;
			break;

		case 5:
			resetFrame = 18;
			break;

		case 6:
			resetFrame = 19;
			break;

		case 7:
			resetFrame = 23;
			break;

		case 8:
			resetFrame = 20;
			break;

		default:
			break;
		}
		break;

	case 52:
		_walkStatus = 1;
		resetFrame = 54;
		break;

	case 55:
		if (_coupleStatus != 13)
			resetFrame = 54;

		break;

	case 89:
		_vm->_gameConv->release();
		break;

	case 90:
		if (_coupleStatus != 14) {
			resetFrame = 89;
		} else {
			resetFrame = 90;
			_globals[kTempVar] = 200;
		}
		break;

	case 102:
		_vm->_gameConv->release();
		_game._player._playerPos = Common::Point(166, 126);
		_game._player.resetFacing(FACING_SOUTH);
		resetFrame = 105;
		_game._player._visible = true;
		_game.syncTimers(2, 0, 3, _globals._animationIndexes[1]);
		break;

	case 106:
		_coupleStatus = 17;
		resetFrame = 105;
		break;

	default:
		break;
	}

	if (resetFrame >= 0) {
		_scene->setAnimFrame(_globals._animationIndexes[1], resetFrame);
		_coupleFrame = resetFrame;
	}
}

void Scene104::handlePlayerWalk() {
	if (_game._player._frameNumber == _lastPlayerFrame)
		return;

	_lastPlayerFrame = _game._player._frameNumber;
	switch (_game._player._facing) {
	case FACING_NORTH:
	case FACING_SOUTH:
		if ((_game._player._frameNumber == 5) || (_game._player._frameNumber == 11))
			_vm->_sound->command(68);
		break;

	case FACING_NORTHEAST:
	case FACING_NORTHWEST:
	case FACING_SOUTHEAST:
	case FACING_SOUTHWEST:
		if ((_game._player._frameNumber == 7) || (_game._player._frameNumber == 14))
			_vm->_sound->command(68);
		break;

	case FACING_EAST:
	case FACING_WEST:
		if ((_game._player._frameNumber == 8) || (_game._player._frameNumber == 16))
			_vm->_sound->command(68);
		break;

	default:
		break;
	}
}

/*------------------------------------------------------------------------*/

} // End of namespace Phantom
} // End of namespace MADS
