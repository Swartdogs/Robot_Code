// ********** Team 525 (2012) **********
//
// Events Class Header File

#ifndef EVENTS_H
#define EVENTS_H

#include "Base.h"

class Events
{
public:
	virtual void RaiseEvent(UINT8 EventSourceId, UINT32 EventNumber) = 0;
	virtual void WriteToLog(char *LogEntry) = 0;
};

#endif
