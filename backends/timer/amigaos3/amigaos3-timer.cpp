#include "amigaos3-timer.h"

#include "assert.h"
#include "common/debug.h"
#include "common/util.h"

#include <clib/alib_protos.h>  // CreateTask
#include <proto/exec.h>
#include <proto/realtime.h>

AmigaOS3TimerManager *AmigaOS3TimerManager::_s_instance = nullptr;

void __saveds __interrupt AmigaOS3TimerManager::TimerTask(void) {
	AmigaOS3TimerManager *const tm = _s_instance;

	// Increase own prriority, so we'll hopefully never miss a trigger
	struct Task *self = FindTask(NULL);
	SetTaskPri(self, 21);

	// Tell main thread we're alive
	Signal(tm->_mainTask, SIGBREAKF_CTRL_F);

	while (true) {
		ULONG signals = Wait(tm->_timerSignalMask | SIGBREAKF_CTRL_C | SIGBREAKF_CTRL_F);
		if (signals & SIGBREAKF_CTRL_C) {
			// Main task wants to end thread
			break;
		}
		// This is just a wakup call, for instance when main thread adds or removes
		// a player and thus needed to update timerSignalMask
		if (signals & SIGBREAKF_CTRL_F) {
			// acknowledge that we received the change to timerSignalMask
			signals &= ~SIGBREAKF_CTRL_F;
			Signal(tm->_mainTask, SIGBREAKF_CTRL_F);
		}

		BYTE playerId = 0;
		while (signals) {
			if ((signals & 1) && tm->_allTimers[playerId].player) {
				const auto &slot = tm->_allTimers[playerId];
				assert(slot.player);
				assert(slot.player->pl_PlayerID == playerId);

				{
					auto lock = LockRealTime(RT_CONDUCTORS);  // do I need this?
					LONG res = SetPlayerAttrs(slot.player, PLAYER_AlarmTime, slot.player->pl_AlarmTime + slot.tics,
											  PLAYER_Ready, TRUE, TAG_END);
					UnlockRealTime(lock);
				}

				((Common::TimerManager::TimerProc)slot.player->pl_UserData)(slot.refCon);
			}
			playerId++;
			signals >>= 1;
		}
	}

	Signal(tm->_mainTask, SIGBREAKF_CTRL_F);
}

AmigaOS3TimerManager::AmigaOS3TimerManager()
  : _timerTask(nullptr), _mainTask(nullptr), _timerSignalMask(0), _numTimers(0) {
	assert(RealTimeBase);
	assert(!_s_instance);
	_s_instance = this;

	memset(_allTimers, 0, sizeof(_allTimers));
	_mainTask = FindTask(NULL);

	if ((_timerTask = CreateTask("ScummVM TimerManager", 10, (APTR)&TimerTask, 100000)) == NULL) {
		error("Can't create TimerManager subtask");
	}

	Wait(SIGBREAKF_CTRL_F);
	debug(1, "AmigaOS3TimerManager() created \n");
}

AmigaOS3TimerManager::~AmigaOS3TimerManager() {
	if (_timerTask) {
		// now tell the timerTask to finish
		Signal(_timerTask, SIGBREAKF_CTRL_C);
	}

	if (_numTimers) {
		// Find the first active player to stop the conductor
		auto lock = LockRealTime(RT_CONDUCTORS);
		for (auto &slot : _allTimers) {
			if (slot.player) {
				SetConductorState(slot.player, CONDSTATE_STOPPED, 0);
			}
		}
		UnlockRealTime(lock);
	}

	if (_timerTask) {
		Wait(SIGBREAKF_CTRL_F);
	}

	for (auto &slot : _allTimers) {
		if (slot.player) {
			BYTE signalBit = (BYTE)slot.player->pl_PlayerID;
			DeletePlayer(slot.player);
			FreeSignal(signalBit);
		}
	}

	_s_instance = nullptr;
}

bool AmigaOS3TimerManager::installTimerProc(Common::TimerManager::TimerProc proc, int32 interval, void *refCon,
											const Common::String &id) {
	// DefaultTimerManager seems to check that neither the same id nor the same proc gets
	// installed twice and would error out on it.
	// I'm just relying on that premise and do not do those checks here.
	{
		Common::StackLock lock(_mutex);

		LONG tics = MAX(1, (interval * TICK_FREQ) / 1000000);

		BYTE timerSignalBit = AllocSignal(-1L);
		if (timerSignalBit == -1) {
			error("AmigaOS3TimerManager() Couldn't allocate additional signal bit\n");
			return false;
		}
		assert(!_allTimers[timerSignalBit].player);

		struct Player *player = nullptr;

		{
			auto rtLock = LockRealTime(RT_CONDUCTORS);  // I believe I need to do this as two tasks
			// will access the same conductor/player state

			ULONG err = 0;
			player =
			  CreatePlayer(PLAYER_Name, (Tag)id.c_str(), PLAYER_Conductor, (Tag) "ScummVM TimerManager Conductor",
						   PLAYER_AlarmSigTask, (Tag)_timerTask, PLAYER_AlarmSigBit, (Tag)timerSignalBit, PLAYER_ID,
						   (Tag)timerSignalBit, PLAYER_UserData, (Tag)proc, PLAYER_ErrorCode, (Tag)&err, TAG_END);
			if (!player) {
				FreeSignal(timerSignalBit);
				error("AmigaOS3TimerManager() can't create player; error: %lu\n", err);
				return false;
			}

			if (!_numTimers) {
				// This is the first timer, get the ball rolling
				err = SetConductorState(player, CONDSTATE_RUNNING, 0);
				if (err) {
					DeletePlayer(player);
					FreeSignal(timerSignalBit);
					error("SetConductorState failed: %lu", err);
					return false;
				}
			}

			BOOL success =
			  SetPlayerAttrs(player, PLAYER_AlarmTime, RealTimeBase->rtb_Time + tics, PLAYER_Ready, TRUE, TAG_END);
			if (!success) {
				DeletePlayer(player);
				FreeSignal(timerSignalBit);

				error("SetPlayerAttrs failed");
				return false;
			}

			UnlockRealTime(rtLock);
		}

		auto &slot = _allTimers[timerSignalBit];
		slot.player = player;
		slot.tics = (ULONG)tics;
		slot.refCon = refCon;

		++_numTimers;
		debug(1,
			  "AmigaOS3TimerManager() '%s' (proc %p) as timer '%d' at %ld tics/s, %d timers alive, current time: %lu\n",
			  id.c_str(), (void *)proc, timerSignalBit, tics, _numTimers, RealTimeBase->rtb_Time);

		_timerSignalMask |= 1L << timerSignalBit;
	}
	// tell the timer task that the timerSignalMask has changed
	Signal(_timerTask, SIGBREAKF_CTRL_F);
	Wait(SIGBREAKF_CTRL_F);

	return true;
}

void AmigaOS3TimerManager::removeTimerProc(Common::TimerManager::TimerProc proc) {
	Common::StackLock lock(_mutex);

	if (_numTimers <= 0) {
		// It seems, sometimes the same procedure gets removed more than once
		debug(1, "AmigaOS3TimerManager::removeTimerProc() no timers left to remove for proc %p", (void *)proc);
		return;
	}

	for (auto &slot : _allTimers) {
		auto &player = slot.player;
		if (player && player->pl_UserData == (void *)proc) {
			BYTE signalBit = (BYTE)player->pl_PlayerID;

			// Prevent the timer thread from waiting on it.
			// Not sure if I'm allowed to free a signal that is still potentially waited on.
			// But since this is never the only signal the timerTask is waiting on,
			// it would hardly block it.
			_timerSignalMask &= ~(1L << signalBit);
			Signal(_timerTask, SIGBREAKF_CTRL_F);  // signal the change of g_timerSignalMask
			Wait(SIGBREAKF_CTRL_F);				   // wait for acknowledge
			FreeSignal(signalBit);

			{
				auto lock = LockRealTime(RT_CONDUCTORS);
				DeletePlayer(player);
				UnlockRealTime(lock);
			}
			player = nullptr;

			_numTimers--;
			debug(1, "AmigaOS3TimerManager::removeTimerProc() timer %d removed or proc %p; %d timers left\n", signalBit,
				  (void *)proc, _numTimers);
			break;
		}
	}
}
