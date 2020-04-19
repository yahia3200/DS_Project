#include "CancelEvent.h"
#include "..\Rest\Restaurant.h"


CancelEvent::CancelEvent(int eTime, int oID) :Event(eTime, oID)
{
}



void CancelEvent::Execute(Restaurant* pRest)
{
	Order* dOrder = new Order(OrderID, TYPE_NRM);

	pRest->RemoveFromDrawing(dOrder);
	Order* orgOrfer = pRest->RemoveFromWaiting_NO(dOrder);

	delete dOrder;
	delete orgOrfer;
}

