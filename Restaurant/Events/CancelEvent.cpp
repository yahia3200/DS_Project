#include "CancelEvent.h"
#include "..\Rest\Restaurant.h"


CancelEvent::CancelEvent(int eTime, int oID) :Event(eTime, oID)
{
}



void CancelEvent::Execute(Restaurant* pRest)
{
	Order* dOrder = new Order(OrderID, TYPE_NRM);

	Order* orgOrder = pRest->RemoveFromWaiting_NO(dOrder);

	//If the order is found and haven't been assigned to a cook yet
	if (orgOrder)
	{
		pRest->RemoveFromDrawing(dOrder);
		delete orgOrder;
	}
	delete dOrder;
}

