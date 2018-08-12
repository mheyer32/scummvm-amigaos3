#ifndef AMIGAOS3TIMER_H
#define AMIGAOS3TIMER_H

#include "common/hashmap.h"
#include "common/mutex.h"
#include "common/timer.h"

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

	template <typename PtrType>
	struct HashPtr {
		uint operator()(PtrType key) const {
			static_assert (sizeof(uint) == sizeof(PtrType), "32bit pointers expected");
			//mhhm, maybe not the best function...
			return (uint)key;
		}
	};

	typedef Common::HashMap<TimerProc, struct Player *, HashPtr<TimerProc> > TimerProcMap;

	Common::Mutex _mutex;
	//	TimerSlot *_head;
	TimerProcMap _callbacks;

	struct Task *_timerTask;

};

#endif  // AMIGAOS3TIMER_H
