
#ifndef EVENTS_AMIGAOS3_H
#define EVENTS_AMIGAOS3_H

#include "common/events.h"
#include "common/queue.h"

#include "backends/events/default/default-events.h"
#include "backends/timer/default/default-timer.h"

class AmigaOS3EventSource : public Common::EventSource {
public:
	AmigaOS3EventSource();
	virtual ~AmigaOS3EventSource();

	virtual bool pollEvent(Common::Event &event);

	DefaultTimerManager *_timerManager = NULL;
};

#endif
