#include "amigaos3-timer.h"

#include "assert.h"
#include "common/debug.h"
#include "common/util.h"

#include <clib/alib_protos.h>  // CreateTask
#include <proto/exec.h>
#include <proto/realtime.h>

// Hook code called whenever realtime.library updates Conductor time.
// Normally this is 600 times/sec.
// uint32 __saveds __stdargs My_hookFunc(struct Hook *hook, struct pmTime *msg, struct PlayerInfo *pi) {
//	switch (msg->pmt_Method) {
//	case PM_TICK:
//		// Test whether Conductor time has exceeded the number in *ticks*.
//		// If it has, then signal the parent task.
//		if ((*uint32 *)(pi->pi_Userdata)) < pi->pi_Source->cdt_ClockTime)
//	  IExec->Signal(My_task, 1UL << My_signal);
//		break;
//	default:
//		break;
//	}

//	return 0;
//}

static volatile ULONG timerSignalMask;

struct Task *mainTask = nullptr;

struct TimerSlot {
	struct Player *player;
	ULONG tics;
	void *refCon;
};

static TimerSlot allTimers[32] = {{nullptr, 1, nullptr}};

int numReceivedTriggers = 0;

static void __saveds __interrupt TimerTask(void) {
	// Increase own prriority, so we'll hopefully never miss a trigger
	struct Task *mt = FindTask(NULL);
	SetTaskPri(mt, 21);

	// Tell main thread we're alive
	Signal(mainTask, SIGBREAKF_CTRL_F);

	while (true) {
		ULONG signals = 0;

		signals = Wait(timerSignalMask | SIGBREAKF_CTRL_C | SIGBREAKF_CTRL_F);
		if (signals & SIGBREAKF_CTRL_C) {
			// Main task wants to end thread
			break;
		}
		// This is just a wakup call, for instance when main thread adds or removes
		// a player and thus needed to update timerSignalMask
		signals &= ~SIGBREAKF_CTRL_F;

		// need to make sure, the player doesn't get deleted while we're handling
		// an alarm
		//		Common::StackLock lock(_mutex);

		BYTE playerId = 0;
		while (signals) {
			if ((signals & 1) && allTimers[playerId].player) {
				const auto &slot = allTimers[playerId];
				assert(slot.player);
				assert(slot.player->pl_PlayerID == playerId);

				//				LONG res = SetConductorState(slot.player, CONDSTATE_RUNNING, 0);
				LONG res = SetPlayerAttrs(slot.player, PLAYER_AlarmTime, slot.player->pl_AlarmTime + slot.tics,
										  PLAYER_Ready, TRUE, TAG_END);

				((Common::TimerManager::TimerProc)slot.player->pl_UserData)(slot.refCon);
			}
			playerId++;
			signals >>= 1;
		}
	}

	//	debug(1, "TimerTask destroyed\n");
}

AmigaOS3TimerManager::AmigaOS3TimerManager() {
	assert(RealTimeBase);

	timerSignalMask = 0L;

	mainTask = FindTask(NULL);

	if ((_timerTask = CreateTask("ScummVM TimerManager", 10, (APTR)&TimerTask, 100000)) == NULL) {
		error("Can't create subtask");
	}

	Wait(SIGBREAKF_CTRL_F);
	debug(1, "AmigaOS3TimerManager() created \n");
}

AmigaOS3TimerManager::~AmigaOS3TimerManager() {
	if (_timerTask) {
		Signal(_timerTask, SIGBREAKF_CTRL_C);
	}
	_timerTask = 0;

	// Wait(timerTask); // need to wait for the task to finish

	for (int t = 0; t < ARRAYSIZE(allTimers); ++t) {
		auto &slot = allTimers[t];
		if (slot.player) {
			BYTE signalBit = (BYTE)slot.player->pl_PlayerID;
			//			LONG res = SetConductorState(slot.player, CONDSTATE_STOPPED, 0);
			DeletePlayer(slot.player);
			FreeSignal(signalBit);
		}
	}
	timerSignalMask = 0L;
}

bool AmigaOS3TimerManager::installTimerProc(Common::TimerManager::TimerProc proc, int32 interval, void *refCon,
											const Common::String &id) {
	{
		Common::StackLock lock(_mutex);

		LONG tics = MAX(1, (interval * TICK_FREQ) / 1000000);

		BYTE timerSignalBit = AllocSignal(-1L);
		if (timerSignalBit == -1) {
			error("AmigaOS3TimerManager() Couldn't allocate additional signal bit\n");
			return false;
		}
		assert(!allTimers[timerSignalBit].player);

		ULONG err = 0;

		auto player =
		  CreatePlayer(PLAYER_Name, (Tag)id.c_str(), PLAYER_Conductor, (Tag) "ScummVM TimerManager Conductor",
					   PLAYER_AlarmSigTask, (Tag)_timerTask, PLAYER_AlarmSigBit, (Tag)timerSignalBit, PLAYER_ID,
					   (Tag)timerSignalBit, PLAYER_UserData, (Tag)proc, PLAYER_ErrorCode, (Tag)&err, TAG_END);
		if (!player || err) {
			FreeSignal(timerSignalBit);
			error("AmigaOS3TimerManager() Can't create a Player: %d\n", err);
			return false;
		}

		auto &slot = allTimers[timerSignalBit];
		slot.player = player;
		slot.tics = tics;
		slot.refCon = refCon;

		_callbacks[proc] = allTimers[timerSignalBit].player;

		debug(1, "AmigaOS3TimerManager() '%s' inserted, %d timers alive, %u ticks current time: %d\n", id.c_str(),
			  _callbacks.size(), tics, RealTimeBase->rtb_Time);

		// FIXME: this would reset the time for other alarms already set
		LONG res = SetConductorState(player, CONDSTATE_RUNNING, 0);
		if (err) {
			error("SetConductorState failed: %d", err);
			return false;
		}

		res = SetPlayerAttrs(player, PLAYER_AlarmTime, RealTimeBase->rtb_Time + tics, PLAYER_Ready, TRUE, TAG_END);
		if (err) {
			error("SetPlayerAttrs failed: %d", err);
			return false;
		}

		timerSignalMask |= 1L << timerSignalBit;
	}

	debug(1, "AmigaOS3TimerManager() signalling thread %x", _timerTask);

	Signal(_timerTask, SIGBREAKF_CTRL_F);

	return true;
}

void AmigaOS3TimerManager::removeTimerProc(Common::TimerManager::TimerProc proc) {
	{
		Common::StackLock lock(_mutex);

		auto it = _callbacks.find(proc);
		if (it != _callbacks.end()) {
			auto player = it->_value;
			BYTE signalBit = (BYTE)player->pl_PlayerID;
			assert(player == allTimers[signalBit].player);
			allTimers[signalBit].player = NULL;
			DeletePlayer(player);
			timerSignalMask &= ~(1 << signalBit);
			FreeSignal(signalBit);
			_callbacks.erase(it);
			debug(1, "AmigaOS3TimerManager() removed, %d timers left\n", _callbacks.size());
		}
	}

	Signal(_timerTask, SIGBREAKF_CTRL_F);
}
