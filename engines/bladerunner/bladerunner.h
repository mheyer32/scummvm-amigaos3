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

#ifndef BLADERUNNER_BLADERUNNER_H
#define BLADERUNNER_BLADERUNNER_H

#include "bladerunner/archive.h"

#include "common/array.h"
#include "common/random.h"
#include "common/stream.h"
#include "common/types.h"

#include "engines/engine.h"

#include "graphics/surface.h"

namespace BladeRunner {

class Actor;
class AmbientSounds;
class AudioPlayer;
class AudioSpeech;
class Chapters;
class Clues;
class GameInfo;
class GameFlags;
class Mouse;
class Scene;
class SceneObjects;
class Script;
class Settings;
class Shape;
class SliceAnimations;
class SliceRenderer;
class TextResource;
class Lights;
class View;
class Waypoints;
class Items;


class BladeRunnerEngine : public Engine {
public:
	bool      _gameIsRunning;
	bool      _windowIsActive;
	int       _playerLosesControlCounter;

	AmbientSounds   *_ambientSounds;
	AudioPlayer     *_audioPlayer;
	AudioSpeech     *_audioSpeech;
	Chapters        *_chapters;
	Clues           *_clues;
	GameFlags       *_gameFlags;
	GameInfo        *_gameInfo;
	Mouse           *_mouse;
	View            *_view;
	Scene           *_scene;
	SceneObjects    *_sceneObjects;
	Script          *_script;
	Settings        *_settings;
	SliceAnimations *_sliceAnimations;
	SliceRenderer   *_sliceRenderer;
	int             *_gameVars;

	Lights          *_lights;
	Waypoints       *_waypoints;
	Items           *_items;

	TextResource    *_textActorNames;
	TextResource    *_textCrimes;
	TextResource    *_textCluetype;
	TextResource    *_textKIA;
	TextResource    *_textSpindest;
	TextResource    *_textVK;
	TextResource    *_textOptions;

	Common::Array<Shape*> _shapes;

	Actor *_actors[99];
	Actor *_voiceoverActor;
	Actor *_playerActor;

	int in_script_counter;

	Graphics::Surface  _surface1;
	Graphics::Surface  _surface2;
	uint16            *_zBuffer1;
	uint16            *_zBuffer2;

	Common::RandomSource _rnd;

	bool _playerActorIdle;
	bool _playerDead;

private:
	static const int kArchiveCount = 10;
	MIXArchive _archives[kArchiveCount];

public:
	BladeRunnerEngine(OSystem *syst);
	~BladeRunnerEngine();

	bool hasFeature(EngineFeature f) const;

	Common::Error run();

	bool startup(bool hasSavegames = false);
	void initChapterAndScene();
	void shutdown();

	bool loadSplash();
	bool init2();

	void gameLoop();
	void gameTick();
	void handleEvents();
	void loopActorSpeaking();

	void outtakePlay(int id, bool no_localization, int container = -1);

	bool openArchive(const Common::String &name);
	bool closeArchive(const Common::String &name);
	bool isArchiveOpen(const Common::String &name);

	Common::SeekableReadStream *getResourceStream(const Common::String &name);

	bool playerHasControl();
	void playerLosesControl();
	void playerGainsControl();

	void ISez(const char *str);
};

static inline
const Graphics::PixelFormat createRGB555() {
	return Graphics::PixelFormat(2, 5, 5, 5, 0, 10, 5, 0, 0);
}

} // End of namespace BladeRunner

#endif
