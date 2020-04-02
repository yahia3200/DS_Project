#ifndef __PROMOTE_EVENT_H_
#define __PROMOTE_EVENT_H_

#include "Event.h"

//class for the arrival event
class PromoteEvent : public Event
{
	double ExtraMoney;
public:
	PromoteEvent(int eTime, int oID, double oExtraMoney);

	virtual void Execute(Restaurant* pRest);	//override execute function
};

#endif
