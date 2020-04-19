#include "PromoteEvent.h"
#include "..\Rest\Restaurant.h"


PromoteEvent::PromoteEvent(int eTime, int oID, double oExtraMoney) :Event(eTime, oID)
{
	ExtraMoney = oExtraMoney;
}

void PromoteEvent::Execute(Restaurant* pRest)
{
	Order* dOrder = new Order(OrderID, TYPE_NRM);
	Order* orgOrder = pRest->RemoveFromWaiting_NO(dOrder);

	delete dOrder;

	if (orgOrder)
	{
		orgOrder->setTotalMoney(orgOrder->getTotalMoney() + ExtraMoney);
		orgOrder->SetType(TYPE_VIP);
		pRest->ToVIP(orgOrder);
	}
}
	
