#ifndef AMIGAOS3TIMER_H
#define AMIGAOS3TIMER_H

#include "common/hashmap.h"
#include "common/mutex.h"
#include "common/timer.h"
#include <exec/types.h>

struct Task;

class AmigaOS3TimerManager : public Common::TimerManager {
public:
	AmigaOS3TimerManager();
	virtual ~AmigaOS3TimerManager();

	/**
	 * Install a new timer callback. It will from now be called every interval microseconds.
	 * The timer may be invoked from a separate thread. Hence any timer code should be
	 * written following the same safety guidelines as any other threaded code.
	 *
	 * @note Although the interval is specified in microseconds, the actual timer resolution
	 *       may be lower. In particular, with the SDL backend the timer resolution is 10ms.
	 * @param proc		the callback
	 * @param interval	the interval in which the timer shall be invoked (in microseconds)
	 * @param refCon	an arbitrary void pointer; will be passed to the timer callback
	 * @param id            unique string id of the installed timer. Used by the event recorder
	 * @return	true if the timer was installed successfully, false otherwise
	 */
	virtual bool installTimerProc(TimerProc proc, int32 interval, void *refCon, const Common::String &id);

	/**
	 * Remove the given timer callback. It will not be invoked anymore,
	 * and no instance of this callback will be running anymore.
	 */
	virtual void removeTimerProc(TimerProc proc);

private:
	struct TimerSlot {
		struct Player *player;
		ULONG tics;
		void *refCon;
	};

	Common::Mutex _mutex;  // is this really needed? Can callbacks add or remove timers asynchronously?


	struct Process *_timerProcess;
	struct Task *_timerTask;
	struct Task *_mainTask;
	volatile ULONG _timerSignalMask;
	TimerSlot _allTimers[32];
	UBYTE _numTimers;

	static void __saveds TimerTask(void);
	static AmigaOS3TimerManager *_s_instance;
};

class TaskLocalTimer {
public:
	static TaskLocalTimer *instance();
	static void destroyInstance();

	void delayMillis(uint msecs);
	uint32 getMillis() const;

	struct Device *getTimerBase() const {
		return TimerBase;
	}

private:
	TaskLocalTimer();
	~TaskLocalTimer();

	struct MsgPort *m_timerMP = NULL;
	struct timerequest *m_timerIOReq = NULL;
	// This is a weird situation: there's a global declared as	TimerBase in timer.h
	// But we technically need multiple of them as we potentially talk to the timer device
	// from multiple Tasks (the compiler just doesn't know about)
	struct Device *TimerBase = NULL;
};

#endif  // AMIGAOS3TIMER_H
