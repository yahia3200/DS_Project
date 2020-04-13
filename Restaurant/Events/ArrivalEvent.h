#ifndef __ARRIVAL_EVENT_H_
#define __ARRIVAL_EVENT_H_

#include "Event.h"


//class for the arrival event
class ArrivalEvent: public Event
{
	//info about the order ralted to arrival event
	int OrdSize;	//order size (no. of dishes)
	ORD_TYPE OrdType;		//order type: Normal, vegan, VIP	                
	double OrdMoney;	//Total order money
public:
	ArrivalEvent(int eTime, int oID, ORD_TYPE oType);
	//Add more constructors if needed
	
	//data members are added with the same order provided in the i/p file
	ArrivalEvent(ORD_TYPE oType, int eTime, int oID, int oSize, double oMoney);

	


	virtual void Execute(Restaurant *pRest);	//override execute function

};

#endif