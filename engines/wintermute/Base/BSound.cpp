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

/*
 * This file is based on WME Lite.
 * http://dead-code.org/redir.php?target=wmelite
 * Copyright (c) 2011 Jan Nedoma
 */

#include "engines/wintermute/Base/BSound.h"
#include "engines/wintermute/Base/BGame.h"
#include "engines/wintermute/Base/BSoundMgr.h"
#include "engines/wintermute/Base/BSoundBuffer.h"

namespace WinterMute {

IMPLEMENT_PERSISTENT(CBSound, false)

//////////////////////////////////////////////////////////////////////////
CBSound::CBSound(CBGame *inGame): CBBase(inGame) {
	_sound = NULL;
	_soundFilename = NULL;

	_soundType = Audio::Mixer::kSFXSoundType;
	_soundStreamed = false;
	_soundLooping = false;
	_soundPlaying = false;
	_soundPaused = false;
	_soundFreezePaused = false;
	_soundPosition = 0;
	_soundPrivateVolume = 0;
	_soundLoopStart = 0;

	_sFXType = SFX_NONE;
	_sFXParam1 = _sFXParam2 = _sFXParam3 = _sFXParam4 = 0;
}


//////////////////////////////////////////////////////////////////////////
CBSound::~CBSound() {
	if (_sound) Game->_soundMgr->removeSound(_sound);
	_sound = NULL;

	delete[] _soundFilename;
	_soundFilename = NULL;
}


//////////////////////////////////////////////////////////////////////////
ERRORCODE CBSound::setSound(const char *filename, Audio::Mixer::SoundType type, bool streamed) {
	if (_sound) {
		Game->_soundMgr->removeSound(_sound);
		_sound = NULL;
	}
	delete[] _soundFilename;
	_soundFilename = NULL;

	_sound = Game->_soundMgr->addSound(filename, type, streamed);
	if (_sound) {
		_soundFilename = new char[strlen(filename) + 1];
		strcpy(_soundFilename, filename);

		_soundType = type;
		_soundStreamed = streamed;

		return STATUS_OK;
	} else return STATUS_FAILED;
}


//////////////////////////////////////////////////////////////////////////
ERRORCODE CBSound::setSoundSimple() {
	_sound = Game->_soundMgr->addSound(_soundFilename, _soundType, _soundStreamed);
	if (_sound) {
		if (_soundPosition) _sound->setPosition(_soundPosition);
		_sound->setLooping(_soundLooping);
		_sound->setPrivateVolume(_soundPrivateVolume);
		_sound->setLoopStart(_soundLoopStart);
		_sound->_freezePaused = _soundFreezePaused;
		if (_soundPlaying) return _sound->resume();
		else return STATUS_OK;
	} else return STATUS_FAILED;
}



//////////////////////////////////////////////////////////////////////////
uint32 CBSound::getLength() {
	if (_sound) return _sound->getLength();
	else return 0;
}


//////////////////////////////////////////////////////////////////////////
ERRORCODE CBSound::play(bool looping) {
	if (_sound) {
		_soundPaused = false;
		return _sound->play(looping, _soundPosition);
	} else return STATUS_FAILED;
}


//////////////////////////////////////////////////////////////////////////
ERRORCODE CBSound::stop() {
	if (_sound) {
		_soundPaused = false;
		return _sound->stop();
	} else return STATUS_FAILED;
}


//////////////////////////////////////////////////////////////////////////
ERRORCODE CBSound::pause(bool freezePaused) {
	if (_sound) {
		_soundPaused = true;
		if (freezePaused) _sound->_freezePaused = true;
		return _sound->pause();
	} else return STATUS_FAILED;
}


//////////////////////////////////////////////////////////////////////////
ERRORCODE CBSound::resume() {
	if (_sound && _soundPaused) {
		_soundPaused = false;
		return _sound->resume();
	} else return STATUS_FAILED;
}


//////////////////////////////////////////////////////////////////////////
ERRORCODE CBSound::persist(CBPersistMgr *persistMgr) {
	if (persistMgr->_saving && _sound) {
		_soundPlaying = _sound->isPlaying();
		_soundLooping = _sound->_looping;
		_soundPrivateVolume = _sound->_privateVolume;
		if (_soundPlaying)
			_soundPosition = _sound->getPosition();
		_soundLoopStart = _sound->_loopStart;
		_soundFreezePaused = _sound->_freezePaused;
	}

	if (persistMgr->_saving) {
		_sFXType = SFX_NONE;
		_sFXParam1 = _sFXParam2 = _sFXParam3 = _sFXParam4 = 0;
	}

	persistMgr->transfer(TMEMBER(Game));

	persistMgr->transfer(TMEMBER(_soundFilename));
	persistMgr->transfer(TMEMBER(_soundLooping));
	persistMgr->transfer(TMEMBER(_soundPaused));
	persistMgr->transfer(TMEMBER(_soundFreezePaused));
	persistMgr->transfer(TMEMBER(_soundPlaying));
	persistMgr->transfer(TMEMBER(_soundPosition));
	persistMgr->transfer(TMEMBER(_soundPrivateVolume));
	persistMgr->transfer(TMEMBER(_soundStreamed));
	persistMgr->transfer(TMEMBER_INT(_soundType));
	persistMgr->transfer(TMEMBER(_soundLoopStart));

	return STATUS_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CBSound::isPlaying() {
	return _sound && _sound->isPlaying();
}


//////////////////////////////////////////////////////////////////////////
bool CBSound::isPaused() {
	return _sound && _soundPaused;
}


//////////////////////////////////////////////////////////////////////////
ERRORCODE CBSound::setPositionTime(uint32 time) {
	if (!_sound) return STATUS_FAILED;
	_soundPosition = time;
	ERRORCODE ret = _sound->setPosition(_soundPosition);
	if (_sound->isPlaying())
		_soundPosition = 0;
	return ret;
}


//////////////////////////////////////////////////////////////////////////
uint32 CBSound::getPositionTime() {
	if (!_sound) return 0;

	if (!_sound->isPlaying())
		return 0;
	else return _sound->getPosition();
}

//////////////////////////////////////////////////////////////////////////
ERRORCODE CBSound::setVolumePercent(int percent) {
	if (!_sound)
		return STATUS_FAILED;
	else return _sound->setPrivateVolume(percent * 255 / 100);
}

//////////////////////////////////////////////////////////////////////////
ERRORCODE CBSound::setVolume(int volume) {
	if (!_sound)
		return STATUS_FAILED;
	else return _sound->setPrivateVolume(volume);
}

//////////////////////////////////////////////////////////////////////////
ERRORCODE CBSound::setPrivateVolume(int volume) {
	if (!_sound) 
		return STATUS_FAILED;
	else return _sound->_privateVolume = volume;
}

//////////////////////////////////////////////////////////////////////////
int CBSound::getVolumePercent() {
	if (!_sound)
		return 0;
	else return _sound->_privateVolume * 100 / 255;
}

//////////////////////////////////////////////////////////////////////////
int CBSound::getVolume() {
	if (!_sound)
		return 0;
	else return _sound->_privateVolume;
}

//////////////////////////////////////////////////////////////////////////
ERRORCODE CBSound::setLoopStart(uint32 pos) {
	if (!_sound)
		return STATUS_FAILED;
	else {
		_sound->setLoopStart(pos);
		return STATUS_OK;
	}
}

//////////////////////////////////////////////////////////////////////////
ERRORCODE CBSound::setPan(float pan) {
	if (_sound)
		return _sound->setPan(pan);
	else return STATUS_FAILED;
}


//////////////////////////////////////////////////////////////////////////
ERRORCODE CBSound::ApplyFX(TSFXType type, float param1, float param2, float param3, float param4) {
	if (!_sound)
		return STATUS_OK;

	if (type != _sFXType || param1 != _sFXParam1 || param2 != _sFXParam2 || param3 != _sFXParam3 || param4 != _sFXParam4) {
		ERRORCODE ret = _sound->applyFX(type, param1, param2, param3, param4);

		_sFXType = type;
		_sFXParam1 = param1;
		_sFXParam2 = param2;
		_sFXParam3 = param3;
		_sFXParam4 = param4;

		return ret;
	}
	return STATUS_OK;
}

} // end of namespace WinterMute
