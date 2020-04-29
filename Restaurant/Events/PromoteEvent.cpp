#include "PromoteEvent.h"
#include "..\Rest\Restaurant.h"


PromoteEvent::PromoteEvent(int eTime, int oID, double oExtraMoney) :Event(eTime, oID)
{
	ExtraMoney = oExtraMoney;
}

void PromoteEvent::Execute(Restaurant* pRest)
{
	
	Order* orgOrder = pRest->RemoveFromWaiting_NO(OrderID);


	if (orgOrder)
	{
		orgOrder->setTotalMoney(orgOrder->getTotalMoney() + ExtraMoney);
		orgOrder->SetType(TYPE_VIP);
		pRest->ToVIP(orgOrder);
	}
}
	
