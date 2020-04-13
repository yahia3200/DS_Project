#include "CancelEvent.h"
#include "..\Rest\Restaurant.h"


CancelEvent::CancelEvent(int eTime, int oID) :Event(eTime, oID)
{
}



void CancelEvent::Execute(Restaurant* pRest)
{
	Order* dOrder = new Order(OrderID, TYPE_NRM);
	pRest->RemoveFromWaiting_NO(dOrder);
}

