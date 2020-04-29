#include "CancelEvent.h"
#include "..\Rest\Restaurant.h"


CancelEvent::CancelEvent(int eTime, int oID) :Event(eTime, oID)
{
}



void CancelEvent::Execute(Restaurant* pRest)
{

	Order* orgOrder = pRest->RemoveFromWaiting_NO(OrderID);

	//If the order is found and haven't been assigned to a cook yet
	if (orgOrder)
	{
		pRest->RemoveFromDrawing(orgOrder);
		delete orgOrder;
	}
	
}

