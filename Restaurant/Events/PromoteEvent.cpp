#include "PromoteEvent.h"
#include "..\Rest\Restaurant.h"


PromoteEvent::PromoteEvent(int eTime, int oID, double oExtraMoney) :Event(eTime, oID)
{
	ExtraMoney = oExtraMoney;
}

void PromoteEvent::Execute(Restaurant* pRest)
{

}
