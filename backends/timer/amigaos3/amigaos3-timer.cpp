#include "amigaos3-timer.h"

#include "assert.h"
#include "common/debug.h"
#include "common/util.h"

#include <clib/alib_protos.h> // DeleteTask
#include <dos/dostags.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/realtime.h>
#include <proto/timer.h>

AmigaOS3TimerManager *AmigaOS3TimerManager::_s_instance = nullptr;

bool AmigaOS3TimerManager::addTimer(Common::TimerManager::TimerProc proc, int32 interval, void *refCon) {
	LONG tics = MAX(1, (interval * TICK_FREQ) / 1000000);

	// FIXME: this is all wrong, but miraculously working.
	// You are not allowed to allocate signals in thread A and wait on them in thread B.
	// Instead, thread B needs to allocate the signals it is waiting on.
	// This will require restructuring all of this code here.
	BYTE timerSignalBit = AllocSignal(-1L);
	if (timerSignalBit == -1) {
		error("AmigaOS3TimerManager() Couldn't allocate additional signal bit\n");
		return false;
	}
	assert(!_allTimers[timerSignalBit].player);

	struct Player *player = nullptr;
	{
		ULONG err = 0;
		player =
			CreatePlayer(PLAYER_Conductor, (Tag) "ScummVM TimerManager Conductor",
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
			SetPlayerAttrs(player, PLAYER_AlarmTime, player->pl_MetricTime + tics, PLAYER_Ready, TRUE, TAG_END);

		if (!success) {
			DeletePlayer(player);
			FreeSignal(timerSignalBit);

			error("SetPlayerAttrs failed");
			return false;
		}
	}

	auto &slot = _allTimers[timerSignalBit];
	slot.player = player;
	slot.tics = (ULONG)tics;
	slot.refCon = refCon;

	++_numTimers;
	debug(1,
		  "AmigaOS3TimerManager() '(proc %p) as timer '%d' at %ld tics/s, %d timers alive, current time: %lu\n",
		  (void *)proc, timerSignalBit, tics, _numTimers, RealTimeBase->rtb_Time);

	_timerSignalMask |= 1L << timerSignalBit;
}

void AmigaOS3TimerManager::removeTimer(TimerProc proc) {
	for (auto &slot : _allTimers) {
		auto &player = slot.player;
		if (player && player->pl_UserData == (void *)proc) {
			UBYTE signalBit = (UBYTE)player->pl_PlayerID;

			_timerSignalMask &= ~(1UL << signalBit);
			FreeSignal(signalBit);

			DeletePlayer(player);
			player = nullptr;

			_numTimers--;
			debug(1, "AmigaOS3TimerManager::removeTimerProc() timer %d removed or proc %p; %d timers left\n", signalBit,
				  (void *)proc, _numTimers);
			break;
		}
	}
}

void __saveds AmigaOS3TimerManager::TimerTask(void) {
	AmigaOS3TimerManager *const tm = _s_instance;

	tm->_timerMP = CreatePort(NULL, 0);
	assert(tm->_timerMP);

	struct MsgPort *const msgPort = tm->_timerMP;

	// Tell main thread we're alive
	Signal(tm->_mainTask, SIGBREAKF_CTRL_F);

	const ULONG mpSignalMask = 1L << msgPort->mp_SigBit;
	ULONG playerSignalMask = 0;

	while (true) {
		ULONG signals = Wait(playerSignalMask | mpSignalMask | SIGBREAKF_CTRL_C);
		if (signals & SIGBREAKF_CTRL_C) {
			// Main task wants to end thread
			break;
		}

		{
			ULONG playerSignals = signals & tm->_timerSignalMask;
			UBYTE playerId = 31;

			while (playerSignals) {
				if ((playerSignals & 0x80000000) && tm->_allTimers[playerId].player) {
					const auto &slot = tm->_allTimers[playerId];
					assert(slot.player);
					assert(slot.player->pl_PlayerID == playerId);

					((Common::TimerManager::TimerProc)slot.player->pl_UserData)(slot.refCon);

					LONG nextAlarm = slot.player->pl_AlarmTime + slot.tics;
					if (nextAlarm < slot.player->pl_MetricTime) {
						nextAlarm = slot.player->pl_MetricTime + slot.tics / 4;
						debug("AmigaOS3TimerManager() TimerTask player fallen behind realtime");
					}

					LONG res = SetPlayerAttrs(slot.player, PLAYER_AlarmTime, nextAlarm,
											  PLAYER_Ready, TRUE, TAG_END);
				}
				playerId--;
				playerSignals <<= 1;
			}
		}

		if (signals & mpSignalMask) {
			TimerMessage *msg = NULL;
			while (msg = (TimerMessage *)GetMsg(msgPort)) {
				switch (msg->cmd) {
				case ADD_TIMER:
					tm->addTimer(msg->proc, msg->interval, msg->refCon);
					playerSignalMask = tm->_timerSignalMask;
					break;
				case REMOVE_TIMER:
					tm->removeTimer(msg->proc);
					playerSignalMask = tm->_timerSignalMask;
					break;
				default:
					assert(false);
				}
				ReplyMsg(&msg->msg);
			}
		}
	}

	if (tm->_numTimers) {
		// Find the first active player to stop the conductor
		for (auto &slot : tm->_allTimers) {
			if (slot.player) {
				SetConductorState(slot.player, CONDSTATE_STOPPED, 0);
			}
		}
	}
	for (auto &slot : tm->_allTimers) {
		if (slot.player) {
			UBYTE signalBit = (UBYTE)slot.player->pl_PlayerID;
			DeletePlayer(slot.player);
			FreeSignal(signalBit);
		}
	}

	struct Message *msg = NULL;
	while (msg = GetMsg(msgPort)) {
		ReplyMsg(msg);
	}
	DeletePort(msgPort);

	// If an instance had been created (as part of a timer callback), then now destroy that instance
	TaskLocalTimer::destroyInstance();

	Signal(tm->_mainTask, SIGBREAKF_CTRL_F);
	Forbid();
}

AmigaOS3TimerManager::AmigaOS3TimerManager()
	: _timerTask(nullptr), _mainTask(nullptr), _timerSignalMask(0), _numTimers(0) {
	assert(RealTimeBase);
	assert(!_s_instance);
	_s_instance = this;

	_mainMP = CreatePort(NULL, 0);

	memset(_allTimers, 0, sizeof(_allTimers));
	_mainTask = FindTask(NULL);

	if ((_timerProcess = CreateNewProcTags(NP_Name, (Tag) "ScummVM TimerManager", NP_Priority, 21, NP_Entry,
										   (Tag)&TimerTask, NP_StackSize, 100000, TAG_END)) == NULL) {
		error("Can't create TimerManager subtask");
	}
	_timerTask = FindTask("ScummVM TimerManager");
	assert(_timerTask);

	// wait for Timertask to report ready
	Wait(SIGBREAKF_CTRL_F);
	debug(1, "AmigaOS3TimerManager() created \n");
}

AmigaOS3TimerManager::~AmigaOS3TimerManager() {
	if (_timerTask) {
		// tell the timerTask to finish
		Signal(_timerTask, SIGBREAKF_CTRL_C);
	}

	if (_mainMP) {
		DeletePort(_mainMP);
	}

	_s_instance = nullptr;
}

bool AmigaOS3TimerManager::installTimerProc(Common::TimerManager::TimerProc proc, int32 interval, void *refCon,
											const Common::String &id) {
	// DefaultTimerManager seems to check that neither the same id nor the same proc gets
	// installed twice and would error out on it.
	// I'm just relying on that premise and do not do those checks here.

	// never install or remove a timer from within the timer task
	assert(FindTask(NULL) != _timerTask);

	{

		Common::StackLock lock(_mutex);

		struct MsgPort *const mainMsgPort = _mainMP;
		struct MsgPort *const timerMsgPort = _timerMP;

		if (timerMsgPort) {
			TimerMessage msg;
			msg.msg.mn_Length = sizeof(msg);
			msg.msg.mn_ReplyPort = mainMsgPort;
			msg.cmd = ADD_TIMER;
			msg.proc = proc;
			msg.interval = interval;
			msg.refCon = refCon;

			PutMsg(timerMsgPort, &msg.msg);
			WaitPort(mainMsgPort);
			GetMsg(mainMsgPort);
		}
	}

	return true;
}

void AmigaOS3TimerManager::removeTimerProc(Common::TimerManager::TimerProc proc) {
	Common::StackLock lock(_mutex);

	// never install or remove a timer from within the timer task
	assert(FindTask(NULL) != _timerTask);

	if (_numTimers <= 0) {
		// It seems, sometimes the same procedure gets removed more than once
		debug(1, "AmigaOS3TimerManager::removeTimerProc() no timers left to remove for proc %p", (void *)proc);
		return;
	}

	{
		Common::StackLock lock(_mutex);

		struct MsgPort *const mainMsgPort = _mainMP;
		struct MsgPort *const timerMsgPort = _timerMP;

		if (timerMsgPort) {
			TimerMessage msg;
			msg.msg.mn_Length = sizeof(msg);
			msg.msg.mn_ReplyPort = mainMsgPort;
			msg.cmd = REMOVE_TIMER;
			msg.proc = proc;

			PutMsg(timerMsgPort, &msg.msg);
			WaitPort(mainMsgPort);
			GetMsg(mainMsgPort);
		}
	}
}

TaskLocalTimer *TaskLocalTimer::instance() {
	struct Task *task = FindTask(NULL);
	if (!task->tc_UserData) {
		task->tc_UserData = new TaskLocalTimer();
	}
	return reinterpret_cast<TaskLocalTimer *>(task->tc_UserData);
}

void TaskLocalTimer::destroyInstance() {
	struct Task *task = FindTask(NULL);
	if (task->tc_UserData) {
		delete reinterpret_cast<TaskLocalTimer *>(task->tc_UserData);
	}
}

TaskLocalTimer::TaskLocalTimer() {
	if ((m_timerMP = CreateMsgPort()) == NULL) {
		exit(EXIT_FAILURE);
	}
	if ((m_timerIOReq = (timerequest *)CreateIORequest(m_timerMP, sizeof(struct timerequest))) == NULL) {
		exit(EXIT_FAILURE);
	}

	BYTE err = OpenDevice("timer.device", UNIT_MICROHZ, &m_timerIOReq->tr_node, 37);
	if (err || m_timerIOReq->tr_node.io_Device == NULL) {
		fprintf(stderr, "Unable to load timer.device!");
		exit(EXIT_FAILURE);
	}

	TimerBase = m_timerIOReq->tr_node.io_Device;
}

TaskLocalTimer::~TaskLocalTimer() {
	CloseDevice(&m_timerIOReq->tr_node);
	DeleteIORequest(m_timerIOReq);
	DeleteMsgPort(m_timerMP);

	struct Task *task = FindTask(NULL);
	task->tc_UserData = NULL;
}

void TaskLocalTimer::delayMillis(uint msecs) {
	if (msecs) {
		if (msecs < 1000) {
			m_timerIOReq->tr_time.tv_secs = 0;
			m_timerIOReq->tr_time.tv_micro = msecs * 1000;
		} else {
			m_timerIOReq->tr_time.tv_secs = msecs / 1000;
			m_timerIOReq->tr_time.tv_micro = (msecs % 1000) * 1000;
		}
		m_timerIOReq->tr_node.io_Command = TR_ADDREQUEST;
		DoIO(&m_timerIOReq->tr_node);
		WaitIO(&m_timerIOReq->tr_node);
	}
}

static struct timeval t0;

uint32 TaskLocalTimer::getMillis() const {
	// Kickstart 2.0 version
	struct timeval t1;
	long secs, usecs;
	unsigned long tc;

	//FIXME: there must be a more efficient way to determine the
	// milliseconds past since program startup?
	if (t0.tv_secs == 0 && t0.tv_micro == 0) {
		GetSysTime(&t0);
		return 0;
	}

	GetSysTime(&t1);

	secs = t1.tv_secs - t0.tv_secs;
	usecs = t1.tv_micro - t0.tv_micro;

	if (usecs < 0) {
		usecs += 1000000;
		secs--;
	}

	tc = secs * 1000 + usecs / 1000;

	return tc;
}
